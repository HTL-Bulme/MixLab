#include "reaction_adapter.hpp"
#include "gui_dialogs.hpp"
#include <wx/wx.h>
namespace mixlab {
void ReactionAdapter::onAboutClicked([[maybe_unused]] wxCommandEvent& event) {
    showAboutDialog(Language::English);
}
} // namespace mixlab