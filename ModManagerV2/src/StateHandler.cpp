#include "StateHandler.h"

namespace state {

static std::string state;
static std::string substate;
static std::stack<std::pair<std::string, std::string>> tracker;

void updateState(const std::string &newState, const std::string &newSubstate) {
    tracker.push({state, substate});
    state = newState;
    substate = newSubstate;
    logger::log({"StateHandler.cpp", "updateState"}, "State updated to " + newState + ", " + newSubstate);
}

void setState(const std::string &newState, const std::string &newSubstate, std::stack<std::pair<std::string, std::string>> newTracker) {
    state = newState;
    substate = newSubstate;
    logger::log({"StateHandler.cpp", "updateState"}, "State set to " + newState + ", " + newSubstate);
    std::string loggerText = "";
    while (!newTracker.empty()) {
        const auto &state = newTracker.top();
        loggerText += state.first + ", " + state.second + " -> ";
        newTracker.pop();
    }
    logger::log({"StateHandler.cpp, updateState"}, "Tracker set to " + loggerText);
}

void returnToPreviousState() {
    if (tracker.empty()) {
        throw std::runtime_error("Tracker is empty, cannot return to previous state");
    }
    state = tracker.top().first;
    substate = tracker.top().second;
    tracker.pop();
}

std::string getState() {
    return state;
};

std::string getSubState() {
    return substate;
};

std::stack<std::pair<std::string, std::string>> getTracker() {
    return tracker;
};

}  // namespace state