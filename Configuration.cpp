#include "Configuration.h"
#include "spdlog/spdlog.h"
#include <fstream>
#include <sstream>

Configuration::Configuration(char const *filename)
{
    spdlog::trace("Configuration loading data from: {}", filename);
    std::ifstream input(filename);

    if(input)
    {
        input >> data;
    }
    else
    {
        spdlog::trace("Unable to open configuration file");
    }

    spdlog::trace("Configuration data loaded: {}", to_string());
}

std::string Configuration::to_string() const
{
    std::stringstream out;
    out << data.dump(4);
    return out.str();
}

Configuration::operator bool() const
{
    return !data.is_null() && !data.empty();
}

std::string to_string(Configuration const &cfg)
{
    return cfg.to_string();
}
