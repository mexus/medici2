#include "performance.h"

int main()
{
    logxx::Log log("Performance");

    Performance perf;
    log(logxx::info) << "Testing OneSwap mixer engine" << logxx::endl;
    perf.Run(MixersFactory::ONE_SWAP);
    log(logxx::info) << "Testing FullCapacity mixer engine" << logxx::endl;
    perf.Run(MixersFactory::FULL_CAPACITY);
}

