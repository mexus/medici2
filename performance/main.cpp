#include <easylogging++.h>
#include "performance.h"

INITIALIZE_EASYLOGGINGPP

int main() {
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(el::Level::Global, el::ConfigurationType::Format,
                    "%datetime %level %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);

    Performance perf;
    LOG(INFO) << "Testing OneSwap mixer engine";
    perf.Run(MixersFactory::ONE_SWAP);
    LOG(INFO) << "Testing FullCapacity mixer engine";
    perf.Run(MixersFactory::FULL_CAPACITY);
}
