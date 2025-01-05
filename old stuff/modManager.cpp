#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "miniz.h"  //Credit to richgel999 on GitHub for the miniz library

// #ifndef DEBUG
// #define DEBUG
// #endif

namespace fs = std::filesystem;

// List of functions
void exitOnEnterPress(const int &exitCode);
std::string trimFirstAndLast(const std::string &input);
void print(const std::vector<std::string> &input);
void print(const std::set<std::string> &input);
void print(const std::unordered_map<std::string, std::set<std::string>> &input);
void print(const std::unordered_map<std::string, bool> &input);
void print(const std::map<std::string, bool> &input);
std::string trim(const std::string &str, const std::string &trimDirection);
std::vector<std::string> splitString(const std::string &str, char delimiter);
std::vector<std::string> extractEverestYamlFromZip(const std::string &zipFileName);
std::set<std::string> extractDependenciesFromEverstVector(const std::vector<std::string> &yaml, const std::string &currentMod);
int countEnabledMods(const std::map<std::string, bool> &enabledModsList);
void cerrColour(const std::string &output, const std::string &colourCode);
void coutColour(const std::string &output, const std::string &colourCode);
void clear();
std::size_t countDigits(std::size_t num);
std::string leftPad(const std::string &input, const std::size_t &size, const char &paddingChar);
std::size_t promptUser(const std::string &question, std::size_t max);
std::map<std::string, bool> disbableAllMods(const std::map<std::string, bool> enabledModsList);
std::map<std::string, bool> enableMod(const std::map<std::string, bool> enabledModsList, std::unordered_map<std::string, std::set<std::string>> modDependencies, std::string modToEnable);
std::map<std::string, bool> updateEnabledFavMods(const std::map<std::string, bool> &enabledFavMods, const std::map<std::string, bool> &enabledModsList);
void updateFile(std::string modsFolder, std::string filename, std::map<std::string, bool> enabledModsList);

// Colour Codes
std::unordered_map<std::string, std::string> colourCodes = {
    {"RED", "\033[91m"},
    {"GREEN", "\033[92m"},
    {"YELLOW", "\033[93m"},
    {"BLUE", "\033[94m"},
    {"MAGENTA", "\033[95m"},
    {"CYAN", "\033[96m"},
    {"WHITE", "\033[37m"},
    {"GREY", "\033[90m"}};

int main() {
#ifdef DEBUG
    for (const auto &[key, value] : colourCodes) {
        coutColour("This is " + key + ".\n", key);
    }
#endif

    std::cout << "Welcome to the celeste mod manager thingo!\n"
              << "Here you can choose to enable or disable mods based on your favourites.txt file\n"
              << "This thing was made to make sure the least number of mods are enabled and also disabling a mod doesn't disable the dependencies.\n"
              << "So it make sure the least number of mods are enabled to prevent running out of RAM or something.\n"
              << '\n';

    std::string modsFolderString = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Celeste\\Mods";
    fs::path modsFolderPath(modsFolderString);

    // Open blacklist.txt and load into enabledMods vector
    std::map<std::string, bool> enabledModsList;
    std::ifstream mods;
    mods.open(modsFolderPath.string() + "\\blacklist.txt");
    if (!mods) {
        cerrColour("Unable to open: " + trimFirstAndLast(modsFolderPath.string()) + "\\blacklist.txt\n", "RED");
        cerrColour("Check if the file exists, or isn't open in another program.\n", "RED");
        exitOnEnterPress(1);
    }
    std::string line;
    while (std::getline(mods, line)) {
        if (line.length() < 4 || !line.ends_with(".zip")) {
            continue;
        }

        if (line[0] == '#') {
            enabledModsList.insert({line.substr(2), true});
        } else {
            enabledModsList.insert({line, false});
        }
    }
    mods.close();

    // Open favourite.txt and load into enabledFavMods vector
    std::map<std::string, bool> enabledFavMods;
    std::vector<std::string> favModsList;
    std::ifstream favs;
    favs.open(modsFolderPath.string() + "\\favorites.txt");
    if (!favs) {
        cerrColour("Unable to open: " + trimFirstAndLast(modsFolderPath.string()) + "\\blacklist.txt\n", "RED");
        cerrColour("Check if the file exists, or isn't open in another program.\n", "RED");
        exitOnEnterPress(1);
    }
    while (std::getline(favs, line))
        if (line[0] != '#' && line.length() > 0) {
            enabledFavMods.insert({line, enabledModsList.at(line)});
            favModsList.push_back(line);
        }
    favs.close();
    std::sort(favModsList.begin(), favModsList.end());

    // Loop over all mods, extract dependencies to modDependencies map
    std::unordered_map<std::string, std::set<std::string>> modDependencies;

    try {
        for (const fs::path &entry : fs::directory_iterator(modsFolderPath)) {  // directory_iterator returns a list of files/folders, const make sure the files dont get changed, & makes it more efficient (pointer instead of copy)
            if (fs::is_regular_file(entry) && entry.extension().string() == ".zip") {
                modDependencies.insert({entry.filename().string(), extractDependenciesFromEverstVector(extractEverestYamlFromZip(entry.string()), entry.stem().string())});
            }
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    /*########################################################################################################################################################################################*/

    // Display
    std::size_t modToToggle;
    while (true) {
        clear();
        std::cout << "Enter the number corressponding to the mod you want to toggle.\n"
                  << "Enter \"help\" for help.\n"
                  << "Enter \"exit\" to exit.\n\n";
        print(enabledFavMods);
        std::cout << "You currently have " << countEnabledMods(enabledModsList) << " mod(s) enabled.\n";

        try {
            modToToggle = promptUser(">", enabledFavMods.size());
        } catch (const std::runtime_error &e) {
            std::string cmd = e.what();
            if (cmd == "help") {
                coutColour("No help for you lol.\n", "RED");
                std::cout << "Press enter to continue.";
                std::cin.get();
                continue;
            }
            break;
        }
        std::cout << "Loading...";
        enabledFavMods.at(favModsList[modToToggle]) = !enabledFavMods.at(favModsList[modToToggle]);
        enabledModsList = disbableAllMods(enabledModsList);
        for (const std::string &favMod : favModsList) {
            if (enabledFavMods.at(favMod)) {
#ifdef DEBUG
                std::cout << enabledFavMods.at(favModsList[modToToggle]) << '\n';
                coutColour("Loading " + favMod + "...\n", "RED");
#endif

                enabledModsList = enableMod(enabledModsList, modDependencies, favMod);
#ifdef DEBUG

                coutColour("Done loading " + favMod + "!\n", "GREEN");
                std::cout << "Press enter to continue.";
                std::cin.get();
#endif
            }
        }
        enabledFavMods = updateEnabledFavMods(enabledFavMods, enabledModsList);
    }

    // TODO: edit blacklist.txt
    coutColour("You are exiting.\n", "MAGENTA");
    updateFile(modsFolderPath.string(), "blacklist.txt", enabledModsList);
    coutColour("You are exited.\n", "MAGENTA");
    exitOnEnterPress(0);
    // Uh what this is illegal you are not supposed to be here
    return 1;
}

void exitOnEnterPress(const int &exitCode) {
    std::cout << "Press enter to exit.";
    std::cin.get();
    exit(exitCode);
}

std::string trimFirstAndLast(const std::string &input) {
    if (input.length() <= 2) {
        return "";
    }

    return input.substr(1, input.length() - 2);
}

void print(const std::vector<std::string> &input) {
    std::size_t i = 0;  // use std::size_t for indexing Credit to chatGPT
    for (const std::string &entry : input) {
        std::cout << i << ':' << entry << '\n';
        i++;
    }
}

void print(const std::set<std::string> &input) {
    std::size_t i = 0;  // use std::size_t for indexing Credit to chatGPT
    for (const std::string &entry : input) {
        std::cout << i << ':' << entry << '\n';
        i++;
    }
}

// Credit to chatGPT, edited by me
// Function to print an unordered_map<std::string, std::set<std::string>> in a nice format
void print(const std::unordered_map<std::string, std::set<std::string>> &input) {
    for (const auto &[key, valueSet] : input) {
        // Print the key
        std::cout << key << ":\n";

        // Print the set of values indented
        for (const auto &value : valueSet) {
            std::cout << "  - " << value << '\n';
        }
    }
}

void print(const std::unordered_map<std::string, bool> &input) {
    for (const auto &[key, value] : input) {
        std::cout << key << ": " << value << '\n';
    }
}

void print(const std::map<std::string, bool> &input) {
    std::size_t digitCount = countDigits(input.size());
    std::size_t i = 0;
    for (const auto &[key, value] : input) {
        if (value) {
            coutColour(leftPad(std::to_string(i), digitCount, ' ') + ": [x] " + key + '\n', "WHITE");
        } else {
            coutColour(leftPad(std::to_string(i), digitCount, ' ') + ": [ ] " + key + '\n', "GREY");
        }
        i++;
    }
}

// Credit to chatGPT, modified my me
std::string trim(const std::string &str, const std::string &trimDirection) {
    if (str.empty())
        return str;
    if (trimDirection == "Leading") {
        // Trim from the front
        size_t start = 0;
        while (start < str.length() && std::isspace(str[start])) {
            ++start;
        }
        return str.substr(start);
    }
    if (trimDirection == "Trailing") {
        // Trim from the back
        size_t end = str.length();
        while (end > 0 && std::isspace(str[end - 1])) {
            --end;
        }
        return str.substr(0, end);
    }
    if (trimDirection == "Both") {
        return trim(trim(str, "Leading"), "Trailing");
    }
    return "";
}

std::vector<std::string> splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);  // Convert to std::stringstream to use std::getline()
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        if (token.length() > 0) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// Credit to chatGPT for this function
// Function to extract the content of "everest.yaml" from a ZIP file
std::vector<std::string> extractEverestYamlFromZip(const std::string &zipFileName) {
    mz_zip_archive zipArchive;
    memset(&zipArchive, 0, sizeof(zipArchive));

    // Initialize the zip reader
    if (!mz_zip_reader_init_file(&zipArchive, zipFileName.c_str(), 0)) {
        std::cerr << "Failed to open ZIP file: " << zipFileName << std::endl;
        return {"Failed to open ZIP file", zipFileName};  // Return an empty vector
    }

    int numFiles = mz_zip_reader_get_num_files(&zipArchive);

    // Loop through all files in the ZIP archive
    for (int i = 0; i < numFiles; ++i) {
        mz_zip_archive_file_stat fileStat;
        if (mz_zip_reader_file_stat(&zipArchive, i, &fileStat)) {
            // Check if the file is "everest.yaml"
            if (std::string(fileStat.m_filename) == "everest.yaml") {
                size_t fileSize = fileStat.m_uncomp_size;
                std::vector<char> buffer(fileSize);

                // Extract "everest.yaml" to memory
                if (mz_zip_reader_extract_to_mem(&zipArchive, i, buffer.data(), fileSize, 0)) {
                    std::string fileContent(buffer.begin(), buffer.end());
                    mz_zip_reader_end(&zipArchive);  // Cleanup before returning
                    return splitString(fileContent, '\n');
                } else {
                    std::cerr << "Failed to read everest.yaml from " << zipFileName << std::endl;
                }
                break;
            }
        }
    }

    mz_zip_reader_end(&zipArchive);              // Cleanup
    return {"Zip file not found", zipFileName};  // Return an empty vector if the file is not found
}

std::set<std::string> extractDependenciesFromEverstVector(const std::vector<std::string> &yaml, const std::string &currentMod) {
    std::set<std::string> output = {};
    bool isDependency = false;
    for (const std::string &entry : yaml) {
        // Set the isDependency flag
        if (entry.substr(0, 4) != "    ") {
            isDependency = false;
        }
        if (entry.substr(0, 15) == "  Dependencies:") {
            isDependency = true;
            continue;
        }

        if (isDependency && entry.substr(0, 12) == "    - Name: ") {
            // TODO: Change dependency check into a blocklist
            std::string dependencyName = trim(entry.substr(12), "Both");
            if (!dependencyName.empty() &&
                dependencyName != "Everest" &&
                dependencyName != "EverestCore" &&
                dependencyName != "Celeste" &&
                dependencyName != currentMod) {
                output.insert(dependencyName + ".zip");
            }
        }
    }
    return output;
}

int countEnabledMods(const std::map<std::string, bool> &enabledModsList) {
    int enabled = 0;
    for (const auto &[key, value] : enabledModsList) {
        enabled += value;
    }
    return enabled;
}

void cerrColour(const std::string &string, const std::string &colourCode) {
    std::cerr << colourCodes.at(colourCode) << string << "\033[0m";
}

void coutColour(const std::string &string, const std::string &colourCode) {
    std::cout << colourCodes.at(colourCode) << string << "\033[0m";
}

// TODO: make the clear work on windows
void clear() {
    std::cout << "\033[3J\033[2J\033[H";
}

// Credit to chatGPT
std::size_t countDigits(std::size_t num) {
    return (num == 0) ? 1 : static_cast<std::size_t>(std::log10(num)) + 1;
}

std::string leftPad(const std::string &input, const std::size_t &size, const char &paddingChar) {
    std::string output = input;
    for (int i = input.length(); i < size; i++) {
        output += paddingChar;
    }
    return output;
}

std::size_t promptUser(const std::string &question, std::size_t max) {
    bool isAnswered = false;
    std::size_t output;
    std::string inputString;
    while (!isAnswered) {
        std::cout << question;
        std::getline(std::cin, inputString);

        try {
            // Convert the string to a size_t
            output = std::stoull(inputString);  // stoull: string to unsigned long long
        } catch (const std::invalid_argument &e) {
            if (inputString == "help" || inputString == "exit") {
                throw std::runtime_error(inputString);
            }
            cerrColour("Invalid input. Please enter a number.\n", "RED");
            continue;
        } catch (const std::out_of_range &e) {
            cerrColour("Enter a number more than 0 or less than " + std::to_string(max + 1) + '\n', "RED");
            continue;
        }
        if (output > max) {
            cerrColour("Enter a number more than 0 or less than " + std::to_string(max + 1) + '\n', "RED");
        } else {
            isAnswered = true;
        }
    }

    return output;
}

std::map<std::string, bool> disbableAllMods(const std::map<std::string, bool> enabledModsList) {
    std::map<std::string, bool> output;
    for (const auto &[key, value] : enabledModsList) {
#ifdef DEBUG
        coutColour("Disabling: " + key + "...\n", "RED");
#endif
        output.insert({key, false});
    }
    return output;
}

// includes dependencies
std::map<std::string, bool> enableMod(const std::map<std::string, bool> enabledModsList, std::unordered_map<std::string, std::set<std::string>> modDependencies, std::string modToEnable) {
    std::map<std::string, bool> output;
    for (const auto &[key, value] : enabledModsList) {
        output.insert({key, value});
    }
#ifdef DEBUG
    coutColour("  " + modToEnable + " -> " + modToEnable + ": loading...\n", "RED");
#endif
    try {
        output.at(modToEnable) = true;
    } catch (const std::out_of_range &e) {
        cerrColour("Whoopsies! It seems like " + modToEnable + " does not exist!\n", "RED");
        cerrColour("You can install any missing dependencies by launching Everest -> Mod Options -> Install missing dependencies.\n", "RED");
        std::cout << "Press enter to continue.";
        std::cin.get();
    }
#ifdef DEBUG
    coutColour("  " + modToEnable + " -> " + modToEnable + ": Done!\n", "GREEN");
#endif
    if (modDependencies.at(modToEnable).size() > 0) {
        for (const std::string &dependency : modDependencies.at(modToEnable)) {
            output = enableMod(output, modDependencies, dependency);
        }
    }
    return output;
}

std::map<std::string, bool> updateEnabledFavMods(const std::map<std::string, bool> &enabledFavMods, const std::map<std::string, bool> &enabledModsList) {
    std::map<std::string, bool> output;
    for (const auto &[key, value] : enabledFavMods) {
        output.insert({key, enabledModsList.at(key)});
    }
    return output;
}

void updateFile(std::string modsFolder, std::string filename, std::map<std::string, bool> enabledModsList) {
    std::string blacklistPath = modsFolder + "\\" + filename;
    std::string tempFilePath = modsFolder + "\\temp.txt";
    std::ifstream blacklist(blacklistPath);
    std::ofstream tempFile(tempFilePath);

    if (!blacklist) {
        cerrColour("Error opening file " + blacklistPath + '\n', "RED");
        exitOnEnterPress(0);
    }
    if (!tempFile) {
        cerrColour("Error opening file " + modsFolder + "\\temp.txt\n", "RED");
        exitOnEnterPress(0);
    }
    std::string line;
    while (std::getline(blacklist, line)) {
        if (line.length() < 4 || !line.ends_with(".zip")) {
            tempFile << line << '\n';
            continue;
        }
        if (line[0] == '#') {
            line = line.substr(2);
        }
        if (enabledModsList.at(line)) {
            line = "# " + line;
        }
        tempFile << line << '\n';
    }
    blacklist.close();
    tempFile.close();
    std::remove((blacklistPath).c_str());
    std::rename((tempFilePath).c_str(), (blacklistPath).c_str());
}
