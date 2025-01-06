#include "ModAttribute.h"

#include <iostream>

ModAttribute::ModAttribute(bool isFavourite, std::set<std::string> dependencies, bool isEnabled)
    : isFavourite(isFavourite), dependencies(dependencies), isEnabled(isEnabled) {}

void ModAttribute::print(int indent) const {
    std::string spaces(indent, ' ');
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

void ModAttribute::setIsFavourite(bool isFavourite) {
    this->isFavourite = isFavourite;
}

std::set<std::string> ModAttribute::getDependencies() const {
    return dependencies;
}

void ModAttribute::addDependency(std::string dependency) {
    dependencies.insert(dependency);
}

bool ModAttribute::getIsEnabled() const {
    return isEnabled;
}

void ModAttribute::setIsEnabled(bool isEnabled) {
    this->isEnabled = isEnabled;
}