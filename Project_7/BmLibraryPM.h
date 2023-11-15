#pragma once

#include "BmLibrary.h"

//! Класс, реализующий модель движения БР по параболе
class ParabolicModel : public IFlightTask
{
public:
    ParabolicModel(int p_id, int p_type, double p_t0, double p_angle, const tPoint p_launch, const tPoint p_boom)
        : c_id(p_id), c_type(p_type), c_t0(p_t0), c_angle(p_angle), c_launch(p_launch), c_boom(p_boom) {}

    //! \return идентификатор задания
    int id() const override
    {
        return c_id;
    }

    //! \return точку старта БР
    const tPoint& launchPoint() const override
    {
        return c_launch;
    }

    //! \return точку падения БР
    const tPoint& boomPoint() const override
    {
        return c_boom;
    }

    //! \return момент начала движения
    double launchTime() const override
    {
        return c_t0;
    }

    //! \return угол бросания. Фактически - тип траектории (настильная/навесная)
    double angle() const override
    {
        return c_angle;
    }

    //! \return тип БР (фактически - ключ к некоторому справочнику ТТХ БР)
    int type() const override
    {
        return c_type;
    }

private:
    int c_type, c_id;
    double c_t0, c_angle;
    tPoint c_launch, c_boom;
};