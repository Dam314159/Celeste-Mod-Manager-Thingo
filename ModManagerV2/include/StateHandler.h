#ifndef GLOBALSTATEHANDLER_H
#define GLOBALSTATEHANDLER_H

#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

namespace state {

void updateState(std::string newState, std::string newSubState);
void setState(std::string newState, std::string newSubstate, std::stack<std::pair<std::string, std::string>> newTracker);
void returnToPreviousState();
std::string getState();
std::string getSubState();
std::stack<std::pair<std::string, std::string>> getTracker();

}  // namespace state
#endif