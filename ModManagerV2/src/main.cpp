#include "ConsoleStuff.h"
#include "Logger.h"
#include "miniz.h"  //Credit to richgel999 on GitHub for the miniz library

// Custom datatype to store mod attributes
#include "ModAttribute.h"

// Optional
// ↳ Handle mod options order, updater blacklist

// GLOBAL STATE = [state, substate, tracker]
#include "StateHandler.h"

// SETTINGS = mods folder, display presets?
#include "SettingsHandler.h"

// f : extractDependenciesFromZip
std::set<std::string> extractDependenciesFromZip(const fs::path &zipFilePath) {
    logger::functionCall("extractDependenciesFromZip", {zipFilePath.string()});
    mz_zip_archive zipArchive;
    memset(&zipArchive, 0, sizeof(zipArchive));
    std::set<std::string> dependencies;

    // ↳ Initialize the zip reader
    if (!mz_zip_reader_init_file(&zipArchive, zipFilePath.string().c_str(), 0)) {
        logger::critical("Failed to open ZIP file: " + zipFilePath.string());
        exitOnEnterPress(1, "Failed to open ZIP file: " + zipFilePath.string());
    }
    logger::log("Zip reader initialised");

    int numFiles = mz_zip_reader_get_num_files(&zipArchive);

    // ↳ Loop through all the files in the zip archive
    for (int i = 0; i < numFiles; ++i) {
        mz_zip_archive_file_stat fileStat;

        if (!mz_zip_reader_file_stat(&zipArchive, i, &fileStat)) {
            continue;
        }

        // Check if the file is in the root directory of the zip (not in a subdirectory)
        std::string filename(fileStat.m_filename);
        if (filename.find('/') != std::string::npos || filename.find('\\') != std::string::npos) {
            continue;
        }

        if (filename != "everest.yaml") {
            continue;
        }
        logger::log("Found everest.yaml");

        size_t fileSize = fileStat.m_uncomp_size;
        std::vector<char> buffer(fileSize);
        if (!mz_zip_reader_extract_to_mem(&zipArchive, i, buffer.data(), fileSize, 0)) {
            logger::warn("Failed to read everest.yaml from " + zipFilePath.string());
            continue;
        }

        std::stringstream fileContent(std::string(buffer.begin(), buffer.end()));
        mz_zip_reader_end(&zipArchive);  // Cleanup
        logger::log("Ready to read everest.yaml");

        // For every line in everest.yaml
        std::string line;
        bool isDependency = false;
        while (std::getline(fileContent, line, '\n')) {
            if (line.substr(0, 4) != "    ") {
                isDependency = false;
            }

            if (line.substr(0, 15) == "  Dependencies:") {
                isDependency = true;
                continue;
            }

            if (!isDependency || line.substr(0, 12) != "    - Name: ") {
                continue;
            }

            std::string dependencyName = std::isspace(line[line.size() - 1]) ? line.substr(12, line.size() - 13) : line.substr(12);

            // TODO: Make this some sort of blacklist
            if (!dependencyName.empty() &&
                dependencyName != "Everest" &&
                dependencyName != "EverestCore" &&
                dependencyName != "Celeste" &&
                dependencyName != zipFilePath.stem().string()) {
                dependencies.insert(dependencyName + ".zip");
                logger::log("Added " + dependencyName + ".zip to dependencies.");
            }
        }
    }
    logger::functionExit();
    return dependencies;
}

// f : Setup
std::map<std::string, ModAttribute> mods;
std::map<std::string, std::set<std::string>> modPresets;
fs::path modsFolderPath;
void setup() {
    colour::cout("Loading...", "MAGENTA");

    logger::init();
    logger::functionCall("setup", {});
    logger::log("Successfully initialised logger.");

    settings::init();
    logger::log("Successfully initialised settings.");

    bool warn = false;
    mods = {};
    modPresets = {};

    // ↳ Check if mods folder path is set correctly
    modsFolderPath = settings::getSettings()["modsFolderPath"].get<std::string>();
    if (!fs::exists(modsFolderPath)) {
        logger::log("Mods folder path not found or does not exist.");
        colour::cout("It seems like your mods folder is not specified or does not exist. Please enter your mods folder path below.\n", "DEFAULT");
        colour::cout("You can find the mod folder by going to Olympus -> Manage installed mods -> Open mods folder, then copy and pasting the file path here.\n\n", "DEFAULT");
        colour::cout("If you're still unsure, try \"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Celeste\\Mods\" for Windows.\n", "DEFAULT");
        fs::path modsFolderPath = ask(
            "Enter your mods folder path:",
            [](std::string input) { return fs::exists(input); },
            [](std::string input) { return "Invalid path. Please try again."; });
        settings::updateSettings<std::string>("modsFolderPath", modsFolderPath.string());
    }
    logger::log("Mods folder path is " + modsFolderPath.string());

    // ↳ Get all mod names by reading names of zip files
    // ↳ Get dependencies by unzipping mod zips and reading everst.yaml
    // ↳ Load names into custom datatype [A]
    // ↳ Load dependencies into [A]
    try {
        for (const fs::path &entry : fs::directory_iterator(modsFolderPath)) {
            if (!fs::is_regular_file(entry) || entry.extension().string() != ".zip") {
                continue;
            }

            mods.insert({entry.filename().string(), ModAttribute(false, true, extractDependenciesFromZip(entry))});
            logger::log("Added " + entry.filename().string() + " to mods.");
        }
    } catch (const fs::filesystem_error &e) {
        exitOnEnterPress(1, std::string(e.what()));
    }

    // ↳ From favorites.txt load into [A]
    std::ifstream favoriteTxt(modsFolderPath / "favorites.txt");
    if (favoriteTxt.is_open()) {
        for (std::string line; std::getline(favoriteTxt, line);) {
            if (line[0] == '#' || line.length() < 1) {
                continue;
            }

            mods[line].setIsFavorite(true);
            logger::log("favorited " + line + " from favorites.txt.");
        }
        favoriteTxt.close();
    } else {
        logger::warn("favorites.txt not found.");
        warn = true;
    }

    // ↳ From blacklist.txt load into [A]
    std::ifstream blacklistTxt(modsFolderPath / "blacklist.txt");
    if (blacklistTxt.is_open()) {
        for (std::string line; std::getline(blacklistTxt, line);) {
            if (line[0] == '#' || line.length() < 1) {
                continue;
            }

            mods[line].setIsEnabled(false);
            logger::log("Disabled " + line + " from blacklist.txt.");
        }
        blacklistTxt.close();
    } else {
        logger::warn("blacklist.txt not found.");
        warn = true;
    }

    // ↳ Get presets from modpresets
    // ↳ Load into a dict<string, set>[C]
    std::ifstream modPresetsTxt(modsFolderPath / "modpresets.txt");
    if (modPresetsTxt.is_open()) {
        for (std::string line, currentPreset; std::getline(modPresetsTxt, line);) {
            if (line[0] == '#' || line.length() < 1) {
                continue;
            }
            if (line.substr(0, 2) == "**") {
                currentPreset = line.substr(2);
                logger::log("Added " + currentPreset + " to modPresets.");
                continue;
            }
            modPresets[currentPreset].insert(line);
            logger::log("Added " + line + " to " + currentPreset + " in modPresets.");
        }
    } else {
        logger::warn("modpresets.txt not found");
        warn = true;
    }

    state::setState("mainMenu", warn ? "warn" : "normal", {});
    logger::functionExit();
}

// Main Menu
void MAIN_MENU() {
    logger::functionCall("MAIN_MENU", {});
    cls();

    // ↳ Display Text
    // ↳ Welcome message
    colour::cout("Welcome to the Celeste Mod Manager!\n", "DEFAULT");
    colour::cout("This mod was created by me so that you don't have to deal with the hassle of ", "DEFAULT");
    colour::cout("helper mods and dependencies", "CYAN");
    colour::cout(" sucking up all your RAM.\n\n", "DEFAULT");
    colour::cout("This program only currently works with zip files. Support of folders (unzipped) will come soon. (and by soon, I mean when I feel like it)\n\n", std::array<int, 3>{255, 130, 0});
    logger::log("Welcome message done");

    if (state::getSubState() == "normal") {
        logger::log("normal main menu");
        // ↳ Options
        colour::cout("Options:\n", "DEFAULT");
        colour::cout("1: Enable or Disable mods\n", "DEFAULT");
        colour::cout("2: Edit favorite.txt file\n", "DEFAULT");
        colour::cout("3: Edit presets\n", "DEFAULT");
        colour::cout("4: Help Center\n", "DEFAULT");

        // ↳ Current mods folder
        // ↳ Change mods folder option
        colour::cout("5: Change mods folder (Currently set to: ", "DEFAULT");
        colour::cout(settings::getSettings().at("modsFolderPath").get<std::string>(), "CYAN");
        colour::cout(")\n\n", "DEFAULT");

        // ↳ Exit option
        colour::cout("q: Exit\n\n", "DEFAULT");
        logger::log("Displayed welcome text.");

        // ↳ Prompt user
        auto validateInput = [](std::string input) -> bool {
            try {
                int choice = std::stoi(input);
                return 1 <= choice && choice <= 5;
            } catch (const std::invalid_argument &e) {
                return input == "q";
            }
        };

        auto subsequent = [](std::string input) -> std::string {
            return "Please enter a number from 1 to 5 (inclusive), or \"q\" to exit.";
        };

        // ↳ Wait for user input (A)
        // ↳ Validate user input
        int choiceInt;
        std::string choiceStr = ask("Input the corresponding number to select an option:", validateInput, subsequent);
        try {
            choiceInt = std::stoi(choiceStr);

            // ↳ If correct GOTO the corresponding option (<Change Mods Folder> | <Help Centre, main> | <Edit favorite.txt>)
            // ↳ Else GOTO (A)
            switch (choiceInt) {
                case 1:
                    state::updateState("enableOrDisableMods", "main");
                    break;
                case 2:
                    state::updateState("editFavoritesTxt", "main");
                    break;
                case 3:
                    state::updateState("editPresets", "main");
                    break;
                case 4:
                    state::updateState("helpCentre", "main");
                    break;
                case 5:
                    state::updateState("changeModsFolder", "main");
                    break;
                default:
                    logger::critical("User input incorrectly accepcted not from 0 to 5");
                    exitOnEnterPress(1, "User answer has been incorrectly accepted even though it was out of bounds.");
            }
        } catch (const std::invalid_argument &e) {
            if (choiceStr != "q") {
                logger::critical("User input incorrectly accepted.");
                exitOnEnterPress(1, "User input accepted incorrectly.");
            }
            state::updateState("exit", "");
        }
        logger::functionExit();
        return;
    }

    if (state::getSubState() == "warn") {
        colour::cout("It seems like some important files have not been found. Please change your mods folder or create the needed files.\n", std::array<int, 3>{255, 130, 0});
        colour::cout("(blacklist.txt, favorites.txt, modpresets.txt)\n\n", std::array<int, 3>{255, 130, 0});
        logger::warn("warning main menu");

        colour::cout("Options:\n", "DEFAULT");
        colour::cout("1: Change mods folder (Currently set to: ", "DEFAULT");
        colour::cout(settings::getSettings().at("modsFolderPath").get<std::string>(), "CYAN");
        colour::cout(")\n\n", "DEFAULT");

        colour::cout("q: Exit\n\n", "DEFAULT");

        auto validateInput = [](std::string input) -> bool {
            try {
                int choice = std::stoi(input);
                return choice == 1;
            } catch (const std::invalid_argument &e) {
                return input == "q";
            }
        };

        auto subsequent = [](std::string input) -> std::string {
            return "Please enter 1, or \"q\" to exit.";
        };

        std::string choiceStr = ask("Input the corresponding number to select an option:", validateInput, subsequent);

        if (!validateInput(choiceStr)) {
            logger::critical("User input incorrectly accepted.");
            exitOnEnterPress(1, "User input accepted incorrectly.");
        }

        if (choiceStr == "1") {
            state::updateState("changeModsFolder", "main");
        } else if (choiceStr == "q") {
            state::updateState("exit", "main");
        }
    }
    logger::functionExit();
}

// Change Mods Folder
void CHANGE_MODS_FOLDER() {
    logger::functionCall("CHANGE_MODS_FOLDER", {});
    // ↳ Display Text
    // ↳ General info
    cls();
    colour::cout("This is where you can change the mods folder.\n", "DEFAULT");
    colour::cout("Your mods folder is currently: ", "DEFAULT");
    colour::cout(settings::getSettings().at("modsFolderPath").get<std::string>(), "CYAN");

    // ↳ How to find the correct folder
    colour::cout("\nYou can find your mods folder by opening Olympus -> \"Manage Installed Mods\" -> (near the top) Open Mods Folder -> (In file explorer) Copy file path -> Paste it in the prompt\n\n", "YELLOW");
    colour::cout("Alternatively, you can enter \"q\" to go back to the main menu.", "YELLOW");
    logger::log("General info printed");

    // ↳ Prompt user
    // ↳ Wait for user input (A)
    // ↳ Validate and sanitise user input
    auto validation = [](const std::string &input) -> bool {
        return fs::exists(input) || input == "q";
    };

    auto subsequent = [](const std::string &input) -> std::string {
        return "File path does not exist or cannot be accessed.";
    };

    // ↳ If correct
    std::string filePath = ask("Please paste in the folder path here.", validation, subsequent);
    if (!validation(filePath)) {
        logger::critical("User input incorrectly accepted.");
        exitOnEnterPress(1, "User input accepted incorrectly.");
    }

    if (filePath != "q") {
        // ↳ RUN  <Setup>
        settings::updateSettings("modsFolderPath", filePath);
        setup();
    }

    // ↳ GOTO <Main Menu>
    // ↳ Else goto(A)
    state::returnToPreviousState();
    logger::functionExit();
    return;
}

// Help Centre
// substate: main
// ↳ Display general info
// ↳ Prompt user (A)
// ↳ Validate user input
// ↳ If correct GOTO <Help Centre, [input], [Help Centre, main]>
// ↳ Else  GOTO (A)

// substate: edit favorites.txt
// ↳ Display Help
// ↳ Wait for enter
// ↳ Return user based on tracker

// substate: edit presets main
// ↳ Display general help and options
// ↳ Prompt user (B)
// ↳ Validate user input
// ↳ If correct GOTO <Help Centre, [input], [Help Centre, edit presets main]>
// ↳ If exit Return user based on tracker

// substate: edit presets add
// ↳ Display Help
// ↳ Wait for enter
// ↳ Return user based on tracker

// substate: edit presets edit
// ↳ Display Help
// ↳ Wait for enter
// ↳ Return user based on tracker

// substate: edit presets delete
// ↳ Display Help
// ↳ Wait for enter
// ↳ Return user based on tracker

// Edit favorite.txt
void EDIT_FAVORITES_TXT() {
    logger::functionCall("EDIT_FAVORITES_TXT", {});
    // ↳ Display general info
    cls();
    colour::cout("This is where you can toggle if the mods are in the favourites list.\n\n", "YELLOW");

    // ↳ Display list of mods "[x] modsname"
    std::vector<std::pair<std::string, bool>> allModsFav;
    for (const auto &[key, value] : mods) {
        allModsFav.push_back({key, value.getIsFavorite()});
    }

    printModsList(allModsFav);
    logger::log("Printed favorite mods");

    auto validation = [allModsFav](const std::string &input) -> bool {
        try {
            int choice = std::stoi(input);
            return 1 <= choice && choice <= allModsFav.size();
        } catch (const std::invalid_argument &e) {
            return input == "q" || input == "h";
        }
    };

    auto subsequent = [allModsFav](const std::string &input) -> std::string {
        try {
            int choice = std::stoi(input);
            return "Please enter a number from 1 to " + std::to_string(allModsFav.size());
        } catch (const std::invalid_argument &e) {
            return "Enter a number, \"q\" to go back, or \"h\" for help.";
        }
    };

    // ↳ Prompt user (A)
    // ↳ Validate user input
    std::string choice = ask("\nEnter the number corresponding to the mod you want to toggle, \"q\" to go back, or \"h\" for the help center.", validation, subsequent);
    if (!validation(choice)) {
        logger::critical("User input incorrectly accepted.");
        exitOnEnterPress(1, "User input accepted incorrectly.");
    }
    try {
        // ↳ If number edit the list
        int choiceNum = std::stoi(choice);
        choiceNum--;
        std::string selectedModName = allModsFav[choiceNum].first;
        logger::log(selectedModName);
        logger::log(anyToString(mods.at(selectedModName).getIsFavorite()));
        mods.at(selectedModName).setIsFavorite(!mods.at(selectedModName).getIsFavorite());
        allModsFav[choiceNum].second = !allModsFav[choiceNum].second;
        // ↳ If done
        // ↳ TODO: Write to favorites.txt
        std::stringstream outputFavs;
        outputFavs << "# This is the favorite list. Lines starting with # are ignored.\n\n";
        for (const auto &[modName, isFav] : allModsFav) {
            logger::log(modName + anyToString(isFav));
            if (isFav) {
                outputFavs << modName << "\n";
            }
        }
        fs::remove(modsFolderPath / "favorites.txt");
        std::ofstream favoritesTxt(modsFolderPath / "favorites.txt", std::ios::app);
        favoritesTxt << outputFavs.str();
        favoritesTxt.close();

        logger::functionExit();
        return;
    } catch (const std::invalid_argument &e) {
        if (choice == "q") {
            // ↳ GOTO <Main Menu>
            state::returnToPreviousState();
            logger::functionExit();
            return;
        }

        // ↳ If help
        if (choice == "h") {
            // ↳ GOTO <Help Centre, edit favorites.txt, [favorites.txt]>
            state::updateState("helpCentre", "editFavoritesTxt");
            logger::functionExit();
            return;
        }
    }
    // ↳ Else goto (A)
}

// Edit Presets
// substate: main
// ↳ Display general info
// ↳ If SETTINGS[display presets?]
// ↳ Display presets
// ↳ Display Options (presetname, remove, help)
// ↳ Prompt user (A)
// ↳ Validate user input
// ↳ If presetname exists GOTO <Edit Presets, edit>
// ↳ If presetname not exists GOTO <Edit Presets, add>
// ↳ If help GOTO <Help centre, edit preset main, [Edit Presets, main]>
// ↳ if exit, write to modpresets.txt
// ↳ Else GOTO (A)

// substate: add
// ↳ Display general info
// ↳ If SETTINGS[display presets?]
// ↳ Display presets
// ↳ Display List of favorites (all )
// ↳ Prompt user (B)
// ↳ Validate user input
// ↳ If number, toggle
// ↳ If help, GOTO <Help centre, edit preset add, [Edit Presets, add]>
// ↳ If exit GOTO <Edit presets, main>

// substate: edit
// ↳ Display general info
// ↳ Display list of favs with presets ticked
// ↳ Prompt user input (C)
// ↳ Validate user input
// ↳ If number, toggle
// ↳ If exit, GOTO <Edit Presets, main>
// ↳ If help, GOTO <Help centre, edit preset edit, [Edit Presets, edit]>

// substate: remove
// ↳ Display general info
// ↳ Display list of presets
// ↳ Prompt user (D)
// ↳ Validate user input
// ↳ If number, remove preset
// ↳ If exit, GOTO <Edit Presets, main>
// ↳ If help, GOTO <Help centre, edit preset remove, [Edit presets, edit]>

void turnOnMod(const std::string &modName) {
    logger::functionCall("turnOnMod", {modName});
    mods.at(modName).setIsEnabled(true);
    logger::log("Turned on " + modName);

    if (mods.at(modName).getDependencies().size() < 1) {
        logger::functionExit();
        return;
    }

    for (const auto &dependencyName : mods.at(modName).getDependencies()) {
        turnOnMod(dependencyName);
    }
}

// Enable or Disable mods
void ENABLE_OR_DISABLE_MODS() {
    logger::functionCall("ENABLE_OR_DISABLE_MODS", {});
    std::vector<std::pair<std::string, bool>> favMods = {};
    int i = 1;
    for (const auto &[modName, modAttribute] : mods) {
        if (modAttribute.getIsFavorite()) {
            favMods.push_back({modName, modAttribute.getIsEnabled()});
        }
        i++;
    }
    logger::log("Set favMods list");

    // ↳ Display general info
    cls();
    colour::cout("This is where you can enable or disable mods.\n", "DEFAULT");
    colour::cout("The list shown below is based off the ", "DEFAULT");
    colour::cout("favorites.txt file", "CYAN");
    colour::cout(" that can be found at ", "DEFAULT");
    colour::cout(settings::getSettings().at("modsFolderPath").get<std::string>() + " favorites.txt", "CYAN");
    colour::cout("\n\n", "DEFAULT");

    // ↳ Display favorites list
    printModsList(favMods);
    logger::log("Displayed text");

    int numOfMods = 0;
    for (const auto &[key, value] : mods) {
        if (value.getIsEnabled()) {
            numOfMods++;
        }
    }
    logger::log("Counted enabled mods");

    colour::cout("\nYou currently have ", "DEFAULT");
    colour::cout(std::to_string(numOfMods), "CYAN");
    colour::cout(" mods enabled.\n", "DEFAULT");

    auto validation = [favMods](const std::string &input) -> bool {
        try {
            int choice = std::stoi(input);
            return (1 <= choice && choice <= favMods.size());

        } catch (const std::invalid_argument &e) {
            return (input == "q" || input == "h");
        }
    };

    auto subsequent = [favMods](const std::string &input) -> std::string {
        try {
            int choice = std::stoi(input);
            return "Please enter a number from 1 to " + std::to_string(favMods.size());
        } catch (const std::invalid_argument &e) {
            return "Enter a number, \"q\" to go back, or \"h\" for help.";
        }
    };

    // ↳ Prompt user (A)
    // ↳ Validate user input
    std::string choice = ask("Enter the number corresponding to the mod you want to toggle, \"q\" to go back, or \"h\" for the help center.", validation, subsequent);
    if (!validation(choice)) {
        logger::critical("User input incorrectly accepted.");
        exitOnEnterPress(1, "User input accepted incorrectly.");
    }
    try {
        // ↳ if number, RUN togglemod
        int chosenMod = std::stoi(choice) - 1;
        favMods[chosenMod].second = !favMods[chosenMod].second;

        // Disable all mods
        for (auto &[key, value] : mods) {
            value.setIsEnabled(false);
        }

        // For favourite mod installed, enable all mods and dependencies
        for (const auto &favMod : favMods) {
            if (!favMod.second) {
                continue;
            }

            turnOnMod(favMod.first);
        }

    } catch (const std::invalid_argument &e) {
        if (choice == "q") {
            // ↳ TODO: if exit, write to blacklist.txt
            state::returnToPreviousState();
            logger::log("Exit");
            logger::functionExit();
            return;

        } else if (choice == "h") {
            // ↳ if help, GOTO <Help centre, enable or diable mods, [enable or disable mods]>
            state::updateState("helpCenter", "enableOrDisableMods");
            logger::log("Go to help center");
            logger::functionExit();
            return;
        }
    }
}

int main() {
    setup();
    while (true) {
        if (state::getState() == "mainMenu") {
            MAIN_MENU();
        } else if (state::getState() == "enableOrDisableMods") {
            ENABLE_OR_DISABLE_MODS();
        } else if (state::getState() == "editFavoritesTxt") {
            EDIT_FAVORITES_TXT();
        } else if (state::getState() == "changeModsFolder") {
            CHANGE_MODS_FOLDER();
        } else if (state::getState() == "exit") {
            break;
        } else {
            logger::error("An invalid state, " + state::getState() + ", was found.");
            exitOnEnterPress(1, "An invalid state has been found.");
        }
    }
    logger::log("Goodbye!");
    logger::functionExit();
    return 0;
}