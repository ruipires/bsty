#ifndef BSTY_CONFIGURATION_H
#define BSTY_CONFIGURATION_H

#include "nlohmann/json.hpp"
#include <filesystem>

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

        [[nodiscard]] std::vector<std::string> account_list() const;
        [[nodiscard]] std::vector<std::tuple<std::string, config::Account>> accounts() const;

        [[nodiscard]] std::string data_folder() const;

        explicit operator bool() const;
        [[nodiscard]] std::string to_string() const;

        std::filesystem::path config_file_location() const;

    private:
        void save_config_file_location(char const* filename);

        nlohmann::json data;
        std::filesystem::path config_file_path;
};

std::string to_string(Configuration const& cfg);


#endif