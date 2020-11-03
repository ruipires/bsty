#include <spdlog/spdlog.h>
#include <cxxopts.hpp>
#include "Configuration.h"

void process_accounts(Configuration const& cfg);

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

        auto const configurationFile = result["config"].as<std::string>();
        auto const generateConfig = result["generate"].as<bool>();
        auto const verbose = result["verbose"].as<bool>();

        if(verbose)
        {
            spdlog::set_level(spdlog::level::trace);
            spdlog::trace("Verbose mode enabled");
        }

        Configuration cfg;
        if(generateConfig)
        {
            spdlog::info("Generating configuration file to {}", configurationFile);
            cfg.generate_skeleton_to(configurationFile);
        }
        else
        {
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
    auto const accounts = cfg.account_list();
    spdlog::trace("Processing the following accounts [{}]", fmt::join(accounts, ", "));

    if(accounts.empty())
    {
        spdlog::error("No accounts defined in the configuration file");
        return;
    }

    for(auto const& account: accounts)
    {
        spdlog::info("Processing account {}", account);

    }
}
