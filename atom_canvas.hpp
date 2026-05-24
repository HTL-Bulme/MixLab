#ifndef ATOM_CANVAS_HPP
#define ATOM_CANVAS_HPP

#include <wx/wx.h>
#include <wx/panel.h>

namespace mixlab {

class AtomCanvas : public wxPanel {
public:
    AtomCanvas(wxWindow* parent);

    void setDunkelModus(bool dunkel);
    void setGeschwindigkeit(int wert);

private:
    void OnPaint(wxPaintEvent& event);
    wxDECLARE_EVENT_TABLE();

    bool dunkelModus_ = true;
    int geschwindigkeit_ = 3;
};

} // namespace mixlab

#endif // ATOM_CANVAS_HPP
