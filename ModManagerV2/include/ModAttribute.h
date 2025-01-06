#ifndef MODATTRIBUTE_H
#define MODATTRIBUTE_H

#include <set>
#include <string>

class ModAttribute {
   public:
    ModAttribute(bool isFavourite, bool isEnabled, std::set<std::string> dependencies);
    void print(std::string modName, int indent) const;
    bool getIsFavourite() const;
    void setIsFavourite(bool isFavourite);
    std::set<std::string> getDependencies() const;
    void addDependency(std::string dependency);
    bool getIsEnabled() const;
    void setIsEnabled(bool isEnabled);

   private:
    bool isFavourite;
    std::set<std::string> dependencies;
    bool isEnabled;
};

#endif
