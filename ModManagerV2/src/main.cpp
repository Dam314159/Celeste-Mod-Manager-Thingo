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

// f : Setup
void setup() {
    logger::init();
    logger::log({"main.cpp", "setup"}, "Successfully initialised logger.");

    settings::init();
    logger::log({"main.cpp", "setup"}, "Successfully initialised settings.");

    // ↳ Check if mods folder path is set correctly
    fs::path modsFolderPath = settings::getSettings()["modsFolderPath"];
    if (!fs::exists(modsFolderPath)) {
        logger::log({"main.cpp", "setup"}, "Mods folder path not found or does not exist.");
        colour::cout("It seems like your mods folder is not specified or does not exist. Please enter your mods folder path below.\n", "DEFAULT");
        colour::cout("You can find the mod folder by going to Olympus -> Manage installed mods -> Open mods folder, then copy and pasting the file path here.\n\n", "DEFAULT");
        colour::cout("If you're still unsure, try \"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Celeste\\Mods\" for Windows.\n", "DEFAULT");
        fs::path modsFolderPath = ask(
            "Enter your mods folder path:",
            [](std::string input) { return fs::exists(input); },
            [](std::string input) { return "Invalid path. Please try again."; });
        settings::updateSettings<std::string>("modsFolderPath", modsFolderPath.string());
    }
    logger::log({"main.cpp", "setup"}, "Mods folder path is " + modsFolderPath.string());

    // ↳ Get all mod names by reading names of zip files
    // ↳ Get dependencies by unzipping mod zips and reading everst.yaml
    // ↳ Load names into custom datatype [A]
    // ↳ Load dependencies into [A]
    std::map<std::string, ModAttribute> modAttributes;
    try {
        for (const fs::path &entry : fs::directory_iterator(modsFolderPath)) {  // For Loop 1
            if (fs::is_regular_file(entry) && entry.extension().string() == ".zip") {
                throw fs::filesystem_error("test", std::make_error_code(std::errc::io_error));
                modAttributes.insert({entry.filename().string(), ModAttribute()});
                logger::log({"main.cpp", "setup", "For Loop 1"}, "Added " + entry.filename().string() + " to modAttributes.");
            }
        }
    } catch (const fs::filesystem_error &e) {
        colour::cerr("An error has occured. Please report this bug on GitHub with the log.txt file attached.\n", "RED");
        logger::error({"main.cpp", "setup", "For Loop 1"}, std::string(e.what()));
        exitOnEnterPress(1, std::string(e.what()));
    }
    // ↳ From favourites.txt load into [A]
    // ↳ Get presets from modpresets
    // ↳ Load into a dict<string, set>[C]
}

// Main Menu
// ↳ Display Text
//     ↳ Welcome message
//     ↳ Options
//     ↳ Current mods folder
//     ↳ Change mods folder option
//     ↳ Prompt user
// ↳ Wait for user input (A)
// ↳ Validate user input
//     ↳ If correct GOTO the corresponding option (<Change Mods Folder> | <Help Centre, main> | <Edit Favourite.txt>)
//     ↳ Else GOTO (A)

// Change Mods Folder
// ↳ Display Text
//     ↳ General info
//     ↳ How to find the correct folder
//     ↳ Prompt user
// ↳ Wait for user input (A)
// ↳ Validate and sanitise user input
//     ↳ If correct
//         ↳ RUN  <Setup>
//         ↳ GOTO <Main Menu>
//     ↳ Else goto(A)

// Help Centre
// substate: main
// ↳ Display general info
// ↳ Prompt user (A)
// ↳ Validate user input
//     ↳ If correct GOTO <Help Centre, [input], [Help Centre, main]>
//     ↳ Else  GOTO (A)

// substate: edit favourites.txt
// ↳ Display Help
// ↳ Wait for enter
// ↳ Return user based on tracker

// substate: edit presets main
// ↳ Display general help and options
// ↳ Prompt user (B)
// ↳ Validate user input
//     ↳ If correct GOTO <Help Centre, [input], [Help Centre, edit presets main]>
//     ↳ If exit Return user based on tracker

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

// Edit Favourite.txt
// ↳ Display general info
// ↳ Display list of mods "[x] modsname"
// ↳ Prompt user (A)
// ↳ Validate user input
//     ↳ If number edit the list
//     ↳ If done
//         ↳ Write to favourites.txt
//         ↳ GOTO <Main Menu>
//     ↳ If help
//         ↳ GOTO <Help Centre, edit favourites.txt, [favourites.txt]>
//     ↳ Else goto (A)

// Edit Presets
// substate: main
// ↳ Display general info
// ↳ If SETTINGS[display presets?]
//     ↳ Display presets
// ↳ Display Options (presetname, remove, help)
// ↳ Prompt user (A)
// ↳ Validate user input
//     ↳ If presetname exists GOTO <Edit Presets, edit>
//     ↳ If presetname not exists GOTO <Edit Presets, add>
//     ↳ If help GOTO <Help centre, edit preset main, [Edit Presets, main]>
//     ↳ if exit, write to modpresets.txt
//     ↳ Else GOTO (A)

// substate: add
// ↳ Display general info
// ↳ If SETTINGS[display presets?]
//     ↳ Display presets
// ↳ Display List of favourites (all )
// ↳ Prompt user (B)
// ↳ Validate user input
//     ↳ If number, toggle
//     ↳ If help, GOTO <Help centre, edit preset add, [Edit Presets, add]>
//     ↳ If exit GOTO <Edit presets, main>

// substate: edit
// ↳ Display general info
// ↳ Display list of favs with presets ticked
// ↳ Prompt user input (C)
// ↳ Validate user input
//     ↳ If number, toggle
//     ↳ If exit, GOTO <Edit Presets, main>
//     ↳ If help, GOTO <Help centre, edit preset edit, [Edit Presets, edit]>

// substate: remove
// ↳ Display general info
// ↳ Display list of presets
// ↳ Prompt user (D)
// ↳ Validate user input
//     ↳ If number, remove preset
//     ↳ If exit, GOTO <Edit Presets, main>
//     ↳ If help, GOTO <Help centre, edit preset remove, [Edit presets, edit]>

// Enable or Disable mods
// ↳ Display general info
// ↳ Display favourites list
// ↳ Prompt user (A)
// ↳ Validate user input
//     ↳ if number, RUN togglemod
//     ↳ if help, GOTO <Help centre, enable or diable mods, [enable or disable mods]>
//     ↳ if exit, write to blacklist.txt

// f : togglemod(modToToggle, itsDependencies, onOrOff)
// ↳ modToToggle -> turn onOrOff
// ↳ if itsDependencies < 1
//     ↳ exit
// ↳ for(dependency in itsDependencies)
//     ↳ RUN togglemod(dependency, dependency[dependencies], onOrOff)

int main() {
    setup();
    return 0;
}