#include "reaction_history.hpp"
#include <wx/wx.h>
namespace mixlab {
void ReactionHistory::addReaction(const std::string& reaction) {
    reactions.push_back(reaction);
    if (reactions.size() > maxSize) {
        reactions.erase(reactions.begin());
    }
}  
const std::vector<std::string>& ReactionHistory::getReactions() const {
    return reactions;
}

} // namespace mixlab