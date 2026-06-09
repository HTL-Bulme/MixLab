#include "reaction_controller.hpp"

#include "reaction.hpp"

namespace mixlab {

ReactionController::ReactionController(UiState& state)
    : state_(state) {}

void ReactionController::setElement1(const std::string& symbol) {
    state_.input.element1 = symbol;
}

void ReactionController::setElement2(const std::string& symbol) {
    state_.input.element2 = symbol;
}

void ReactionController::setCount1(int count) {
    state_.input.count1 = count;
}

void ReactionController::setCount2(int count) {
    state_.input.count2 = count;
}

void ReactionController::setSpeed(int speed) {
    state_.speed = speed;
}

void ReactionController::setDarkMode(bool darkMode) {
    state_.darkMode = darkMode;
}

void ReactionController::toggleDarkMode() {
    state_.darkMode = !state_.darkMode;
}

const ReactionResult& ReactionController::recompute() {
    state_.currentResult = calculateReaction(state_.input);
    return state_.currentResult;
}

} // namespace mixlab
