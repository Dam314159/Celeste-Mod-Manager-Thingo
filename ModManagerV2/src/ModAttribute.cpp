#include "ModAttribute.h"

ModAttribute::ModAttribute(const bool &isfavorite, const bool &isEnabled, const std::set<std::string> &dependencies)
    : isfavorite(isfavorite), isEnabled(isEnabled), dependencies(dependencies) {
    logger::functionCall("ModAttribute constructor", {anyToString(isfavorite), anyToString(isEnabled), anyToString(dependencies)});
    logger::log("Created ModAttribute object");
    logger::functionExit();
}

// debug use only
void ModAttribute::print(const std::string &modName, const int &indent) const {
    std::string spaces(indent, ' ');
    std::cout << "Mod Name: " << modName << '\n';
    std::cout << spaces << "Is favorite: " << (isfavorite ? "True" : "False") << '\n';
    std::cout << spaces << "Dependencies:\n";
    for (const auto &dependency : dependencies) {
        std::cout << spaces << spaces << dependency << '\n';
    }
    std::cout << spaces << "Is Enabled: " << (isEnabled ? "True" : "False") << "\n\n";
    logger::log("Called print.");
}

bool ModAttribute::getIsFavorite() const {
    return isfavorite;
}

void ModAttribute::setIsFavorite(const bool &isfavorite) {
    this->isfavorite = isfavorite;
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