#include "atom_canvas.hpp"
#include <wx/dcclient.h>

namespace mixlab {

wxBEGIN_EVENT_TABLE(AtomCanvas, wxPanel)
    EVT_PAINT(AtomCanvas::OnPaint)
wxEND_EVENT_TABLE()

AtomCanvas::AtomCanvas(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void AtomCanvas::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    dc.DrawRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight());
}

} // namespace mixlab
