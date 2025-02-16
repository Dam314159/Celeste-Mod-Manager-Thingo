#include "SettingsHandler.h"

nlohmann::json config;
fs::path settingsFilePath = fs::current_path() / "settings.json";

namespace settings {

void init() {
    std::ifstream settingsFile(settingsFilePath);
    config = {};
    if (settingsFile.is_open()) {
        logger::log({"SettingsHandler.cpp", "settings::init"}, "Settings file found at " + settingsFilePath.string());
        try {
            config = json::parse(settingsFile);
            logger::log({"SettingsHandler.cpp", "settings::init"}, "Settings file loaded as " + config.dump(4));
        } catch (const json::parse_error &e) {
            settingsFile.close();

            logger::error({"SettingsHandler.cpp", "settings::init"}, "ERROR: Settings file could not be loaded. Settings file may be corrupted or malformed.");
            colour::cerr("Your settings file could not be loaded as it might be corrupted.\n", "RED");
            colour::cerr("You can fix this by going to \"" + settingsFilePath.string() + "\" and fixing it manually with a text editor or by deleting it (It will be recreated with the default settings the next time this is run).\n", "RED");

            std::string errorMessage = "Exited due to \"" + std::string(e.what()) + "\"";
            exitOnEnterPress(1, errorMessage);
        }

    } else {
        logger::log({"SettingsHandler.cpp", "settings::init"}, "Settings file not found. Creating settings file...");

        config["firstTime?"] = true;
        config["modsFolderPath"] = "";
        config["showPresets?"] = true;

        std::ofstream settingsFile(settingsFilePath);
        if (settingsFile.is_open()) {
            settingsFile << config.dump(4);
            logger::log({"SettingsHandler.cpp", "settings::init"}, "Settings file created and loaded with " + config.dump(4));
        } else {
            logger::warn({"SettingsHandler.cpp", "settings::init"}, "Settings file not created. Using default settings : " + config.dump(4));
        }
        settingsFile.close();
    }
}

json getSettings() {
    return config;
}

}  // namespace settings