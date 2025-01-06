#include "Logger.h"

#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include <iostream>

#include "ConsoleStuff.h"

fs::path logFilePath = fs::current_path() / "log.txt";

namespace logger {

void init() {
    fs::remove(logFilePath);
    std::ofstream logFile(logFilePath);
    if (logFile.is_open()) {
        logFile << "[Logger.cpp, init()]: \"Log file created.\"\n";
    } else {
        colour::cerr("WARNING: Log file not created.\n", "YELLOW");
    }
    logFile.close();
}

void log(const std::string &location, const std::string &text) {
    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "[" << location << "]: \"" << text << "\"\n";
}

}  // namespace logger