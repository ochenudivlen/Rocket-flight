#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <thread>
#include <functional>
#include "BmLibraryC.h"

//! Описатель точки в пространстве !!!!!
struct tPoint
{
    double x, y;

    tPoint(double p_x = 0, double p_y = 0) : x(p_x), y(p_y) {}

    static const tPoint& none()
    {
        static tPoint s{ std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN() };
        return s;
    }
};

//! Абстрактный описатель полётного задания
class IFlightTask
{
public:
    //! \return идентификатор задания
    virtual int id() const = 0;

    //! \return точку старта БР
    virtual const tPoint& launchPoint() const = 0;

    //! \return точку падения БР
    virtual const tPoint& boomPoint() const = 0;

    //! \return момент начала движения
    virtual double launchTime() const = 0;

    //! \return угол бросания. Фактически - тип траектории (настильная/навесная)
    virtual double angle() const = 0;

    //! \return тип БР (фактически - ключ к некоторому справочнику ТТХ БР)
    virtual int type() const = 0;
};

//! Абстрактный описатель элемента
class IBmElement
{
public:
    //! \return точку старта БР
    virtual const tPoint& launchPoint() const = 0;

    //! \return точку падения БР
    virtual const tPoint& boomPoint() const = 0;

    //! \return момент начала движения
    virtual double launchTime() const = 0;

    //! \return момент окончания движения
    virtual double boomTime() const = 0;

    //! \return текущее положение на указанный момент времени
    // (требуется если вы решаете выполнять интерполяцию в tElement, а не в StrikeScenario)
    virtual tPoint positionAt(double p_time) const = 0;
};

class AbstractStrikeScenario
{
public:
    typedef std::unordered_map<int, std::vector<int>> tStrike;
public:
    // ...
    //! \return 0 - подготовка началась, меньше 0 - что-то пошло не так и подготовка никогда не начнётся
    int prepare(tStrike& p_strike);

    //! Дожидается завершения подготовки всех разверток и возвращает итоговый "диагноз":
    //! 0 - успех, <0 - ошибка (например, нарушены контрактные ограничения)
    int waitForAll(std::function<void(double)> p_callback);

protected:
    std::unordered_map<int, IFlightTask*> tasks;
    std::unordered_map<int, IBmElement*> elements;
    //! Таблица с опорными значениями для интерполяции
    std::unordered_map<int, std::map<double, tPoint>>flights;
    std::vector<std::thread> threads;
    double Min{ Config::self().maxT() }, Max{ 0 };
    int successes = 0;

    virtual IFlightTask* flightTaskFor(int p_id, int p_type, double p_t0, double p_angle, const tPoint& p_launch, const tPoint& p_boom) = 0;
    virtual IBmElement* bmElementFor(IFlightTask* p_task) = 0;
};