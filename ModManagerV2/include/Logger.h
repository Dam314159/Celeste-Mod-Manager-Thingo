#ifndef LOGGER_H
#define LOGGER_H

#include <any>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "ConsoleStuff.h"
namespace fs = std::filesystem;

namespace logger {
void init();
std::string getCurrentTimeStamp();
std::string escapeNewlines(std::string text);
std::string unescapeString(std::string text);
void log(const std::string &text);
void warn(const std::string &text);
void error(const std::string &text);
void critical(const std::string &text);
void functionCall(std::string functionName, std::vector<std::string> arguments);
void functionExit();

}  // namespace logger

#endif