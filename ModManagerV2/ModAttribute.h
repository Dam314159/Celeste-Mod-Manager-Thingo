#ifndef MODATTRIBUTE_H
#define MODATTRIBUTE_H

#include <set>
#include <string>

namespace ModAttribute {

class ModAttribute {
   public:
    ModAttribute(bool isFavourite = false, std::set<std::string> dependencies = {}, bool isEnabled = false);
    void print(int indent) const;
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

}  // namespace ModAttribute

#endif
