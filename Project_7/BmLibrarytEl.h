#pragma once

#include "BmLibrary.h"

#define g_const 9.8

//!
class tElement : public IBmElement
{
private:
    IFlightTask* element;
public:
    tElement(IFlightTask* p_element) : element(p_element) {}

    //! \return ����� ������ ��
    virtual const tPoint& launchPoint() const
    {
        return element->launchPoint();
    }

    //! \return ����� ������� ��
    virtual const tPoint& boomPoint() const
    {
        return element->boomPoint();
    }

    //! \return ������ ������ ��������
    virtual double launchTime() const
    {
        return element->launchTime();
    }

    //! \return ������ ��������� ��������
    virtual double boomTime() const
    {
        return sqrt(2 * (element->boomPoint().x - element->launchPoint().x) * tan(element->angle()) / g_const) + element->launchTime();
    }

    //! \return ������� ��������� �� ��������� ������ �������
    // (��������� ���� �� ������� ��������� ������������ � tElement, � �� � StrikeScenario)
    virtual tPoint positionAt(double p_time) const
    {
        tPoint point;

        if (p_time <= launchTime())
        {
            return launchPoint();
        }
        else if (p_time < boomTime())
        {
            point.x = element->launchPoint().x + g_const * (boomTime() - launchTime()) / (2 * sin(element->angle())) * cos(element->angle()) * (p_time - launchTime());
            point.y = g_const * (boomTime() - launchTime()) * (p_time - launchTime()) / 2 - g_const * (p_time - launchTime() )* (p_time - launchTime()) / 2;

            return point;
        }
        return boomPoint();
    }
};