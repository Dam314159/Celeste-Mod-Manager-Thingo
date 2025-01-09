#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

#include "Logger.h"

namespace state {

void updateState(const std::string &newState, const std::string &newSubstate);
void setState(const std::string &newState, const std::string &newSubstate, std::stack<std::pair<std::string, std::string>> newTracker);
void returnToPreviousState();
std::string getState();
std::string getSubState();
std::stack<std::pair<std::string, std::string>> getTracker();

}  // namespace state
#endif