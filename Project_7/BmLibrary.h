#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <thread>
#include <functional>
#include "BmLibraryC.h"

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

class AbstractStrikeScenario
{
public:
    typedef std::unordered_map<int, std::vector<int>> tStrike;
public:
    // ...
    //! \return 0 - ���������� ��������, ������ 0 - ���-�� ����� �� ��� � ���������� ������� �� �������
    int prepare(tStrike& p_strike);

    //! ���������� ���������� ���������� ���� ��������� � ���������� �������� "�������":
    //! 0 - �����, <0 - ������ (��������, �������� ����������� �����������)
    int waitForAll(std::function<void(double)> p_callback);

protected:
    std::unordered_map<int, IFlightTask*> tasks;
    std::unordered_map<int, IBmElement*> elements;
    //! ������� � �������� ���������� ��� ������������
    std::unordered_map<int, std::map<double, tPoint>>flights;
    std::vector<std::thread> threads;
    double Min{ Config::self().maxT() }, Max{ 0 };
    int successes = 0;

    virtual IFlightTask* flightTaskFor(int p_id, int p_type, double p_t0, double p_angle, const tPoint& p_launch, const tPoint& p_boom) = 0;
    virtual IBmElement* bmElementFor(IFlightTask* p_task) = 0;
};