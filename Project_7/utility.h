#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>

//!
class MathHelper
{
public:
    static const double g;
    static const double eps;

public:
    static double random(double p_min, double p_max)
    {
//        srand(time(NULL));;
        return p_min + (p_max - p_min) * rand() / static_cast<double>(RAND_MAX);
    }
};