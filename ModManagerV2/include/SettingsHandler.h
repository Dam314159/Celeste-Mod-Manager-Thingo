#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <string>

#include "json.hpp"

namespace settings {

void init();

nlohmann::json getSettings();

}  // namespace settings

#endif