#ifndef REACTION_CONTROLLER_HPP
#define REACTION_CONTROLLER_HPP

#include "ui_state.hpp"
#include <string>

namespace mixlab {

class ReactionController {
public:
    explicit ReactionController(UiState& state);

    void setElement1(const std::string& symbol);
    void setElement2(const std::string& symbol);
    void setCount1(int count);
    void setCount2(int count);
    void setSpeed(int speed);
    void setDarkMode(bool darkMode);
    void toggleDarkMode();

    const ReactionResult& recompute();

private:
    UiState& state_;
};

} // namespace mixlab

#endif // REACTION_CONTROLLER_HPP
