#include "atom_app.hpp"
#include "atom_frame.hpp"
#include <wx/wx.h>

namespace mixlab {

bool AtomApp::OnInit() {
    AtomFrame* frame = new AtomFrame("MixLab");
    frame->SetClientSize(1000, 800);
    frame->Center();
    frame->Show(true);
    return true;
}

} // namespace mixlab

wxIMPLEMENT_APP(mixlab::AtomApp);


