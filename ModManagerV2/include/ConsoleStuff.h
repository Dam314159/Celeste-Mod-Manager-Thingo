#ifndef CONSOLESTUFF_H
#define CONSOLESTUFF_H

#include <cstdlib>
#include <functional>
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
std::string ask(const std::string &question,
                const std::function<bool(std::string)> &validation,
                const std::function<std::string(std::string)> &subsequent);

#endif