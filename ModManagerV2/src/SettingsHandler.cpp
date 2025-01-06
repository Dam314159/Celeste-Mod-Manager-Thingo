#include "SettingsHandler.h"

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <iostream>

#include "ConsoleStuff.h"
#include "Logger.h"
#include "json.hpp"  //credit to nlohmann on GitHub for the json handler library
using json = nlohmann::json;

fs::path settingsFilePath = fs::current_path() / "settings.json";
json config;

namespace settings {
void init() {
    std::ifstream settingsFile(settingsFilePath);
    if (settingsFile.is_open()) {
        logger::log("SettingsHandler.cpp, init", "Settings file found.");
        try {
            settingsFile >> config;
            logger::log("SettingsHandler.cpp, init", "Settings file loaded.");
        } catch (const json::parse_error &e) {
            settingsFile.close();

            logger::log("SettingsHandler.cpp, init", "ERROR: Settings file could not be loaded. Settings file may be corrupted or malformed.");
            colour::cerr("Your settings file could not be loaded as it might be corrupted.\n", "RED");
            colour::cerr("You can fix this by going to \"" + settingsFilePath.string() + "\" and fixing it manually with a text editor or by deleting it (It will be recreated with the default settings the next time this is run).\n", "RED");

            std::string errorMessage = "Exited due to \"" + std::string(e.what()) + "\"";
            exitOnEnterPress(1, errorMessage);
        }

    } else {
        logger::log("SettingsHandler.cpp, init", "Settings file not found. Creating settings file...");

        config["firstTime?"] = true;
        config["modsFolderPath"] = "";
        config["showPresets?"] = true;

        std::ofstream settingsFile(settingsFilePath);
        if (settingsFile.is_open()) {
            settingsFile << config.dump(4);
            logger::log("SettingsHandler.cpp, init", "Settings file created.");
        } else {
            logger::log("SettingsHandler.cpp, init", "WARNING: Settings file not created. Using default settings.");
        }
        settingsFile.close();
    }
}

}  // namespace settings