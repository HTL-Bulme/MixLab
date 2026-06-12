#ifndef REACTION_HISTORY_HPP
#define REACTION_HISTORY_HPP

#include <string>
#include <vector>

namespace mixlab {

class ReactionHistory {
public:
    explicit ReactionHistory(size_t maxSize = 10);
    void addReaction(const std::string& reaction);
    const std::vector<std::string>& getReactions() const;

private:
    std::vector<std::string> reactions_;
    size_t maxSize_;
};

} // namespace mixlab

#endif // REACTION_HISTORY_HPP
