#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <iostream>

#include "json.hpp"  //credit to nlohmann on GitHub for the json handler library
using json = nlohmann::json;

void init() {
    fs::path filePath = fs::current_path() / "settings.json";

    std::ifstream settingsFile(filePath);
    if (settingsFile.is_open()) {
        std::cout << "open";
    } else {
        std::cout << "closed";
    }
}

int main() {
    init();
    return 0;
}