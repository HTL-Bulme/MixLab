#include "atom_canvas.hpp"
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

namespace mixlab {

wxBEGIN_EVENT_TABLE(AtomCanvas, wxPanel)
    EVT_PAINT(AtomCanvas::OnPaint)
wxEND_EVENT_TABLE()

AtomCanvas::AtomCanvas(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void AtomCanvas::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    int W = GetSize().GetWidth();
    int H = GetSize().GetHeight();

    dc.SetBackground(wxBrush(
        dunkelModus_ ? wxColour(13,17,23) : wxColour(240,244,248)
    ));
    dc.Clear();

    dc.SetPen(wxPen(
        dunkelModus_ ? wxColour(255,255,255,15) : wxColour(0,0,0,20), 1
    ));
    for (int x = 0; x < W; x += 28)
        dc.DrawLine(x, 0, x, H);
    for (int y = 0; y < H; y += 28)
        dc.DrawLine(0, y, W, y);

    dc.SetBrush(wxBrush(wxColour(159,225,203)));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawCircle(W/2, H/2, 40);
}

void AtomCanvas::setDunkelModus(bool dunkel) {
    dunkelModus_ = dunkel;
    Refresh();
}

void AtomCanvas::setGeschwindigkeit(int wert) {
    geschwindigkeit_ = wert;
}

} // namespace mixlab
