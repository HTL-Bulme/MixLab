#include "atom_frame.hpp"
#include "atom_canvas.hpp"
#include "gui_sidebar.hpp"

#include <wx/wx.h>
#include <algorithm>
#include <initializer_list>

namespace mixlab {
namespace {

void setBold(wxWindow* window, int pointSizeDelta = 0) {
  if (!window) return;

  wxFont font = window->GetFont();
  font.SetWeight(wxFONTWEIGHT_BOLD);
  if (pointSizeDelta != 0) {
    font.SetPointSize(font.GetPointSize() + pointSizeDelta);
  }
  window->SetFont(font);
}

void applyColour(wxWindow* window, const wxColour& fg, const wxColour& bg) {
  if (!window) return;

  window->SetForegroundColour(fg);
  window->SetBackgroundColour(bg);
  window->Refresh();
}

void applyColourToAll(std::initializer_list<wxWindow*> windows,
                      const wxColour& fg,
                      const wxColour& bg) {
  for (wxWindow* window : windows) {
    applyColour(window, fg, bg);
  }
}

wxColour getStatusColour(ReactionStatus status, bool dark) {
  switch (status) {
    case ReactionStatus::Safe:
      return dark ? wxColour(52, 180, 97)  : wxColour(25, 120, 55);
    case ReactionStatus::Warning:
      return dark ? wxColour(237, 167, 41) : wxColour(155, 95, 0);
    case ReactionStatus::Dangerous:
      return dark ? wxColour(214, 68, 68)  : wxColour(175, 35, 35);
    default:
      return dark ? wxColour(140, 160, 190): wxColour(80, 100, 130);
  }
}

template <typename UiState>
void updateCanvasAtoms(AtomCanvas* canvas, const UiState& uiState) {
  if (!canvas) return;

  canvas->setAtoms(
    uiState.input.element1,
    uiState.input.count1,
    uiState.input.element2,
    uiState.input.count2
  );
}


} // namespace

AtomFrame::AtomFrame(const wxString& title)
  : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 620)),
    reactionController_(uiState_) {
  SetMinSize(wxSize(800, 560));

  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

  // Toolbar layout
  toolbar_ = new wxPanel(this);
  wxBoxSizer* tbSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* speedGroup = new wxBoxSizer(wxHORIZONTAL);

  speedLabel_ = new wxStaticText(toolbar_, wxID_ANY, wxT("Geschwindigkeit:"));
  speedSlider_ = new wxSlider(toolbar_, wxID_ANY, 3, 1, 5,
      wxDefaultPosition, wxSize(150, -1));
  pauseBtn_ = new wxButton(toolbar_, wxID_ANY,
      wxT("Pause"), wxDefaultPosition, wxSize(88, -1));
  themeBtn_ = new wxButton(toolbar_, wxID_ANY,
      wxT("Tag/Nacht"), wxDefaultPosition, wxSize(96, -1));

  speedGroup->Add(speedLabel_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 8);
  speedGroup->Add(speedSlider_, 0, wxALIGN_CENTER_VERTICAL);

  tbSizer->Add(speedGroup, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
  tbSizer->Add(pauseBtn_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
  tbSizer->AddStretchSpacer(1);
  tbSizer->Add(themeBtn_, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);

  toolbar_->SetSizer(tbSizer);
  mainSizer->Add(toolbar_, 0, wxEXPAND | wxGROW | wxLEFT | wxRIGHT | wxTOP, 6);

  // Sidebar layout
  wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

  sidebar_ = new GuiSidebar(this);
  sidebar_->SetMinSize(wxSize(160, -1));
  sidebar_->Bind(wxEVT_LISTBOX, [this](wxCommandEvent&) { applySidebarSelection(); });

  contentSizer->Add(sidebar_, 0, wxEXPAND);
  contentSizer->Add(rightSizer, 1, wxEXPAND);
  mainSizer->Add(contentSizer, 1, wxEXPAND | wxGROW | wxALL, 6);

  // Selection row
  wxBoxSizer* selectionRow = new wxBoxSizer(wxHORIZONTAL);
  selectionText_ = new wxStaticText(this, wxID_ANY, "Reaktanten:");
  selectionRow->Add(selectionText_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 16);

  auto addReactantControls = [&](wxStaticText*& elementText,
                                 wxButton*& minusBtn,
                                 wxStaticText*& valueText,
                                 wxButton*& plusBtn,
                                 const wxString& defaultLabel,
                                 int rightPaddingAfterPlus) {
    elementText = new wxStaticText(this, wxID_ANY, defaultLabel);
    minusBtn = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(40, 40));
    valueText = new wxStaticText(this, wxID_ANY, "0",
        wxDefaultPosition, wxSize(34, -1), wxALIGN_CENTRE_HORIZONTAL);
    plusBtn = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(40, 40));

    selectionRow->Add(elementText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 8);
    selectionRow->Add(minusBtn, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 6);
    selectionRow->Add(valueText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 6);
    selectionRow->Add(plusBtn, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, rightPaddingAfterPlus);
  };

  addReactantControls(element1Text_, count1MinusBtn_, count1ValueText_,
                      count1PlusBtn_, "Element 1", 12);

  plusLabel_ = new wxStaticText(this, wxID_ANY, "+");
  selectionRow->Add(plusLabel_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 16);

  addReactantControls(element2Text_, count2MinusBtn_, count2ValueText_,
                      count2PlusBtn_, "Element 2", 0);

  for (wxWindow* label : { selectionText_, element1Text_, element2Text_ }) {
    setBold(label, 2);
  }

  setBold(plusLabel_, 4);

  for (wxWindow* button : { count1MinusBtn_, count1PlusBtn_,
                            count2MinusBtn_, count2PlusBtn_ }) {
    setBold(button, 4);
  }

  for (wxWindow* valueText : { count1ValueText_, count2ValueText_ }) {
    setBold(valueText, 4);
  }

  // Keep internal reaction state in sync with visible default counters.
  reactionController_.setCount1(0);
  reactionController_.setCount2(0);
  updateCountTexts();

  rightSizer->Add(selectionRow, 0, wxALL, 12);
  updateSelectionText();

  SetSizer(mainSizer);

  // Events
  themeBtn_->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    reactionController_.toggleDarkMode();
    canvas_->setDunkelModus(uiState_.darkMode);
    sidebar_->setDarkMode(uiState_.darkMode);
    applyTheme();
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

  count1MinusBtn_->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { adjustCount1(-1); });
  count1PlusBtn_->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { adjustCount1(+1); });
  count2MinusBtn_->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { adjustCount2(-1); });
  count2PlusBtn_->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { adjustCount2(+1); });

  // Main field with animation
  canvas_ = new AtomCanvas(this);
  updateCanvasAtoms(canvas_, uiState_);
  rightSizer->Add(canvas_, 1, wxEXPAND);

  // Result row
  resultFormulaText_ = new wxStaticText(this, wxID_ANY, "Formel: -");
  resultNameText_ = new wxStaticText(this, wxID_ANY, "Name: -");
  resultStatusText_ = new wxStaticText(this, wxID_ANY, "Status: -");
  resultHintText_ = new wxStaticText(this, wxID_ANY, "Hinweis: -");
  resultHintText_->Wrap(760);

  setBold(resultFormulaText_);
  setBold(resultStatusText_);

  for (wxStaticText* text : { resultFormulaText_, resultNameText_,
                              resultStatusText_, resultHintText_ }) {
    rightSizer->Add(text, 0, wxALL, 5);
  }

  applyTheme();
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
  if (!selectionText_) return;

  auto setElementLabel = [](wxStaticText* text,
                            const std::string& element,
                            const wxString& fallback) {
    if (!text) return;

    const wxString label = element.empty()
      ? fallback
      : wxString::FromUTF8(getElementDisplayName(element));
    text->SetLabel(label);
  };

  setElementLabel(element1Text_, uiState_.input.element1, "Element 1");
  setElementLabel(element2Text_, uiState_.input.element2, "Element 2");

  Layout();
}

void AtomFrame::updateReactionResult(const ReactionResult& result) {
  if (!resultFormulaText_ || !resultNameText_ || !resultStatusText_ || !resultHintText_) return;

  resultFormulaText_->SetLabel("Formel: " + result.formula);
  resultNameText_->SetLabel("Name: " + result.name);
  resultStatusText_->SetLabel("Status: " + result.statusText);
  resultHintText_->SetLabel("Hinweis: " + result.hint);

  resultStatusText_->SetForegroundColour(getStatusColour(result.status, uiState_.darkMode));
  resultStatusText_->Refresh();

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
      updateCountTexts();
    }
    hasFirstSelection_ = true;
    hasSecondSelection_ = false;
  } else {
    reactionController_.setElement2(element);
    if (uiState_.input.count2 <= 0) {
      reactionController_.setCount2(1);
      updateCountTexts();
    }
    hasSecondSelection_ = true;
  }

  updateSelectionText();
  updateCanvasAtoms(canvas_, uiState_);

  const ReactionResult& result = reactionController_.recompute();
  updateReactionResult(result);
  sidebar_->addHistoryEntry(result.formula, result.name);
}

void AtomFrame::applyAtomCounts() {
  reactionController_.setCount1(std::clamp(uiState_.input.count1, 0, 4));
  reactionController_.setCount2(std::clamp(uiState_.input.count2, 0, 4));
  updateCountTexts();
  updateSelectionText();
  updateCanvasAtoms(canvas_, uiState_);

  const ReactionResult& result = reactionController_.recompute();
  updateReactionResult(result);
}

void AtomFrame::adjustCount1(int delta) {
  const int next = std::clamp(uiState_.input.count1 + delta, 0, 4);
  if (next == uiState_.input.count1) return;
  reactionController_.setCount1(next);
  applyAtomCounts();
}

void AtomFrame::adjustCount2(int delta) {
  const int next = std::clamp(uiState_.input.count2 + delta, 0, 4);
  if (next == uiState_.input.count2) return;
  reactionController_.setCount2(next);
  applyAtomCounts();
}

void AtomFrame::updateCountTexts() {
  if (count1ValueText_) {
    count1ValueText_->SetLabel(wxString::Format("%d", uiState_.input.count1));
  }
  if (count2ValueText_) {
    count2ValueText_->SetLabel(wxString::Format("%d", uiState_.input.count2));
  }
}

void AtomFrame::applyTheme() {
  const bool dark = uiState_.darkMode;
  const wxColour bg      = dark ? wxColour(22, 27, 34)    : wxColour(255, 255, 255);
  const wxColour fg      = dark ? wxColour(230, 237, 243) : wxColour(30, 30, 30);
  const wxColour tbBg    = dark ? wxColour(30, 36, 45)    : wxColour(255, 255, 255);
  const wxColour btnBg   = dark ? wxColour(54, 64, 78)    : wxColour(255, 255, 255);
  const wxColour subFg   = dark ? wxColour(150, 168, 195) : wxColour(68, 82, 106);
  const wxColour valueBg = dark ? wxColour(42, 58, 82)    : wxColour(255, 255, 255);

  SetBackgroundColour(bg);

  applyColourToAll({ toolbar_, speedLabel_ }, fg, tbBg);
  applyColourToAll({ pauseBtn_, themeBtn_,
                     count1MinusBtn_, count1PlusBtn_,
                     count2MinusBtn_, count2PlusBtn_ }, fg, btnBg);
  applyColourToAll({ plusLabel_, element1Text_, element2Text_,
                     resultFormulaText_, resultNameText_ }, fg, bg);
  applyColourToAll({ selectionText_, resultStatusText_, resultHintText_ }, subFg, bg);
  applyColourToAll({ count1ValueText_, count2ValueText_ }, fg, valueBg);

  Refresh();
  Update();
}

} // namespace mixlab