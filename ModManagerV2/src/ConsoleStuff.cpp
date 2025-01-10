#include "ConsoleStuff.h"

namespace colour {

const std::unordered_map<std::string, std::string> colourCodes = {
    {"RED", "\033[91m"},
    {"GREEN", "\033[92m"},
    {"YELLOW", "\033[93m"},
    {"BLUE", "\033[94m"},
    {"MAGENTA", "\033[95m"},
    {"CYAN", "\033[96m"},
    {"WHITE", "\033[37m"},
    {"GREY", "\033[90m"},
    {"DEFAULT", "\033[0m"}};

void cout(const std::string &text, const std::string &colour) {
    std::cout << colourCodes.at(colour) << text;
    // logger::log({"ConsoleStuff.cpp", "colour::cout"}, "Printed \"" + text + "\" in " + colour + " colour.");
}

void cerr(const std::string &text, const std::string &colour) {
    std::cerr << colourCodes.at(colour) << text;
    // logger::log({"ConsoleStuff.cpp", "colour::cerr"}, "Printed \"" + text + "\" in " + colour + " colour.");
}

}  // namespace colour

void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    // logger::log({"ConsoleStuff.cpp", "cls"}, "Cleared console screen.");
}

void exitOnEnterPress(const int &exitCode, const std::string &message) {
    colour::cout("Press enter to exit...", "DEFAULT");
    std::cin.get();
    if (exitCode == 0) {
        logger::log({"ConsoleStuff.cpp", "exitOnEnterPress"}, "Exited with code 0. " + message);
    } else {
        logger::error({"ConsoleStuff.cpp", "exitOnEnterPress"}, "Exited with code " + std::to_string(exitCode) + ". " + message);
    }
    exit(exitCode);
}

std::string ask(const std::string &question,
                const std::function<bool(std::string)> &validation,
                const std::function<std::string(std::string)> &subsequent) {
    std::string input;
    bool isValid = false;
    logger::log({"ConsoleStuff.cpp", "ask"}, "Asked \"" + question + "\".");

    colour::cout(question, "YELLOW");
    colour::cout("\n>", "DEFAULT");
    std::getline(std::cin, input);
    isValid = validation(input);
    logger::log({"ConsoleStuff.cpp", "ask"}, "Received \"" + input + "\", " + (isValid ? "true" : "false") + ".");

    while (!isValid) {
        colour::cout(subsequent(input), "RED");
        colour::cout("\n>", "DEFAULT");
        logger::log({"ConsoleStuff.cpp", "ask"}, "Re-asked \"" + question + "\".");
        std::getline(std::cin, input);
        isValid = validation(input);
        logger::log({"ConsoleStuff.cpp", "ask"}, "Received \"" + input + "\", " + (isValid ? "true" : "false") + ".");
    }

    return input;
}
