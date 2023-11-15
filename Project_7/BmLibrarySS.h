#pragma once

#include "BmLibrary.h"
#include <unordered_map>
#include <map>
#include "BmLibraryLE.h"
#include "BmLibraryL.h"
#include "BmLibraryC.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex_e;
std::condition_variable cv;




//!
class StrikeScenario
{
public:
    typedef std::unordered_map<int, std::vector<int>> tStrike;
public:
    //! ��������� �������� ������� � ��������
    IFlightTask* addFlightTask(int p_type, double p_t0, double p_angle, const tPoint& p_launch, const tPoint& p_boom)
    {
        IFlightTask* ft = nullptr;

        if (p_type == 1)
        {
            ft = new ParabolicModel(tasks.size() + 1, p_type, p_t0, p_angle, p_launch, p_boom);
        }
        else
        {
            // TODO: ������ �� ���������������� ��� ������ ��������
            Logger_errors& logger = Logger_errors::instance();
            logger.log(TYPEofMODEL, "This message is about an unsupported model type");
        }

        if (ft != nullptr)
        {
            tasks.insert({ ft->id(), ft });
        }

        return ft;
    }

    double tMin = Config::self().maxT(),
           tMax = 0;

    std::vector<std::thread> threads;

    static void worker(IBmElement* e, std::unordered_map<int, IFlightTask*>::iterator ft, std::unordered_map<int, IBmElement*>& elements,
        std::unordered_map<int, std::vector<int>>& p_strike, double& tMin, double& tMax,
        std::unordered_map<int, std::map<double, tPoint>>& flights)
//    void worker()
    {
        std::lock_guard<std::mutex> guard(mutex_e);

        e = elements.insert({ ft->second->id(), new tElement(ft->second) }).first->second;
        p_strike[ft->second->id()] = { ft->second->id() };

        //!��� �������� ���������� ���������
        int count = 0;

        if (e->launchTime() < tMin)
        {
            tMin = e->launchTime();
        }

        if (e->boomTime() > tMax)
        {
            tMax = e->boomTime();
        }

        for (double t = e->launchTime(), tE = e->boomTime(); t < tE; t += 0.1)
        {
            // ��������� ������� (����������� ��� � IBmElement) � �������� ���������� ��� ������������
            flights[ft->second->id()][t] = e->positionAt(t);
            count++;
        }

        // ���������, ��� ��� ������� e->boomTime() ������ ���� ���������
        if (flights[ft->second->id()].count(e->boomTime()) == 0)
        {
            flights[ft->second->id()][e->boomTime()] = e->positionAt(e->boomTime());
            count++;
        }

        // ���� ���-�� ����� �� ��� - ������ � ��� � ����� � �������
        if (count != flights[ft->second->id()].size())
        {
            Logger_errors& logger = Logger_errors::instance();
            logger.log(INTERPOLATION, "This is a message about incorrect interpolation");
        }

        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    //!
    int prepare(tStrike& p_strike)
    {
        for (const auto& ft : tasks)
        {
            IBmElement* e;

            std::thread thr(&StrikeScenario::worker, e, ft, elements, p_strike, tMin, tMax, flights);
//            std::thread thr(&StrikeScenario::worker, this);
            threads.push_back(move(thr));

            /*
            IBmElement* e = elements.insert({ ft.second->id(), new tElement(ft.second) }).first->second;
            p_strike[ft.second->id()] = { ft.second->id() };

            //!��� �������� ���������� ���������
            int count = 0;

            if (e->launchTime() < tMin)
            {
                tMin = e->launchTime();
            }

            if (e->boomTime() > tMax)
            {
                tMax = e->boomTime();
            }

            for (double t = e->launchTime(), tE = e->boomTime(); t < tE; t += 0.1)
            {
                // ��������� ������� (����������� ��� � IBmElement) � �������� ���������� ��� ������������
                flights[ft.second->id()][t] = e->positionAt(t);
                count++;
            }

            // ���������, ��� ��� ������� e->boomTime() ������ ���� ���������
            if (flights[ft.second->id()].count(e->boomTime()) == 0)
            {
                flights[ft.second->id()][e->boomTime()] = e->positionAt(e->boomTime());
                count++;
            }

            // ���� ���-�� ����� �� ��� - ������ � ��� � ����� � �������
            if (count != flights[ft.second->id()].size())
            {
                Logger_errors& logger = Logger_errors::instance();
                logger.log(INTERPOLATION, "This is a message about incorrect interpolation");
            }
            */
        }

        // TODO: ��������� ����������� �����������
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
            }
        }

        // TODO: ��������� ��������� �������� �������������
        double tRange = tMax - tMin;

        if (tRange < 0)
        {
            Logger_errors& logger = Logger_errors::instance();
            logger.log(RANGEofTIME, "This message is about an incorrectly calculated time range");
        }

        return 0;
    }

    int waitForAll()
    {
        std::unique_lock<std::mutex> ul(mutex_e);
        cv.wait(ul, [&]()
            {
                //���������� ����� ����� ���������� ����������� �������
                return threads.size() == tasks.size();
            });

        for (auto& thr : threads)
        {
            thr.join();
        }

        return 0;
    }

    //!
    tPoint positionOf(int p_id, double p_time)
    {
        // TODO
        // ������� �1
        // �������� ������� � �������� ���������� ��� ������������
        // ���� ��� ������� id ����� ��� - ������������
        // ���� ���� - ��������� �� � ������ ��������� �������� �� ������ p_time
        // 
        // ������� �2
        // ����� IBmElement �� ��� id, ���� ������ ��� - ������������
        // ���� ���� - ������������ ��� ������������ �� ������ p_time

        //���������� �������� �2:
        if (elements.count(p_id) == 0)
        {
            Logger_errors& logger = Logger_errors::instance();
            logger.log(NOT_FOUND, "This message is about IBmElement not found");
        }
        else
        {
            Logger& logger = Logger::instance();

            if (p_time < elements[p_id]->launchTime())
            {
                logger.log(BEGIN, "This is a message that the rocket has not yet been launched", elements[p_id]->launchPoint());
                return elements[p_id]->launchPoint();
            }
            else if (p_time < elements[p_id]->boomTime())
            {
                tPoint position = { 0, 0 };

//                std::map<double, tPoint>::iterator t_low = lower_bound(flights[p_id].begin(), flights[p_id].end(), p_time);
                std::map<double, tPoint>::iterator t_low = flights[p_id].lower_bound(p_time);

                if (p_time == t_low->first)
                {
                    position = t_low->second;
                }
                else
                {
                    --t_low;
//                    std::map<double, tPoint>::iterator t_up = upper_bound(flights[p_id].begin(), flights[p_id].end(), p_time);
                    std::map<double, tPoint>::iterator t_up = flights[p_id].upper_bound(p_time);

                    position.x = t_low->second.x + (t_up->second.x - t_low->second.x) * (p_time - t_low->first) / (t_up->first - t_low->first);
                    position.y = t_low->second.y + (t_up->second.y - t_low->second.y) * (p_time - t_low->first) / (t_up->first - t_low->first);
 //                   position.y = elements[p_id]->positionAt(p_time).y;
                }

                logger.log(FLIGHT, "This is a message that the rocket is in flight", position);
                return position;
            }
            logger.log(END, "This is a message that the rocket has fallen", elements[p_id]->boomPoint());
            return elements[p_id]->boomPoint();
        }
    }

    double minLaunchTime()
    {
        return tMin;
    }

    double maxBoomTime()
    {
        return tMax;
    }

    //! ������� � �������� ���������� ��� ������������
    std::unordered_map<double, tPoint>interpolation;
    std::unordered_map<int, std::map<double, tPoint>>flights;

private:
    std::unordered_map<int, IFlightTask*> tasks;
    std::unordered_map<int, IBmElement*> elements;
};