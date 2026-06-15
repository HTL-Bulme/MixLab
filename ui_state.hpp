#ifndef UI_STATE_HPP
#define UI_STATE_HPP

#include "language.hpp"
#include "reaction.hpp"

#include <vector>

namespace mixlab {

struct UiState {
    ReactionInput input;
    int speed = 3;
    bool darkMode = true;
    int savedCount = 0;
    ReactionResult currentResult;
    std::vector<ReactionResult> savedReactions;
    Language language = Language::English;
};

} // namespace mixlab

#endif // UI_STATE_HPP
