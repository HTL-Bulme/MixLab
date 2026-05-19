#ifndef ATOM_CANVAS_HPP
#define ATOM_CANVAS_HPP

#include <wx/wx.h>
#include <wx/panel.h>

namespace mixlab {

class AtomCanvas : public wxPanel {
public:
    AtomCanvas(wxWindow* parent);

private:
    void OnPaint(wxPaintEvent& event);
    wxDECLARE_EVENT_TABLE();
};

} // namespace mixlab

#endif // ATOM_CANVAS_HPP
