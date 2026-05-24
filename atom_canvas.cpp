#include "atom_canvas.hpp"
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <cmath>

namespace mixlab {

wxBEGIN_EVENT_TABLE(AtomCanvas, wxPanel)
    EVT_PAINT(AtomCanvas::OnPaint)
    EVT_TIMER(wxID_ANY, AtomCanvas::OnTimer)
wxEND_EVENT_TABLE()

AtomCanvas::AtomCanvas(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    timer_ = new wxTimer(this);
    timer_->Start(16); // 16мс = ~60 FPS
}

void AtomCanvas::OnTimer(wxTimerEvent& event) {
    globalZeit_ += 1.0f;
    Refresh();
}

void AtomCanvas::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    int W = GetSize().GetWidth();
    int H = GetSize().GetHeight();

    dc.SetBackground(wxBrush(
        dunkelModus_ ? wxColour(13,17,23) : wxColour(240,244,248)
    ));
    dc.Clear();

    // Netz
    dc.SetPen(wxPen(
        dunkelModus_ ? wxColour(255,255,255,15) : wxColour(0,0,0,20), 1
    ));
    for (int x = 0; x < W; x += 28) dc.DrawLine(x, 0, x, H);
    for (int y = 0; y < H; y += 28) dc.DrawLine(0, y, W, y);

    // test atom ->
    float atomX = std::fmod(globalZeit_ * geschwindigkeit_, W);
    float atomY = H / 2.0f;
    dc.SetBrush(wxBrush(wxColour(181, 212, 244))); // blau - H
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawCircle((int)atomX, (int)atomY, 16);

    dc.SetTextForeground(wxColour(12, 68, 124));
    dc.DrawText(wxT("H"), (int)atomX - 5, (int)atomY - 8);
    //
    // test atom 2 <— 
    float atom2X = W - std::fmod(globalZeit_ * geschwindigkeit_, W / 2.0f);
    float atom2Y = H / 2.0f;

    dc.SetBrush(wxBrush(wxColour(159, 225, 203))); // green = O
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawCircle((int)atom2X, (int)atom2Y, 16);
    dc.SetTextForeground(wxColour(8, 80, 65));
    dc.DrawText(wxT("O"), (int)atom2X - 5, (int)atom2Y - 8);
    // 

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
