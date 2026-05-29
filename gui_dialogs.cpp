#include "gui_dialogs.hpp"
#include <wx/wx.h>
namespace mixlab {
void showAboutDialog() {
    wxMessageBox("MixLab is a digital audio workstation (DAW) designed for music production and audio editing. It provides a user-friendly interface and a wide range of features for musicians, producers, and audio engineers.", "About MixLab", wxICON_INFORMATION | wxOK);
}

} // namespace mixlab
