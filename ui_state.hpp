#ifndef UI_STATE_HPP
#define UI_STATE_HPP

#include "reaction.hpp"

namespace mixlab {

struct UiState {
    ReactionInput input;
    int speed = 3;
    bool darkMode = true;
    int savedCount = 0;
    ReactionResult currentResult;
};

} // namespace mixlab

#endif // UI_STATE_HPP
