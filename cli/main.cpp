#include "alternative-selection.h"
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main() {
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(el::Level::Global, el::ConfigurationType::Format,
                    "%datetime %level %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);

    AlternativeSelection cli;
    LOG(INFO) << "Running CLI";
    cli.Run();
    return 0;
}
