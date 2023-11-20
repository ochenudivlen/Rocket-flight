#pragma once

#include "BmLibrary.h"
#include <unordered_map>
#include <map>
#include "BmLibraryLE.h"
#include "BmLibraryL.h"
#include "BmLibraryC.h"
#include "BmLibraryPM.h"
#include "BmLibrarytEl.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <functional>

//!
class StrikeScenario: public AbstractStrikeScenario
{
public:
    typedef std::unordered_map<int, std::vector<int>> tStrike;
public:
    IFlightTask* flightTaskFor(int p_id, int p_type, double p_t0, double p_angle, const tPoint& p_launch, const tPoint& p_boom) override
    {
        return new (std::nothrow) ParabolicModel(p_id, p_type, p_t0, p_angle, p_launch, p_boom);
    }

    IBmElement* bmElementFor(IFlightTask* p_task) override
    {
        return new (std::nothrow) tElement(p_task);
    }

    //! Добавляет полетное задание в сценарий
    IFlightTask* addFlightTask(int p_type, double p_t0, double p_angle, const tPoint& p_launch, const tPoint& p_boom)
    {
        IFlightTask* ft = nullptr;

        if (p_type == 1)
        {
            ft = flightTaskFor(tasks.size() + 1, p_type, p_t0, p_angle, p_launch, p_boom);
        }
        else
        {
            // TODO: жалоба на неподдерживаемый тип модели движения
            Logger_errors& logger = Logger_errors::instance();
            logger.log(TYPEofMODEL, "This message is about an unsupported model type");
        }

        if (ft != nullptr)
        {
            tasks.insert({ ft->id(), ft });
        }

        return ft;
    }

    //!
    tPoint positionOf(int p_id, double p_time)
    {
        // TODO
        // Вариант №1
        // получить таблицу с опорными значениями для интерполяции
        // если для нужного id такой нет - пожаловаться
        // если есть - посчитать на её основе положение элемента на момент p_time
        // 
        // Вариант №2
        // найти IBmElement по его id, если такого нет - пожаловаться
        // если есть - делегировать ему интерполяцию на момент p_time

        //Реализация варианта №2:
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
                std::map<double, tPoint>::iterator t_low = flights[p_id].lower_bound(p_time);

                if (p_time == t_low->first)
                {
                    position = t_low->second;
                }
                else
                {
                    t_low--;
                    std::map<double, tPoint>::iterator t_up = flights[p_id].upper_bound(p_time);

                    position.x = t_low->second.x + (t_up->second.x - t_low->second.x) * (p_time - t_low->first) / (t_up->first - t_low->first);
                    position.y = t_low->second.y + (t_up->second.y - t_low->second.y) * (p_time - t_low->first) / (t_up->first - t_low->first);
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
        return Min;
    }

    double maxBoomTime()
    {
        return Max;
    }
};