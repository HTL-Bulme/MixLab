#include "reaction_history.hpp"
#include "reaction_adapter.hpp"
#include "gui_sidebar.hpp"
#include "gui_menu.hpp"
#include "gui_dialogs.hpp"
#include "atom_app.hpp"
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
    wxInitializer initializer;
    if (!initializer) {
        return -1;
    }
    mixlab::ReactionAdapter adapter;
    wxCommandEvent event;
    adapter.onAboutClicked(event);
    return 0;
}