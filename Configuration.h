#ifndef BSTY_CONFIGURATION_H
#define BSTY_CONFIGURATION_H

#include "nlohmann/json.hpp"

class Configuration
{
    public:
        Configuration();

        bool load(std::string const& filename);
        bool load(char const* filename);

        void generate_skeleton_to(std::string const& filename) const;

        explicit operator bool() const;
        std::string to_string() const;

    private:
        nlohmann::json data;
};

std::string to_string(Configuration const& cfg);


#endif