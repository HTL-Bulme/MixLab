#include "../reaction_history.hpp"
#include "../reaction.hpp"
#include <cassert>
#include <string>

using mixlab::ReactionHistory;
using mixlab::ReactionInput;
using mixlab::ReactionStatus;

void testReactionHistory() {
    ReactionHistory history(3);

    history.addReaction("Reaction-1");
    history.addReaction("Reaction-2");
    history.addReaction("Reaction-3");
    history.addReaction("Reaction-4"); // pushes out Reaction-1

    const auto& reactions = history.getReactions();

    assert(reactions.size() == 3);
    assert(reactions[0] == "Reaction-2");
    assert(reactions[1] == "Reaction-3");
    assert(reactions[2] == "Reaction-4");
}

void testCalculateReaction() {
    // H2 + O → H2O (Safe)
    {
        ReactionInput input;
        input.element1 = "H";
        input.count1 = 2;
        input.element2 = "O";
        input.count2 = 1;

        const auto result = mixlab::calculateReaction(input);

        assert(result.status == ReactionStatus::Safe);
        assert(result.formula == "H2O");
        assert(result.known);
    }

    // C + O → CO (Dangerous)
    {
        ReactionInput input;
        input.element1 = "C";
        input.count1 = 1;
        input.element2 = "O";
        input.count2 = 1;

        const auto result = mixlab::calculateReaction(input);

        assert(result.status == ReactionStatus::Dangerous);
        assert(result.formula == "CO");
        assert(result.known);
    }

    // Unknown reaction
    {
        ReactionInput input;
        input.element1 = "Fe";
        input.count1 = 1;
        input.element2 = "S";
        input.count2 = 1;

        const auto result = mixlab::calculateReaction(input);

        assert(result.status == ReactionStatus::Unknown);
        assert(!result.known);
    }
}

int main() {
    testReactionHistory();
    testCalculateReaction();
}
