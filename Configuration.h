#ifndef BSTY_CONFIGURATION_H
#define BSTY_CONFIGURATION_H

#include "nlohmann/json.hpp"

namespace config
{
    struct Account
    {
        std::string folder;
        std::string type;
        std::string format;
    };

    void to_json(nlohmann::json& j, Account const& a);
    void from_json(nlohmann::json const& j, Account& a);
}

class Configuration
{
    public:
        Configuration() = default;

        bool load(std::string const& filename);
        bool load(char const* filename);

        static void generate_skeleton_to(std::string const& filename) ;

        std::vector<std::string> account_list() const;
        std::vector<std::tuple<std::string, config::Account>> accounts() const;

        explicit operator bool() const;
        std::string to_string() const;

    private:
        nlohmann::json data;
};

std::string to_string(Configuration const& cfg);


#endif