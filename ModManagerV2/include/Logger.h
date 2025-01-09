#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ConsoleStuff.h"
namespace fs = std::filesystem;

namespace logger {

void init();
std::string getCurrentTimeStamp();
void log(const std::vector<std::string> &location, const std::string &text);
void warn(const std::vector<std::string> &location, const std::string &text);
void error(const std::vector<std::string> &location, const std::string &text);

}  // namespace logger

#endif