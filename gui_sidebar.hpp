#ifndef GUI_SIDEBAR_HPP
#define GUI_SIDEBAR_HPP

#include <wx/wx.h>

namespace mixlab {

class SidebarPanel : public wxPanel {
public:
    SidebarPanel(wxWindow* parent);

private:
    void onAboutClicked(wxCommandEvent& event);
};

} // namespace mixlab

#endif // GUI_SIDEBAR_HPP
