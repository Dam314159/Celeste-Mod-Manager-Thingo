#include "StateHandler.h"

namespace state {

static std::string state;
static std::string substate;
static std::stack<std::pair<std::string, std::string>> tracker;

void updateState(const std::string &newState, const std::string &newSubstate) {
    logger::functionCall("state::updateState", {newState, newSubstate});
    tracker.push({state, substate});
    state = newState;
    substate = newSubstate;
    logger::log("State updated to " + newState + ", " + newSubstate);
    logger::functionExit();
}

void setState(const std::string &newState, const std::string &newSubstate, std::stack<std::pair<std::string, std::string>> newTracker) {
    logger::functionCall("state::setState", {newState, newSubstate, anyToString(newTracker)});
    state = newState;
    substate = newSubstate;
    logger::log("State set to " + newState + ", " + newSubstate);
    tracker = newTracker;
    logger::log("Tracker set to " + anyToString(newTracker));
    logger::functionExit();
}

void returnToPreviousState() {
    logger::functionCall("state::returnToPreviousState", {});
    if (tracker.empty()) {
        logger::critical("Tracker is empty, cannot return to previous state");
        throw std::runtime_error("Tracker is empty, cannot return to previous state");
    }

    state = tracker.top().first;
    substate = tracker.top().second;
    tracker.pop();
    logger::log("State and tracker set");
    logger::functionExit();
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