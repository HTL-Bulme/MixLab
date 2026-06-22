#include "reaction_history.hpp"

namespace mixlab {

ReactionHistory::ReactionHistory(size_t maxSize)
    : maxSize_(maxSize) {
}

void ReactionHistory::addReaction(const std::string& reaction) {
    if (!reactions_.empty() &&
        reactions_.back() == reaction) {
        return;
    }
    reactions_.push_back(reaction);
    if (reactions_.size() > maxSize_) {
        reactions_.erase(reactions_.begin());
    }
}

const std::vector<std::string>& ReactionHistory::getReactions() const {
    return reactions_;
}

} // namespace mixlab
