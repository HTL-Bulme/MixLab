#include "atom_frame.hpp"
#include "atom_canvas.hpp"
#include "gui_sidebar.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace mixlab {

AtomFrame::AtomFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 620)),
    reactionController_(uiState_) {
  SetBackgroundStyle(wxBG_STYLE_PAINT);
  SetMinSize(wxSize(800, 560));

  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

  // Toolbar layout
  wxPanel* toolbar = new wxPanel(this);
  wxBoxSizer* tbSizer = new wxBoxSizer(wxHORIZONTAL);

  wxBoxSizer* speedGroup = new wxBoxSizer(wxHORIZONTAL);
  speedGroup->Add(new wxStaticText(toolbar, wxID_ANY, wxT("Geschwindigkeit:")),
      0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 8);
  speedSlider_ = new wxSlider(toolbar, wxID_ANY, 3, 1, 5,
      wxDefaultPosition, wxSize(150, -1));
  speedGroup->Add(speedSlider_, 0, wxALIGN_CENTER_VERTICAL);

    pauseBtn_ = new wxButton(toolbar, wxID_ANY,
      wxT("Pause"), wxDefaultPosition, wxSize(88, -1));

  wxButton* themeBtn = new wxButton(toolbar, wxID_ANY,
      wxT("Tag/Nacht"), wxDefaultPosition, wxSize(96, -1));

  tbSizer->Add(speedGroup, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
  tbSizer->Add(pauseBtn_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
  tbSizer->AddStretchSpacer(1);
  tbSizer->Add(themeBtn, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);

  toolbar->SetSizer(tbSizer);
  mainSizer->Add(toolbar, 0, wxEXPAND | wxGROW | wxLEFT | wxRIGHT | wxTOP, 6);

  // Sidebar layout
  wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

  // wxPanel* sidebarPanel = new wxPanel(this);
  sidebar_ = new GuiSidebar(this);
  // sidebarPanel->SetBackgroundColour(wxColour(60, 60, 60));
  sidebar_->SetMinSize(wxSize(160, -1));
  sidebar_->Bind(wxEVT_LISTBOX, [this](wxCommandEvent&){applySidebarSelection();});

  wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

  contentSizer->Add(sidebar_, 0, wxEXPAND);
  contentSizer->Add(rightSizer, 1, wxEXPAND);

  mainSizer->Add(contentSizer, 1, wxEXPAND | wxGROW | wxALL, 6);

    wxBoxSizer* selectionRow = new wxBoxSizer(wxHORIZONTAL);
    selectionText_ = new wxStaticText(this, wxID_ANY, "Auswahl:");
    selectionRow->Add(selectionText_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

    element1Text_ = new wxStaticText(this, wxID_ANY, wxT("-:"));
    selectionRow->Add(element1Text_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 6);
    count1_ = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
      wxDefaultPosition, wxSize(64, -1), wxSP_ARROW_KEYS, 0, 4, 0);
    selectionRow->Add(count1_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

    selectionRow->Add(new wxStaticText(this, wxID_ANY, "+"),
      0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

    element2Text_ = new wxStaticText(this, wxID_ANY, wxT("-:"));
    selectionRow->Add(element2Text_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 6);
    count2_ = new wxSpinCtrl(this, wxID_ANY, wxEmptyString,
      wxDefaultPosition, wxSize(64, -1), wxSP_ARROW_KEYS, 0, 4, 0);
    selectionRow->Add(count2_, 0, wxALIGN_CENTER_VERTICAL);

    // Keep internal reaction state in sync with visible default counters.
    reactionController_.setCount1(count1_->GetValue());
    reactionController_.setCount2(count2_->GetValue());

    rightSizer->Add(selectionRow, 0, wxALL, 8);
  updateSelectionText();

  SetSizer(mainSizer);

  // Events
  themeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    reactionController_.toggleDarkMode();
    canvas_->setDunkelModus(uiState_.darkMode);
    sidebar_->setDarkMode(uiState_.darkMode);
  });

  speedSlider_->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
    reactionController_.setSpeed(speedSlider_->GetValue());
    canvas_->setGeschwindigkeit(uiState_.speed);
  });

  pauseBtn_->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    toggleAnimationPause();
  });

  Bind(wxEVT_CHAR_HOOK, [this](wxKeyEvent& event) {
    const int key = event.GetKeyCode();
    if (key == WXK_SPACE || key == 'P' || key == 'p') {
      toggleAnimationPause();
      return;
    }
    event.Skip();
  });

  count1_->Bind(wxEVT_SPINCTRL, [this](wxCommandEvent&) {
    applyAtomCounts();
  });
  count2_->Bind(wxEVT_SPINCTRL, [this](wxCommandEvent&) { 
    applyAtomCounts();
  });

  // Main field with animation
  canvas_ = new AtomCanvas(this);
  canvas_->setAtoms(
    uiState_.input.element1,
    uiState_.input.count1,
    uiState_.input.element2,
    uiState_.input.count2
  );
  rightSizer->Add(canvas_, 1, wxEXPAND);

  // Result row
  resultFormulaText_ = new wxStaticText(this, wxID_ANY, "Formel: -");
  resultNameText_ = new wxStaticText(this, wxID_ANY, "Name: -");
  resultStatusText_ = new wxStaticText(this, wxID_ANY, "Status: -");
  resultHintText_ = new wxStaticText(this, wxID_ANY, "Hinweis: -");
  resultHintText_->Wrap(760);

  rightSizer->Add(resultFormulaText_, 0, wxALL, 5);
  rightSizer->Add(resultNameText_, 0, wxALL, 5);
  rightSizer->Add(resultStatusText_, 0, wxALL, 5);
  rightSizer->Add(resultHintText_, 0, wxALL, 5);

}

void AtomFrame::toggleAnimationPause() {
  animationPaused_ = !animationPaused_;
  if (canvas_) {
    canvas_->setAnimationPaused(animationPaused_);
  }
  if (pauseBtn_) {
    pauseBtn_->SetLabel(animationPaused_ ? wxT("Resume") : wxT("Pause"));
  }
}

void AtomFrame::updateSelectionText() {
  if(!selectionText_) return;

  if (element1Text_) {
    const wxString label1 = uiState_.input.element1.empty()
      ? wxString("-:")
      : wxString::FromUTF8(uiState_.input.element1 + ":");
    element1Text_->SetLabel(label1);
  }
  if (element2Text_) {
    const wxString label2 = uiState_.input.element2.empty()
      ? wxString("-:")
      : wxString::FromUTF8(uiState_.input.element2 + ":");
    element2Text_->SetLabel(label2);
  }

  selectionText_->SetLabel("Auswahl:");

  Layout();
}

void AtomFrame::updateReactionResult(const ReactionResult& result) {
  if (!resultFormulaText_ || !resultNameText_ || !resultStatusText_ || !resultHintText_) return;

  resultFormulaText_->SetLabel("Formel: " + result.formula);
  resultNameText_->SetLabel("Name: " + result.name);
  resultStatusText_->SetLabel("Status: " + result.statusText);
  resultHintText_->SetLabel("Hinweis: " + result.hint);

  if (canvas_) {
    canvas_->setReactionStatus(result.status, result.statusText);
  }

    Layout();
}

void AtomFrame::applySidebarSelection() {
  const std::string element = sidebar_->getSelectedElement();
  if (element.empty()) return;

  if (!hasFirstSelection_ || hasSecondSelection_) {
    reactionController_.setElement1(element);
    if (uiState_.input.count1 <= 0) {
      reactionController_.setCount1(1);
      if (count1_) {
        count1_->SetValue(1);
      }
    }
    hasFirstSelection_ = true;
    hasSecondSelection_ = false;
  } else {
    reactionController_.setElement2(element);
    if (uiState_.input.count2 <= 0) {
      reactionController_.setCount2(1);
      if (count2_) {
        count2_->SetValue(1);
      }
    }
    hasSecondSelection_ = true;
  }

  updateSelectionText();
  if (canvas_) {
    canvas_->setAtoms(
      uiState_.input.element1,
      uiState_.input.count1,
      uiState_.input.element2,
      uiState_.input.count2
    );
  }

  const ReactionResult& result = reactionController_.recompute();
  updateReactionResult(result);
  sidebar_->addHistoryEntry(result.formula, result.name);
}

void AtomFrame::applyAtomCounts() {
  if (!count1_ || !count2_) return;

  reactionController_.setCount1(count1_->GetValue());
  reactionController_.setCount2(count2_->GetValue());
  updateSelectionText();
  if (canvas_) {
    canvas_->setAtoms(
      uiState_.input.element1,
      uiState_.input.count1,
      uiState_.input.element2,
      uiState_.input.count2
    );
  }

  const ReactionResult& result = reactionController_.recompute();
  updateReactionResult(result);
}

} // namespace mixlab