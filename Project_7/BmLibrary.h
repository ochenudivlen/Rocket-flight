#pragma once

#include <iostream>

//! ��������� ����� � ������������ !!!!!
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

//! ����������� ��������� �������� �������
class IFlightTask
{
public:
    //! \return ������������� �������
    virtual int id() const = 0;

    //! \return ����� ������ ��
    virtual const tPoint& launchPoint() const = 0;

    //! \return ����� ������� ��
    virtual const tPoint& boomPoint() const = 0;

    //! \return ������ ������ ��������
    virtual double launchTime() const = 0;

    //! \return ���� ��������. ���������� - ��� ���������� (����������/��������)
    virtual double angle() const = 0;

    //! \return ��� �� (���������� - ���� � ���������� ����������� ��� ��)
    virtual int type() const = 0;
};

//! ����������� ��������� ��������
class IBmElement
{
public:
    //! \return ����� ������ ��
    virtual const tPoint& launchPoint() const = 0;

    //! \return ����� ������� ��
    virtual const tPoint& boomPoint() const = 0;

    //! \return ������ ������ ��������
    virtual double launchTime() const = 0;

    //! \return ������ ��������� ��������
    virtual double boomTime() const = 0;

    //! \return ������� ��������� �� ��������� ������ �������
    // (��������� ���� �� ������� ��������� ������������ � tElement, � �� � StrikeScenario)
    virtual tPoint positionAt(double p_time) const = 0;
};