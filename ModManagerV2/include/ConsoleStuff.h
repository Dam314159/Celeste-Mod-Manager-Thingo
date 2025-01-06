#ifndef COLOURCOUT_H
#define COLOURCOUT_H

#include <string>
#include <unordered_map>

namespace colour {

void cout(std::string text, std::string colour);

void cerr(std::string text, std::string colour);

}  // namespace colour

void cls();

#endif