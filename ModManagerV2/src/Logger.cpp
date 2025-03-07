#include "Logger.h"

// TODO: Revamp the logging methods

fs::path logFilePath = fs::current_path() / "log.txt";
auto start = std::chrono::system_clock::now();
std::vector<std::string> callStack = {"main"};
bool fileExists = false;

namespace logger {

void init() {
    fs::remove(logFilePath);
    std::ofstream logFile(logFilePath);
    if (logFile.is_open()) {
        fileExists = true;
        logFile << "================================================== LOG FILE CREATED " << getCurrentTimeStamp() << "ms SINCE EXECUTION START ==================================================\n";
        logFile << "Each line is in the same format as shown below:\n";
        logFile << "[LOG/WARN/ERROR] [Time since program start] [Function Call Stack] text\n";
        logFile << "==================================================================================================================================================\n";
    } else {
        colour::cerr("WARNING: Log file not created.\n", "YELLOW");
    }
    logFile.close();
}

std::string getCurrentTimeStamp() {
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch() - start.time_since_epoch();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
    return std::to_string(timestamp);
}

std::string escapeNewlines(std::string text) {
    text = std::regex_replace(text, std::regex("\n"), "\\n");
    return text;
}

std::string callStackString() {
    std::stringstream callStackString;
    callStackString << "[";
    for (size_t i = 0; i < callStack.size(); i++) {
        callStackString << callStack[i];
        if (i < callStack.size() - 1) {
            callStackString << " -> ";
        }
    }
    callStackString << "]";
    return callStackString.str();
}

void log(const std::string &text) {
    if (!fileExists) {
        return;
    }

    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "[LOG]   ";
    logFile << "[" << getCurrentTimeStamp() << "ms] ";
    logFile << callStackString() << ": ";
    logFile << escapeNewlines(text) << "\n";
    logFile.close();
}

void warn(const std::string &text) {
    if (!fileExists) {
        return;
    }

    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "[WARN]  ";
    logFile << "[" << getCurrentTimeStamp() << "ms] ";
    logFile << callStackString() << ": ";
    logFile << escapeNewlines(text) << "\n";
    logFile.close();
}

void error(const std::string &text) {
    if (!fileExists) {
        return;
    }

    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "[ERROR] ";
    logFile << "[" << getCurrentTimeStamp() << "ms] ";
    logFile << callStackString() << ": ";
    logFile << escapeNewlines(text) << "\n";
    logFile.close();
}

void critical(const std::string &text) {
    if (!fileExists) {
        return;
    }

    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "====================================================================[CRITICAL]====================================================================\n";
    logFile << "[" << getCurrentTimeStamp() << "ms] ";
    logFile << callStackString() << ": ";
    logFile << escapeNewlines(text) << "\n";
    logFile.close();
}

void functionCall(std::string functionName, std::vector<std::string> arguments) {
    if (!fileExists) {
        return;
    }

    callStack.push_back(functionName);

    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "[FUNC]  ";
    logFile << "[" << getCurrentTimeStamp() << "ms] ";
    logFile << callStackString() << ": ";
    logFile << "Called " << functionName;

    if (!arguments.empty()) {
        logFile << " with arguements {";
        for (size_t i = 0; i < arguments.size(); i++) {
            logFile << arguments[i];
            if (i < arguments.size() - 1) {
                logFile << ", ";
            }
        }
        logFile << "}";
    }
    logFile << "\n";
    logFile.close();
}

void functionExit() {
    if (!fileExists) {
        return;
    }

    std::string extiedFunction = callStack.back();
    callStack.pop_back();

    std::ofstream logFile(logFilePath, std::ios::app);
    logFile << "[FUNC]  ";
    logFile << "[" << getCurrentTimeStamp() << "ms] ";
    logFile << callStackString() << ": ";
    logFile << "Exited " << extiedFunction << ".\n";
    logFile.close();
}

}  // namespace logger