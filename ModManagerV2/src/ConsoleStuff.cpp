#include "ConsoleStuff.h"

#include <cstdlib>
#include <iostream>

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

void cout(std::string text, std::string colour) {
    std::cout << colourCodes.at(colour) << text << "\033[0m";
}

void cerr(std::string text, std::string colour) {
    std::cerr << colourCodes.at(colour) << text << "\033[0m";
}

}  // namespace colour

void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

}  // namespace console