#include "atom_frame.hpp"
#include "atom_canvas.hpp"
#include <wx/wx.h>

namespace mixlab {

AtomFrame::AtomFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)) {
  SetBackgroundStyle(wxBG_STYLE_PAINT);
  canvas_ = new AtomCanvas(this);
}

} // namespace mixlab