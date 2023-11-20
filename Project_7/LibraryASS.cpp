#include "BmLibrarySS.h"
#include "BmLibrary.h"
#include "thread"
#include <shared_mutex>
#include <mutex>

std::mutex mutex_e;
std::shared_mutex smtx;
std::condition_variable cv;

int AbstractStrikeScenario::prepare(tStrike& p_strike)
{
    for (auto ft : tasks)
    {
        IBmElement* e;

        std::thread thr([&](const std::pair<const int, IFlightTask*>& ft)
            {
                std::lock_guard<std::mutex> guard(mutex_e);

                e = elements.insert({ ft.second->id(), bmElementFor(ft.second) }).first->second;
                p_strike[ft.second->id()] = { ft.second->id() };

                //!Для проверки количества элементов
                int count = 0;

                for (double t = e->launchTime(), tE = e->boomTime(); t < tE; t += 0.1)
                {
                    // заполнить таблицу (собственную или в IBmElement) с опорными значениями для интерполяции
                    flights[ft.second->id()][t] = e->positionAt(t);
                    count++;
                }

                // проверить, что для момента e->boomTime() данные тоже посчитаны
                if (flights[ft.second->id()].count(e->boomTime()) == 0)
                {
                    flights[ft.second->id()][e->boomTime()] = e->positionAt(e->boomTime());
                    count++;
                }

                double tMin = e->launchTime(),
                    tMax = e->boomTime();

                // проверяем, не является ли парабола вырожденной
                if (tMax - tMin <= 0)
                {
                    Logger_errors& logger = Logger_errors::instance();
                    logger.log(RANGEofTIME, "This message is about an incorrectly calculated time range");
                }
                else
                {
                    successes++;
                }

                if (tMin < Min)
                {
                    Min = tMin;
                }

                if (tMax > Max)
                {
                    Max = tMax;
                }

                // если что-то пошло не так - жалоба в лог и выход с ошибкой
                if (count != flights[ft.second->id()].size())
                {
                    Logger_errors& logger = Logger_errors::instance();
                    logger.log(INTERPOLATION, "This is a message about incorrect interpolation");
                }

                cv.notify_one();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }, ft);
        threads.push_back(move(thr));
    }

    return 0;
}

int AbstractStrikeScenario::waitForAll(std::function<void(double)> p_callback)
{
    std::unique_lock<std::mutex> ul(mutex_e);
    cv.wait(ul, [&]()
        {
            int taskSize = tasks.size();
            smtx.lock_shared();
            if (p_callback) p_callback(static_cast<double>(successes) / taskSize);
            smtx.unlock_shared();
            //количество задач равно количеству законченных заданий
            return successes == taskSize;
        });

    for (auto& thr : threads)
    {
        thr.join();
    }

    // TODO: Проверить контрактные ограничения
    std::map<double, int> flying;
    for (const auto& e : elements)
    {
        ++flying[e.second->launchTime()];
        --flying[e.second->boomTime()];
    }

    int rockets = 0;
    for (std::map<double, int>::iterator it = flying.begin(); it != flying.end(); ++it)
    {
        rockets += it->second;

        if (rockets > Config::self().premittedBMs())
        {
            Logger_errors& logger = Logger_errors::instance();
            logger.log(CONTRACT_RESTRICTION, "This is a contract violation message");
            return -1;
        }
    }
    return 0;
}