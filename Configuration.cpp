#include "Configuration.h"
#include "spdlog/spdlog.h"
#include <fstream>
#include <sstream>

using json = nlohmann::json;

namespace config
{
    void to_json(nlohmann::json &j, Account const& a)
    {
        j = nlohmann::json{{"folder", a.folder},
                           {"type",   a.type},
                           {"format", a.format}};
    }

    void from_json(nlohmann::json const& j, Account &a)
    {
        j.at("folder").get_to(a.folder);
        j.at("type").get_to(a.type);
        j.at("format").get_to(a.format);
    }
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
        save_config_file_location(filename);
        bool const allow_exceptions = true;
        bool const ignore_comments = true;
        data = json::parse(input, nullptr, allow_exceptions, ignore_comments);
    }
    else
    {
        spdlog::trace("Unable to open configuration file");
    }

    spdlog::trace("Configuration data loaded: {}", to_string());

    return operator bool();
}

void Configuration::generate_skeleton_to(std::string const &filename)
{
    std::ofstream out(filename);

    if(out)
    {
        spdlog::trace("Generating configuration skeleton file in {}", filename);

        out << R"raw({
  "data folder": "data",
  "accounts": {
//    "cgd ordenado": {
//      "folder": "cgd_ordenado",
//      "type": "cgd checking account",
//      "format": "csv"
//    }
  }
})raw";
        spdlog::trace("Skeleton file generation complete");
    }
    else
    {
        spdlog::trace("Unable to open {} for writing, could not generate skeleton file", filename);
    }
}

std::vector<std::string> Configuration::account_list() const
{
    std::vector<std::string> result;

    if (data.contains("accounts"))
        for (auto& [key, value] : data.find("accounts")->items())
        {
            result.push_back(key);
        }

    return result;
}

std::vector<std::tuple<std::string, config::Account>> Configuration::accounts() const
{
    std::vector<std::tuple<std::string, config::Account>> result;

    if (data.contains("accounts"))
        for (auto& [key, value] : data.find("accounts")->items())
        {
            config::Account account;
            from_json(value, account);
            result.emplace_back(key, account);
        }

    return result;
}

std::string Configuration::data_folder() const
{
    std::string result;

    if (data.contains("data folder"))
        result = data["data folder"];

    return result;
}

void Configuration::save_config_file_location(char const *filename)
{
    config_file_path = std::filesystem::path(filename);
}

std::filesystem::path Configuration::config_file_location() const
{
    return config_file_path;
}

std::string to_string(Configuration const &cfg)
{
    return cfg.to_string();
}
