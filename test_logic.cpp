#include "reaction_history.hpp"
#include "reaction_adapter.hpp"
#include "reaction.hpp"
#include <wx/wx.h>
#include <cassert>

void testReactionHistory() {
    mixlab::ReactionHistory history(3);
    history.addReaction("Reaction 1");
    history.addReaction("Reaction 2");
    history.addReaction("Reaction 3");
    history.addReaction("Reaction 4");

    const auto& reactions = history.getReactions();
    assert(reactions.size() == 3);
    assert(reactions[0] == "Reaction 2");
    assert(reactions[1] == "Reaction 3");
    assert(reactions[2] == "Reaction 4");
}

int main() {
    testReactionHistory();

    mixlab::ReactionInput input;
    input.element1 = "H";
    input.count1 = 2;
    input.element2 = "O";
    input.count2 = 1;

    const auto result = mixlab::calculateReaction(input);
    assert(result.status == mixlab::ReactionStatus::Safe);
    assert(result.formula == "H2O");
    assert(result.known);

    wxInitializer initializer;
    if (!initializer) {
        return -1;
    }

    mixlab::ReactionAdapter adapter;
    wxCommandEvent event;
    adapter.onAboutClicked(event);
    return 0;
}
