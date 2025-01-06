#include "ModAttribute.h"

#include <iostream>

#include "Logger.h"

ModAttribute::ModAttribute(const bool &isFavourite = false, const bool &isEnabled = false, const std::set<std::string> &dependencies = {})
    : isFavourite(isFavourite), dependencies(dependencies), isEnabled(isEnabled) {
    logger::log("ModAttribute.cpp, ModAttribute", "Created ModAttribute object.");
}

// debug use only
void ModAttribute::print(const std::string &modName, const int &indent) const {
    std::string spaces(indent, ' ');
    std::cout << "Mod Name: " << modName << '\n';
    std::cout << spaces << "Is Favourite: " << (isFavourite ? "True" : "False") << '\n';
    std::cout << spaces << "Dependencies:\n";
    for (const auto &dependency : dependencies) {
        std::cout << spaces << spaces << dependency << '\n';
    }
    std::cout << spaces << "Is Enabled: " << (isEnabled ? "True" : "False") << "\n\n";
}

bool ModAttribute::getIsFavourite() const {
    return isFavourite;
}

void ModAttribute::setIsFavourite(const bool &isFavourite) {
    this->isFavourite = isFavourite;
}

std::set<std::string> ModAttribute::getDependencies() const {
    return dependencies;
}

void ModAttribute::addDependency(const std::string &dependency) {
    dependencies.insert(dependency);
}

bool ModAttribute::getIsEnabled() const {
    return isEnabled;
}

void ModAttribute::setIsEnabled(const bool &isEnabled) {
    this->isEnabled = isEnabled;
}