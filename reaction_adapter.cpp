#include "reaction_adapter.hpp"
#include "gui_dialogs.hpp"
#include <wx/wx.h>
namespace mixlab {
void ReactionAdapter::onAboutClicked(wxCommandEvent& event) {
    showAboutDialog();
}
} // namespace mixlab