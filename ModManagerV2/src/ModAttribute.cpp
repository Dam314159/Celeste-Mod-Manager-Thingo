#include "ModAttribute.h"

ModAttribute::ModAttribute(const bool &isFavourite, const bool &isEnabled, const std::set<std::string> &dependencies)
    : isFavourite(isFavourite), isEnabled(isEnabled), dependencies(dependencies) {
    logger::log({"ModAttribute.cpp", "Constructor"}, "Created ModAttribute object with isFavourite: " + std::to_string(isFavourite) + ", isEnabled: " + std::to_string(isEnabled) + ", and dependencies size: " + std::to_string(dependencies.size()));
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
    logger::log({"ModAttribute.cpp", "print"}, "Called print.");
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