#include <iostream>
#include <map>
#include <unordered_map>
#include <cmath>
#include <string>
#include <fstream>
#include "BmLibrary.h"
#include "utility.h"
#include "BmLibrarytEl.h"
#include "BmLibraryPM.h"
#include "BmLibrarySS.h"
#include "BmLibraryLE.h"
#include "BmLibraryL.h"
#include "BmLibraryC.h"

//
int main()
{
    StrikeScenario ss;
    const Config& cfg = Config::self();
    Logger& logger = Logger::instance();

    for (int i = 0; i < cfg.maxBM(); ++i)
    {
        (void)ss.addFlightTask(1,                            //!< тип БР пока всегда 1
            MathHelper::random(0, cfg.maxT()),               //!< задержка старта
            MathHelper::random(cfg.minUS(), cfg.maxUS()),    //!< угол бросания
            tPoint(MathHelper::random(0, cfg.maxS())),       //!< точка старта
            tPoint(2 + MathHelper::random(0, cfg.maxS()))    //!< точка падения
        );
    }

    std::unordered_map<int, std::vector<int>> elements; // id (id полетного задания, который запомнили) -> массив движущихся элементов
    int ok = ss.prepare(elements);
    if (ok == 0)
    {
        ok = ss.waitForAll();
    }

//    std::cout << ss.maxBoomTime() << " " << ss.minLaunchTime() << std::endl;

    if (ok != 0)
    {
        Logger_errors& logger = Logger_errors::instance();
        logger.log(NOT_PREPARE, "This message indicates that 'prepare' was not executed");
    }
    else
    {
        for (double t = ss.minLaunchTime(); t < ss.maxBoomTime() + cfg.stepT(); t += cfg.stepT())
        {
            for (const auto& e : elements)
            {
                for (int id : e.second)
                {
                    // печать в основной лог текущего положения данного элемента в данный момент времени (заключена внутри метода positionOF и логгера в контексте БР)
                    auto p = ss.positionOf(id, t);
                }
            }
        }
    }
}