#include <filesystem>
#include <spdlog/spdlog.h>
#include <fmt/ostream.h>
#include <cxxopts.hpp>
#include "Configuration.h"
#include "core/ParserMapping.h"
#include "cgd/ContaOrdem.h"

void process_accounts(Configuration const& cfg,
                      bsty::core::ParserMapping const& parsers);
void process_account(std::string const& name,
                     config::Account const& account,
                     Configuration const& cfg,
                     bsty::core::ParserMapping const& parsers);
std::filesystem::path get_absolute_path(std::string const& account_folder, Configuration const& cfg);

int main(int argc, char **argv)
{
    spdlog::info("Initializing");

    try
    {
        cxxopts::Options options("BSTY", "Bank Statement into YNAB (importer/converter)");
        options.add_options()
                ("g,generate", "Generate configuration file (to config.json)")
                ("c,config", "Configuration file name", cxxopts::value<std::string>()->default_value("config.json"))
                ("v,verbose", "Verbose output (debug output)", cxxopts::value<bool>()->default_value("false"))
                ("x,extra-verbose", "Extra verbose output (debug + trace output)", cxxopts::value<bool>()->default_value("false"))
                ("h,help", "Print usage")
                ;
        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            fmt::print("{}\n", options.help());
            exit(0);
        }

        std::string const configurationFile = result["config"].as<std::string>();
        auto const generateConfig = result["generate"].as<bool>();
        auto const verbose = result["verbose"].as<bool>();
        auto const trace = result["extra-verbose"].as<bool>();

        if(verbose)
        {
            spdlog::set_level(spdlog::level::debug);
            spdlog::debug("Verbose mode enabled (debug output will be visible)");
        }

        if(trace)
        {
            spdlog::set_level(spdlog::level::trace);
            spdlog::debug("Extra verbose mode enabled (trace output will be visible)");
        }

        if(generateConfig)
        {
            spdlog::info("Generating configuration file to {}", configurationFile);
            Configuration::generate_skeleton_to(configurationFile);
        }
        else
        {
            Configuration cfg;
            cfg.load(configurationFile);

            bsty::core::ParserMapping parsers;
            parsers.registerParser("cgd checking account", "csv", std::make_unique<cgd::ContaOrdemParser>());

            if(cfg)
            {
                spdlog::trace("Configuration data ok");
                process_accounts(cfg, parsers);
            }
            else
                spdlog::error("Error loading configuration data from {}", configurationFile);
        }
    }
    catch(std::exception const& e)
    {
        spdlog::error("Something went wrong, caught the following exception {}", e.what());
    }
    catch(...)
    {
        spdlog::error("Something went wrong, caught an unknown exception");
    }
}

void process_accounts(Configuration const& cfg,
                      bsty::core::ParserMapping const& parsers)
{
    auto const accounts = cfg.accounts();
    std::vector<std::string> names;
    std::for_each(accounts.begin(), accounts.end(), [&names](auto &i){ names.push_back(std::get<0>(i)); });

    auto msg=fmt::format("Processing the following accounts [{}]", fmt::join(names, ", "));
    spdlog::trace(msg);

    if(accounts.empty())
    {
        spdlog::error("No accounts defined in the configuration file");
        return;
    }

    for(auto const& [name, account]: accounts)
        process_account(name, account, cfg, parsers);
}


void process_account(std::string const& name,
                     config::Account const& account,
                     Configuration const& cfg,
                     bsty::core::ParserMapping const& parsers)
{
    namespace fs = std::filesystem;
    spdlog::info("Processing account {}", name);

    auto const accountType = account.type;
    auto const statementFormat = account.format;

    auto const parser = parsers.getParser(accountType, statementFormat);

    if(parser == nullptr)
    {
        spdlog::error("Unable to load parser for '{}' in '{}' format.", accountType, statementFormat);
        return;
    }

    auto const dir = get_absolute_path(account.folder, cfg);

    if(dir == fs::path())
        spdlog::error("Unable to find account directory using data_folder:({}), account_folder ({}), cfg_file_location({})",
                      cfg.data_folder(), account.folder, cfg.config_file_location().remove_filename());
    else
        for (auto const& entry : fs::directory_iterator(dir))
        {
            auto const filename = entry.path().filename().string();
            auto const full_path = entry.path().string();
            spdlog::info("* loading {}", filename);
            spdlog::trace("  * full_path:{}", full_path);

            auto const &data = parser->loadFrom(full_path);

            spdlog::debug("* account #{} \"{}\", statement taken on {}, covering {} to {}", *data.getAccountCode(),
                          *data.getAccountDescription(),
                          *data.getReportDate(),
                          *data.getBeginDate(),
                          *data.getEndDate());

            for (auto const &row: data.getRows())
            {
                spdlog::trace("* date:'{}', payee:'{}' desc:'{}', in:'{}', out:'{}'",
                              to_string(row.date),
                              row.payee,
                              row.memo,
                              to_string(row.inflow),
                              to_string(row.outflow));
            }
        }
}

std::filesystem::path get_absolute_path(std::string const& account_folder, Configuration const& cfg)
{
    using std::filesystem::is_directory;
    std::filesystem::path result;


    // search for a valid path using the following rules/precedence
    // 1 - account defines an absolute path representing an existing directory
    // 2 - "data folder" configuration + account folder form a valid path representing an existing directory
    // 3 - config file location + data folder + account folder form a valid path representing an existing directory

    std::filesystem::path const account_folder_path(account_folder);

    if(is_directory(account_folder_path) && account_folder_path.is_absolute()) // 1
        result = account_folder_path;
    else
    {
        // 2
        std::filesystem::path const base_path(cfg.data_folder());
        auto account_path = base_path / account_folder_path;

        if(is_directory(account_path))
            result = account_path;
        else
        {
            // 3
            auto const config_file_folder = cfg.config_file_location().remove_filename();

            account_path = config_file_folder / base_path / account_folder_path;

            if(is_directory(account_path))
                result = account_path;
        }
    }

    return result;
}
