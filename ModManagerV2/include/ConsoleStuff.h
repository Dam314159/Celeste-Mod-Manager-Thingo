#ifndef COLOURCOUT_H
#define COLOURCOUT_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Logger.h"

namespace colour {

void cout(const std::string &text, const std::string &colour);
void cerr(const std::string &text, const std::string &colour);

}  // namespace colour

void cls();
void exitOnEnterPress(const int &exitCode, const std::string &message);

#endif