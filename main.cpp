#include "spdlog/spdlog.h"
#include "Configuration.h"

int main()
{
    spdlog::info("Initializing");
    spdlog::set_level(spdlog::level::trace);

    char const * configurationFile = "config.json";
    Configuration cfg(configurationFile);

    if(cfg)
    {
        spdlog::trace("Configuration data ok");
    }
    else
    {
        spdlog::error("Error loading configuration data from {}", configurationFile);
    }
}
