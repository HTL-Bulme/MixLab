#include "atom_frame.hpp"
#include "atom_canvas.hpp"
#include <wx/wx.h>

namespace mixlab {

AtomFrame::AtomFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 620)) {
  SetBackgroundStyle(wxBG_STYLE_PAINT);
  SetMinSize(wxSize(800, 560));

  // Sidebar layout
  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

  // Header layout
  wxPanel* toolbar = new wxPanel(this);
  wxBoxSizer* tbSizer = new wxBoxSizer(wxHORIZONTAL);

  tbSizer->Add(new wxStaticText(toolbar, wxID_ANY,
      wxT("Geschwindigkeit:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);

  speedSlider_ = new wxSlider(toolbar, wxID_ANY, 3, 1, 5,
      wxDefaultPosition, wxSize(90, -1));
  tbSizer->Add(speedSlider_, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
  
  tbSizer->Add(new wxStaticText(toolbar, wxID_ANY,
      wxT("Teilchen:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);

  partSlider_ = new wxSlider(toolbar, wxID_ANY, 6, 3, 12,
      wxDefaultPosition, wxSize(80, -1));
  tbSizer->Add(partSlider_, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

  wxButton* themeBtn = new wxButton(toolbar, wxID_ANY,
      wxT("Tag/Nacht"), wxDefaultPosition, wxSize(80, -1));
  tbSizer->Add(themeBtn, 0, wxALIGN_CENTER_VERTICAL);

  toolbar->SetSizer(tbSizer);
  mainSizer->Add(toolbar, 0, wxEXPAND | wxGROW | wxALL, 6);

  // Hauptfeld mit Animation
  canvas_ = new AtomCanvas(this);
  mainSizer->Add(canvas_, 1, wxEXPAND);

  SetSizer(mainSizer);

  // Events
  themeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    dunkelModus_ = !dunkelModus_;
    canvas_->setDunkelModus(dunkelModus_); 
  });

  speedSlider_->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
    canvas_->setGeschwindigkeit(speedSlider_->GetValue());
  });

}

} // namespace mixlab