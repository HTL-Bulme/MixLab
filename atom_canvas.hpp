#ifndef ATOM_CANVAS_HPP
#define ATOM_CANVAS_HPP

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/timer.h>
#include <vector>

namespace mixlab {

struct Teilchen {
    float x, y;
    float vx, vy;
    float radius;
    wxColour farbe;
    wxString symbol;
};

class AtomCanvas : public wxPanel {
public:
    AtomCanvas(wxWindow* parent);

    void setDunkelModus(bool dunkel);
    void setGeschwindigkeit(int wert);

private:
    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);

    wxTimer* timer_;
    bool dunkelModus_ = true;
    int geschwindigkeit_ = 3;

    std::vector<Teilchen> teilchen_;
    float globalZeit_ = 0;
    
    wxDECLARE_EVENT_TABLE();
};

} // namespace mixlab

#endif // ATOM_CANVAS_HPP
