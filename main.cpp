#include <filesystem>
#include <spdlog/spdlog.h>
#include <cxxopts.hpp>
#include "Configuration.h"
#include "cgd/ContaOrdem.h"

void process_accounts(Configuration const& cfg);
void process_account(std::string const& name, config::Account const& account);

int main(int argc, char **argv)
{
    spdlog::info("Initializing");

    try
    {
        cxxopts::Options options("BSTY", "Bank Statement into YNAB (importer/converter)");
        options.add_options()
                ("g,generate", "Generate configuration file (to config.json)")
                ("c,config", "Configuration file name", cxxopts::value<std::string>()->default_value("config.json"))
                ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("true"))
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

        if(verbose)
        {
            spdlog::set_level(spdlog::level::trace);
            spdlog::trace("Verbose mode enabled");
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

            if(cfg)
            {
                spdlog::trace("Configuration data ok");
                process_accounts(cfg);
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

void process_accounts(Configuration const& cfg)
{
    auto const accounts = cfg.accounts();
    std::vector<std::string> names;
    std::for_each(accounts.begin(), accounts.end(), [&names](auto &i){ names.push_back(std::get<0>(i)); });
    spdlog::trace("Processing the following accounts [{}]", fmt::join(names, ", "));

    if(accounts.empty())
    {
        spdlog::error("No accounts defined in the configuration file");
        return;
    }

    for(auto const& [name, account]: accounts)
        process_account(name, account);
}

void process_account(std::string const& name, config::Account const& account)
{
    namespace fs = std::filesystem;
    spdlog::info("Processing account {}", name);

    auto const dir = account.folder;

    for (auto const& entry : fs::directory_iterator(dir))
    {
        auto const filename = entry.path().filename().string();
        auto const fullpath = entry.path().string();
        spdlog::info("* loading {}", filename);
        spdlog::trace("  * fullpath:{}", fullpath);

        cgd::ContaOrdem::load_csv(fullpath);
    }
}