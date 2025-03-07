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

extern nlohmann::json config;
extern fs::path settingsFilePath;

namespace settings {

void init();
nlohmann::json getSettings();

template <typename T>
void updateSettings(const std::string &key, const T &value) {
    logger::functionCall("settings::updateSettings", {key, anyToString(value)});
    logger::log(config.dump(4));
    try {
        config.at(key) = value;
        logger::log(config.dump(4));
        if constexpr (std::is_same_v<T, std::string>) {
            logger::log(key + " updated to " + value);
        } else {
            logger::log(key + " updated to " + std::to_string(value));
        }
    } catch (const json::out_of_range &e) {
        config[key] = value;
        if constexpr (std::is_same_v<T, std::string>) {
            logger::log(key + " added with value " + value);
        } else {
            logger::log(key + " added with value " + std::to_string(value));
        }
    }
    std::ofstream settingsFile(settingsFilePath);
    if (settingsFile.is_open()) {
        settingsFile << config.dump(4);
        logger::log("Settings file updated.");
    } else {
        logger::warn("Settings file not updated.");
    }
    settingsFile.close();
    logger::functionExit();
}

}  // namespace settings

#endif