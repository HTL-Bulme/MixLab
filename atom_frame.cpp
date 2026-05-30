#include "atom_frame.hpp"
#include "atom_canvas.hpp"
#include "reaction.hpp"
#include "gui_sidebar.hpp"

#include <wx/wx.h>

namespace mixlab {

AtomFrame::AtomFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 620)) {
  SetBackgroundStyle(wxBG_STYLE_PAINT);
  SetMinSize(wxSize(800, 560));

  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
  
  // Toolbar layout
  wxPanel* toolbar = new wxPanel(this);
  wxBoxSizer* tbSizer = new wxBoxSizer(wxHORIZONTAL);

  tbSizer->Add(new wxStaticText(toolbar, wxID_ANY,
      wxT("Geschwindigkeit:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
  
  speedSlider_ = new wxSlider(toolbar, wxID_ANY, 3, 1, 5, wxDefaultPosition, wxSize(90, -1)); 
  tbSizer->Add(speedSlider_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

  wxButton* themeBtn = new wxButton(toolbar, wxID_ANY,
      wxT("Tag/Nacht"), wxDefaultPosition, wxSize(80, -1));
  tbSizer->Add(themeBtn, 0, wxALIGN_CENTER_VERTICAL);

  toolbar->SetSizer(tbSizer);
  mainSizer->Add(toolbar, 0, wxEXPAND | wxGROW | wxALL, 6);

  // Sidebar layout
  wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

  // wxPanel* sidebarPanel = new wxPanel(this);
  sidebar_ = new GuiSidebar(this);
  // sidebarPanel->SetBackgroundColour(wxColour(60, 60, 60));
  sidebar_->SetMinSize(wxSize(160, -1));

  wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

  contentSizer->Add(sidebar_, 0, wxEXPAND);
  contentSizer->Add(rightSizer, 1, wxEXPAND);
  
  mainSizer->Add(contentSizer, 1, wxEXPAND | wxGROW | wxALL, 6);

  // Input the elements
  currentInput_.element1 = "H";
  currentInput_.count1 = 2;
  currentInput_.element2 = "O";
  currentInput_.count2 = 1;

  selectionText_ = new wxStaticText(this, wxID_ANY, "Auswahl: -");
  rightSizer->Add(selectionText_, 0, wxALL, 8);
  updateSelectionText();

  // Main field with animation
  canvas_ = new AtomCanvas(this);
  rightSizer->Add(canvas_, 1, wxEXPAND);

  SetSizer(mainSizer);

  // Events
  themeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    dunkelModus_ = !dunkelModus_;
    canvas_->setDunkelModus(dunkelModus_);
    sidebar_->setDarkMode(dunkelModus_); 
  });

  speedSlider_->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
    canvas_->setGeschwindigkeit(speedSlider_->GetValue());
  });

  // // Test
  resultFormulaText_ = new wxStaticText(this, wxID_ANY, "Formel: -");
  resultNameText_ = new wxStaticText(this, wxID_ANY, "Name: -");
  resultStatusText_ = new wxStaticText(this, wxID_ANY, "Status: -");

  rightSizer->Add(resultFormulaText_, 0, wxALL, 5);
  rightSizer->Add(resultNameText_, 0, wxALL, 5);
  rightSizer->Add(resultStatusText_, 0, wxALL, 5);

  ReactionResult result = calculateReaction(currentInput_);
  updateReactionResult(result);
  sidebar_->addHistoryEntry(result.formula, result.name);

}

void AtomFrame::updateSelectionText() {
  if(!selectionText_) return;

  std::string text = "Auswahl: " + currentInput_.element1 + std::to_string(currentInput_.count1) +
      " + " + currentInput_.element2 + std::to_string(currentInput_.count2);
  
  selectionText_->SetLabel(wxString::FromUTF8(text));

  Layout();
}

void AtomFrame::updateReactionResult(const ReactionResult& result) {
    if (!resultFormulaText_ || !resultNameText_ || !resultStatusText_) return;

    resultFormulaText_->SetLabel(result.formula);
    resultNameText_->SetLabel(result.name);
    resultStatusText_->SetLabel(result.statusText);

    Layout();
}

} // namespace mixlab