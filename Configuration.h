#ifndef BSTY_CONFIGURATION_H
#define BSTY_CONFIGURATION_H

#include "nlohmann/json.hpp"

class Configuration
{
    public:
        explicit Configuration(char const * filename);

        explicit operator bool() const;
        std::string to_string() const;

    private:
        nlohmann::json data;
};

std::string to_string(Configuration const& cfg);


#endif