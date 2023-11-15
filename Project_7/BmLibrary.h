#pragma once

#include <iostream>

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