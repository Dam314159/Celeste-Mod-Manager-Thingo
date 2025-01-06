#include "Logger.h"

#include <filesystem>
namespace fs = std::filesystem;
#include <fstream>
#include <iostream>

#include "ConsoleStuff.h"

fs::path filePath = fs::current_path() / "log.txt";

namespace logger {

void init() {
    fs::remove(filePath);
    std::ofstream logFile(filePath);
    if (logFile.is_open()) {
        logFile << "Log file created\n";
    } else {
        colour::cerr("WARNING: Log file not created.\n", "YELLOW");
    }
    logFile.close();
}

void log(std::string location, std::string text) {
    std::ofstream logFile(filePath, std::ios::app);
    logFile << "From \"" << location << "\": \"" << text << "\"\n";
}

}  // namespace logger