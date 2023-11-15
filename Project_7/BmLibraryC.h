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

    //  онтрактные ограничени€
    int premittedBMs() const //!< максимально допустимое количество Ѕ–, одновременно наход€щихс€ в полЄте
    {
        int maxCM = 3;
        return maxCM;
    }

    // ѕараметры дл€ имитации
    int maxBM() const //!< количество Ѕ– в ударе
    {
        int max_hit = 3;
        return max_hit;
    }

    double stepT() const //!< шаг модельного времени
    {
        double step = 0.05;
        return step;
    }

    // ѕараметры дл€ рандомизации тестовых данных
    double maxS() const  //!< максимальное удаление точки старта от [0;0]
    {
        double max_s = 0.5;
        return max_s;
    }

    double minUS() const //!< минимальный угол бросани€
    {
        double min_angle = 0.1;
        return min_angle;
    }

    double maxUS() const //!< максимальный угол бросани€ (при 0.9 не превышает 52-х градусов)
    {
        double max_angle = 0.9;
        return max_angle;
    }

    double maxT() const  //!< максимальна€ задержка времени старта
    {
        double max_t = 2.5;
        return max_t;
    }
};