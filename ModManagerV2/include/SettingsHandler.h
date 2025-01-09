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

void init();
nlohmann::json getSettings();

}  // namespace settings

#endif