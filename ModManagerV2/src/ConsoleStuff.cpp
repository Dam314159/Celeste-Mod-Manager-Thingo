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
}

void cout(const std::string &text, const std::array<int, 3> &colour) {
    if (0 <= colour[0] && colour[0] <= 255) {
        logger::warn("R value is out of bounds while printing " + text);
    }
    if (0 <= colour[1] && colour[1] <= 255) {
        logger::warn("G value is out of bounds while printing " + text);
    }
    if (0 <= colour[2] && colour[2] <= 255) {
        logger::warn("B value is out of bounds while printing " + text);
    }

    std::cout << "\u001b[38;2;" << colour[0] << ";" << colour[1] << ";" << colour[2] << "m" << text;
}

void cerr(const std::string &text, const std::string &colour) {
    std::cerr << colourCodes.at(colour) << text;
}

void cerr(const std::string &text, const std::array<int, 3> &colour) {
    if (0 <= colour[0] && colour[0] <= 255) {
        logger::warn("R value is out of bounds while printing " + text);
    }
    if (0 <= colour[1] && colour[1] <= 255) {
        logger::warn("G value is out of bounds while printing " + text);
    }
    if (0 <= colour[2] && colour[2] <= 255) {
        logger::warn("B value is out of bounds while printing " + text);
    }

    std::cerr << "\u001b[38;2;" << colour[0] << ";" << colour[1] << ";" << colour[2] << "m" << text;
}

}  // namespace colour

void cls() {
    // #ifdef _WIN32
    //     std::system("cls");
    // #else
    //     std::system("clear");
    // #endif
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}

void exitOnEnterPress(const int &exitCode, const std::string &message) {
    if (exitCode == 0) {
        colour::cout("Press enter to exit...", "DEFAULT");
        std::cin.get();
        logger::log("Exited with code 0. " + message);
    } else {
        logger::critical("Exited with code " + std::to_string(exitCode) + ". " + message);
        colour::cout("Press enter to exit...", "RED");
        std::cin.get();
    }
    exit(exitCode);
}

std::string ask(const std::string &question,
                const std::function<bool(std::string)> &validation,
                const std::function<std::string(std::string)> &subsequent) {
    logger::functionCall("ask", {question, anyToString(validation), anyToString(subsequent)});

    std::string input;
    bool isValid = false;
    logger::log("Asked \"" + question + "\".");

    colour::cout(question, "YELLOW");
    colour::cout("\n>", "DEFAULT");
    std::getline(std::cin, input);
    isValid = validation(input);
    logger::log("Received \"" + input + "\", " + (isValid ? "true" : "false") + ".");

    while (!isValid) {
        colour::cout(subsequent(input), "RED");
        colour::cout("\n>", "DEFAULT");
        logger::log("Re-asked \"" + question + "\".");
        std::getline(std::cin, input);
        isValid = validation(input);
        logger::log("Received \"" + input + "\", " + (isValid ? "true" : "false") + ".");
    }

    logger::functionExit();
    return input;
}

void printModsList(std::vector<std::pair<std::string, bool>> modsList) {
    int modsListLen = modsList.size();
    int modsListLenDigitCount = (modsListLen == 0) ? 1 : std::log10(modsListLen) + 1;

    int i = 1;
    for (const auto &[key, value] : modsList) {
        std::string index = std::to_string(i);
        while (index.size() < modsListLenDigitCount) {
            index = " " + index;
        }
        if (value) {
            colour::cout("[x] " + index + ": " + key + "\n", "WHITE");
        } else {
            colour::cout("[ ] " + index + ": " + key + "\n", "GREY");
        }
        i++;
    }
    logger::functionExit();
}

std::string anyToString(const std::any &value) {
    if (value.type() == typeid(bool)) return (std::any_cast<bool>(value) ? "true" : "false");
    if (value.type() == typeid(std::function<bool(std::string)>)) return "bool function(std::string)";
    if (value.type() == typeid(std::function<std::string(std::string)>)) return "std::string function(std::string)";

    if (value.type() == typeid(std::set<std::string>)) {
        std::set<std::string> castedValue = std::any_cast<std::set<std::string>>(value);
        std::stringstream ss;
        ss << "[";
        for (auto it = castedValue.begin(); it != castedValue.end(); ++it) {
            ss << *it;
            if (std::next(it) != castedValue.end()) {
                ss << ", ";
            }
        }
        ss << "]";
        return ss.str();
    }

    if (value.type() == typeid(std::stack<std::pair<std::string, std::string>>)) {
        std::stack<std::pair<std::string, std::string>> castedValue = std::any_cast<std::stack<std::pair<std::string, std::string>>>(value);
        std::stringstream ss;
        ss << "[";

        std::stack<std::pair<std::string, std::string>> temp;
        while (!castedValue.empty()) {
            temp.push(castedValue.top());
            castedValue.pop();
        }

        while (!temp.empty()) {
            ss << "[" << temp.top().first << ", " << temp.top().second << "]";
            if (temp.size() > 1) {
                ss << ", ";
            }
        }
        ss << "]";

        return ss.str();
    }

    return std::any_cast<std::string>(value);
}