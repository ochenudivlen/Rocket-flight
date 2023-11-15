#pragma once

//!
class Config
{
public:
    static const Config& self()
    {
        static Config conf;
        return conf;
    }

    // ����������� �����������
    int premittedBMs() const //!< ����������� ���������� ���������� ��, ������������ ����������� � �����
    {
        int maxCM = 3;
        return maxCM;
    }

    // ��������� ��� ��������
    int maxBM() const //!< ���������� �� � �����
    {
        int max_hit = 3;
        return max_hit;
    }

    double stepT() const //!< ��� ���������� �������
    {
        double step = 0.05;
        return step;
    }

    // ��������� ��� ������������ �������� ������
    double maxS() const  //!< ������������ �������� ����� ������ �� [0;0]
    {
        double max_s = 0.5;
        return max_s;
    }

    double minUS() const //!< ����������� ���� ��������
    {
        double min_angle = 0.1;
        return min_angle;
    }

    double maxUS() const //!< ������������ ���� �������� (��� 0.9 �� ��������� 52-� ��������)
    {
        double max_angle = 0.9;
        return max_angle;
    }

    double maxT() const  //!< ������������ �������� ������� ������
    {
        double max_t = 2.5;
        return max_t;
    }
};