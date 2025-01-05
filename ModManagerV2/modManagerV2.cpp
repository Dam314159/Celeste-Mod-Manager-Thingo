#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "miniz.h"  //Credit to richgel999 on GitHub for the miniz library

// Custom datatype to store mod attributes
#include "ModAttribute.h"

// Optional
// ↳ Handle mod options order, updater blacklist

// GLOBAL STATE = [state, substate, tracker]
// SETTINGS = mods folder, display presets?

// f : Setup
// ↳ Get all mod names by reading names of zip files
// ↳ Load names into custom datatype map<string, <map<string, bool>, map<string, unordered_set> [A]
// ↳ From favourites.txt load into [A]
// ↳ Get dependencies by unzipping mod zips and reading everst.yaml
// ↳ Load dependencies into [A]
// ↳ Get presets from modpresets
// ↳ Load into a dict<string, set>[C]

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
    std::cout << "Hello, World!" << std::endl;
    return 0;
}