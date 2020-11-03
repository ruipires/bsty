#include "Configuration.h"
#include "spdlog/spdlog.h"
#include <fstream>
#include <sstream>

Configuration::Configuration()
{
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

bool Configuration::load(std::string const& filename)
{
    return load(filename.c_str());
}

bool Configuration::load(char const *filename)
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

    return operator bool();
}

void Configuration::generate_skeleton_to(std::string const &filename) const
{
    std::ofstream out(filename);

    if(out)
    {
        spdlog::trace("Generating configuration skeleton file in {}", filename);
        out << "{\n";
        out << "    \"data folder\": \"data\"\n";
        out << "}\n";
        spdlog::trace("Skeleton file generation complete");
    }
    else
    {
        spdlog::trace("Unable to open {} for writing, could not generate skeleton file", filename);
    }
}

std::string to_string(Configuration const &cfg)
{
    return cfg.to_string();
}
