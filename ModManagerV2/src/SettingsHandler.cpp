#include "SettingsHandler.h"

nlohmann::json config;
fs::path settingsFilePath = fs::current_path() / "settings.json";

namespace settings {

void init() {
    logger::functionCall("settings::init", {});
    std::ifstream settingsFile(settingsFilePath);
    config = {};
    if (settingsFile.is_open()) {
        logger::log("Settings file found at " + settingsFilePath.string());
        try {
            config = json::parse(settingsFile);
            logger::log("Settings file loaded as " + config.dump(4));
        } catch (const json::parse_error &e) {
            settingsFile.close();

            logger::critical("Settings file could not be loaded. Settings file may be corrupted or malformed.");
            colour::cerr("Your settings file could not be loaded as it might be corrupted.\n", "RED");
            colour::cerr("You can fix this by going to \"" + settingsFilePath.string() + "\" and fixing it manually with a text editor or by deleting it (It will be recreated with the default settings the next time this is run).\n", "RED");

            std::string errorMessage = "Exited due to \"" + std::string(e.what()) + "\"";
            exitOnEnterPress(1, errorMessage);
        }

    } else {
        logger::log("Settings file not found. Creating settings file...");

        // Default settings
        config["firstTime?"] = true;
        config["modsFolderPath"] = "";
        config["showPresets?"] = true;
        config["logging?"] = true;

        std::ofstream settingsFile(settingsFilePath);
        if (settingsFile.is_open()) {
            settingsFile << config.dump(4);
            logger::log("Settings file created and loaded with " + config.dump(4));
        } else {
            logger::warn("Settings file not created. Using default settings : " + config.dump(4));
        }
        settingsFile.close();
    }
    logger::functionExit();
}

json getSettings() {
    return config;
}

}  // namespace settings