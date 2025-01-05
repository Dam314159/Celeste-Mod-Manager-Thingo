#include <bits/stdc++.h>

class ModAttribute {
   public:
    bool isFavourite;
    std::set<std::string> dependencies;

    ModAttribute(bool isfavourite = false, std::set<std::string> dependencies = {}) {
        this->isFavourite = isfavourite;
        this->dependencies = dependencies;
    }

    void addDependency(const std::string &dependency) {
        dependencies.insert(dependency);
    }

    void print(int tabSize) const {
        std::string tab = "";
        for (int i = 0; i < tabSize; i++) {
            tab += " ";
        }

        std::cout << tab << "is Favourite? : " << isFavourite << '\n';
        std::cout << tab << "Dependencies :\n";
        for (const std::string &dependency : dependencies) {
            std::cout << tab << tab << dependency << '\n';
        }
        std::cout << std::endl;
    }
};

int main() {
    std::map<std::string, ModAttribute> mods;
    mods.insert({"mod1", ModAttribute(true, {"mod2", "mod3", "mod4"})});
    mods.insert({"mod2", ModAttribute(false, {"mod5"})});
    mods.insert({"mod3", ModAttribute()});
    mods.insert({"mod4", ModAttribute()});
    mods.insert({"mod6", ModAttribute(true)});

    for (const auto &[key, value] : mods) {
        std::cout << "Mod name: " << key << '\n';
        value.print(2);
    }

    return 0;
}