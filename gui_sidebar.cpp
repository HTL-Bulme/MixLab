#include "gui_sidebar.hpp"
#include <wx/wx.h>
namespace mixlab {
SidebarPanel::SidebarPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxButton* aboutButton = new wxButton(this, wxID_ANY, "About");
    sizer->Add(aboutButton, 0, wxALL | wxEXPAND, 5);
    SetSizer(sizer);
    aboutButton->Bind(wxEVT_BUTTON, &SidebarPanel::onAboutClicked, this);
}