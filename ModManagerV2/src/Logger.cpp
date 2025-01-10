#include "Logger.h"

fs::path logFilePath = fs::current_path() / "log.txt";

namespace logger {

void init() {
    fs::remove(logFilePath);
    std::ofstream logFile(logFilePath);
    if (logFile.is_open()) {
        logFile << "  LOG (" << getCurrentTimeStamp() << ") [Logger.cpp] [init] Log file created.\n";
    } else {
        colour::cerr("WARNING: Log file not created.\n", "YELLOW");
    }
    logFile.close();
}

std::string getCurrentTimeStamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

std::string escapeString(std::string text) {
    text = std::regex_replace(text, std::regex("\n"), "\\n");
    return text;
}

std::string unescapeString(std::string text) {
    text = std::regex_replace(text, std::regex("\\n"), "\n");
    return text;
}

void log(const std::vector<std::string> &location, const std::string &text) {
    std::ofstream logFile(logFilePath, std::ios::app);
    std::stringstream ss;

    ss << "  LOG (" << getCurrentTimeStamp() << ") ";
    for (const std::string &loc : location) {
        ss << "[" << loc << "] ";
    }
    ss << escapeString(text) << "\n";

    if (logFile.is_open()) {
        logFile << ss.str();
        logFile.close();
    } else {
        std::cerr << "\033[91m" << "ERROR: Log file not created.\n"
                  << ss.str() << "\033[0m" << "Press enter to exit...";
        std::cin.get();
        exit(1);
    }
    logFile.close();
}

void warn(const std::vector<std::string> &location, const std::string &text) {
    std::ofstream logFile(logFilePath, std::ios::app);
    std::stringstream ss;

    ss << " WARN (" << getCurrentTimeStamp() << ") ";
    for (const std::string &loc : location) {
        ss << "[" << loc << "] ";
    }
    ss << escapeString(text) << "\n";

    if (logFile.is_open()) {
        logFile << ss.str();
        logFile.close();
    } else {
        std::cerr << "\033[91m" << "ERROR: Log file not created.\n"
                  << ss.str() << "\033[0m" << "Press enter to exit...";
        std::cin.get();
        exit(1);
    }
    logFile.close();
}

void error(const std::vector<std::string> &location, const std::string &text) {
    std::ofstream logFile(logFilePath, std::ios::app);
    std::stringstream ss;

    ss << "ERROR (" << getCurrentTimeStamp() << ") ";
    for (const std::string &loc : location) {
        ss << "[" << loc << "] ";
    }
    ss << escapeString(text) << "\n";

    if (logFile.is_open()) {
        logFile << ss.str();
        logFile.close();
    } else {
        std::cerr << "\033[91m" << "ERROR: Log file not created.\n"
                  << ss.str() << "\033[0m" << "Press enter to exit...";
        std::cin.get();
        exit(1);
    }
    logFile.close();
}

}  // namespace logger