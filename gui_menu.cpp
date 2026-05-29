#include "gui_menu.hpp"
#include <wx/wx.h>
namespace mixlab {
wxMenuBar* createMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "&Exit");
    menuBar->Append(fileMenu, "&File");
    return menuBar;
}
} // namespace mixlab
