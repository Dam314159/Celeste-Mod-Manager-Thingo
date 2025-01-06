#ifndef MODATTRIBUTE_H
#define MODATTRIBUTE_H

#include <set>
#include <string>

class ModAttribute {
   public:
    ModAttribute(const bool &isFavourite, const bool &isEnabled, const std::set<std::string> &dependencies);
    void print(const std::string &modName, const int &indent) const;
    bool getIsFavourite() const;
    void setIsFavourite(const bool &isFavourite);
    std::set<std::string> getDependencies() const;
    void addDependency(const std::string &dependency);
    bool getIsEnabled() const;
    void setIsEnabled(const bool &isEnabled);

   private:
    bool isFavourite;
    std::set<std::string> dependencies;
    bool isEnabled;
};

#endif
