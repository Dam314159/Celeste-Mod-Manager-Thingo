#include "ConsoleStuff.h"

#include <cstdlib>
#include <iostream>

#include "Logger.h"

namespace colour {

const std::unordered_map<std::string, std::string> colourCodes = {
    {"RED", "\033[91m"},
    {"GREEN", "\033[92m"},
    {"YELLOW", "\033[93m"},
    {"BLUE", "\033[94m"},
    {"MAGENTA", "\033[95m"},
    {"CYAN", "\033[96m"},
    {"WHITE", "\033[37m"},
    {"GREY", "\033[90m"}};

void cout(const std::string &text, const std::string &colour) {
    std::cout << colourCodes.at(colour) << text << "\033[0m";
    logger::log("ConsoleStuff.cpp, cout", "Printed \"" + text + "\" in " + colour + " colour.");
}

void cerr(const std::string &text, const std::string &colour) {
    std::cerr << colourCodes.at(colour) << text << "\033[0m";
    logger::log("ConsoleStuff.cpp, cerr", "Printed \"" + text + "\" in " + colour + " colour.");
}

}  // namespace colour

void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    logger::log("ConsoleStuff.cpp, cls", "Cleared console screen.");

}  // namespace console