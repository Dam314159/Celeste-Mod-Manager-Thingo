#ifndef MODATTRIBUTE_H
#define MODATTRIBUTE_H

#include <iostream>
#include <set>
#include <string>

#include "Logger.h"

class ModAttribute {
   public:
    ModAttribute(const bool &isfavorite = false, const bool &isEnabled = false, const std::set<std::string> &dependencies = {});
    void print(const std::string &modName, const int &indent) const;
    bool getIsfavorite() const;
    void setIsfavorite(const bool &isfavorite);
    std::set<std::string> getDependencies() const;
    void addDependency(const std::string &dependency);
    bool getIsEnabled() const;
    void setIsEnabled(const bool &isEnabled);

   private:
    bool isfavorite;
    std::set<std::string> dependencies;
    bool isEnabled;
};

#endif
