#include "StateHandler.h"

namespace state {

static std::string state;
static std::string subState;
static std::stack<std::pair<std::string, std::string>> tracker;

void updateState(std::string newState, std::string newSubState) {
    tracker.push({state, subState});
    state = newState;
    subState = newSubState;
}
void setState(std::string newState, std::string newSubstate, std::stack<std::pair<std::string, std::string>> newTracker) {
    state = newState;
    subState = newSubstate;
}

void returnToPreviousState() {
    if (tracker.empty()) {
        throw std::runtime_error("Tracker is empty, cannot return to previous state");
    }
    state = tracker.top().first;
    subState = tracker.top().second;
    tracker.pop();
}
std::string getState() {
    return state;
};
std::string getSubState() {
    return subState;
};
std::stack<std::pair<std::string, std::string>> getTracker() {
    return tracker;
};

}  // namespace state