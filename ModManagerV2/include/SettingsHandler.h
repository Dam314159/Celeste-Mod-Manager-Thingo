#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "ConsoleStuff.h"
#include "Logger.h"
#include "json.hpp"  //credit to nlohmann on GitHub for the json handler library
namespace fs = std::filesystem;
using json = nlohmann::json;

#include "json.hpp"

namespace settings {

nlohmann::json config;
fs::path settingsFilePath = fs::current_path() / "settings.json";

void init();
nlohmann::json getSettings();

template <typename T>
void updateSettings(const std::string &key, const T &value) {
    try {
        config.at(key) = value;
        if constexpr (std::is_same_v<T, std::string>) {
            logger::log({"SettingsHandler.cpp", "settings::updateSettings"}, key + " updated to " + value);
        } else {
            logger::log({"SettingsHandler.cpp", "settings::updateSettings"}, key + " updated to " + std::to_string(value));
        }
    } catch (const json::out_of_range &e) {
        config[key] = value;
        if constexpr (std::is_same_v<T, std::string>) {
            logger::log({"SettingsHandler.cpp", "settings::updateSettings"}, key + " added with value " + value);
        } else {
            logger::log({"SettingsHandler.cpp", "settings::updateSettings"}, key + " added with value " + std::to_string(value));
        }
    }
    std::ofstream settingsFile(settingsFilePath);
    if (settingsFile.is_open()) {
        settingsFile << config.dump(4);
        logger::log({"SettingsHandler.cpp", "settings::updateSettings"}, "Settings file updated.");
    } else {
        logger::warn({"SettingsHandler.cpp", "settings::updateSettings"}, "Settings file not updated.");
    }
}

}  // namespace settings

#endif