#include "atom_frame.hpp"
#include "atom_canvas.hpp"
#include "gui_sidebar.hpp"
#include "gui_menu.hpp"
#include "gui_dialogs.hpp"
#include "language.hpp"

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

  // Create and set menu bar
  menuBar_ = createMenuBar(uiState_.language);
  SetMenuBar(menuBar_);

  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

  // Toolbar layout
  toolbar_ = new wxPanel(this);
  wxBoxSizer* tbSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* speedGroup = new wxBoxSizer(wxHORIZONTAL);

  speedLabel_ = new wxStaticText(toolbar_, wxID_ANY, localize(uiState_.language, "Speed:", "Geschwindigkeit:"));
  speedSlider_ = new wxSlider(toolbar_, wxID_ANY, 3, 1, 5,
      wxDefaultPosition, wxSize(150, -1));
  pauseBtn_ = new wxButton(toolbar_, wxID_ANY,
      localize(uiState_.language, "Pause", "Pause"), wxDefaultPosition, wxSize(88, -1));
  themeBtn_ = new wxButton(toolbar_, wxID_ANY,
      localize(uiState_.language, "Light/Dark", "Hell/Dunkel"), wxDefaultPosition, wxSize(96, -1));
  langBtn_ = new wxButton(toolbar_, wxID_ANY,
      localize(uiState_.language, "En/De", "En/De"), wxDefaultPosition, wxSize(64, -1));

  speedGroup->Add(speedLabel_, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), 8);
  speedGroup->Add(speedSlider_, 0, wxALIGN_CENTER_VERTICAL);

  tbSizer->Add(speedGroup, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxLEFT | wxRIGHT), 10);
  tbSizer->Add(pauseBtn_, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), 10);
  tbSizer->AddStretchSpacer(1);
  tbSizer->Add(themeBtn_, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxLEFT | wxRIGHT), 10);
  tbSizer->Add(langBtn_, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), 10);

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
  selectionText_ = new wxStaticText(this, wxID_ANY, localize(uiState_.language, "Reaction:", "Reaktion:"));
  selectionRow->Add(selectionText_, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), 16);

  auto addReactantControls = [&](wxStaticText*& elementText,
                                 wxButton*& minusBtn,
                                 wxStaticText*& valueText,
                                 wxButton*& plusBtn,
                                 const wxString& defaultLabel,
                                 int rightPaddingAfterPlus) {
    elementText = new wxStaticText(this, wxID_ANY, defaultLabel);
    minusBtn = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(36, 36));
    valueText = new wxStaticText(this, wxID_ANY, "0",
        wxDefaultPosition, wxSize(34, -1), wxALIGN_CENTRE_HORIZONTAL);
    plusBtn = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(36, 36));

    selectionRow->Add(elementText, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), 8);
    selectionRow->Add(minusBtn, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), 6);
    selectionRow->Add(valueText, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), 6);
    selectionRow->Add(plusBtn, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), rightPaddingAfterPlus);
  };

  addReactantControls(element1Text_, count1MinusBtn_, count1ValueText_,
                      count1PlusBtn_, localize(uiState_.language, "Element 1", "Element 1"), 12);

  plusLabel_ = new wxStaticText(this, wxID_ANY, "+");
  selectionRow->Add(plusLabel_, 0, wxALIGN_CENTER_VERTICAL | static_cast<int>(wxRIGHT), 16);

  addReactantControls(element2Text_, count2MinusBtn_, count2ValueText_,
                      count2PlusBtn_, localize(uiState_.language, "Element 2", "Element 2"), 0);

  
  mixBtn_ = new wxButton(this, wxID_ANY, "Mix");
  selectionRow->Add(mixBtn_, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 20);

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
    canvas_->setDarkMode(uiState_.darkMode);
    sidebar_->setDarkMode(uiState_.darkMode);
    applyTheme();
  });

  langBtn_->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    uiState_.language = (uiState_.language == Language::English)
        ? Language::German
        : Language::English;
    updateLanguage();
  });

  speedSlider_->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
    reactionController_.setSpeed(speedSlider_->GetValue());
    canvas_->setSpeed(uiState_.speed);
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
  mixBtn_->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { onMixClicked(); });

  // Menu event handlers for dialogs and actions
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showAboutDialog(uiState_.language); }, mixlab::ID_Menu_About);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showHelpDialog(uiState_.language); }, mixlab::ID_Menu_Help);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showValenceDialog(uiState_.language); }, mixlab::ID_Menu_Valence);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showElementTableDialog(uiState_.language); }, mixlab::ID_Menu_ElementTable);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showAllReactionsDialog(uiState_.language); }, mixlab::ID_Menu_AllReactions);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showDangerousReactionsDialog(uiState_.language); }, mixlab::ID_Menu_DangerousReactions);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showSafeReactionsDialog(uiState_.language); }, mixlab::ID_Menu_SafeReactions);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showSettingsDialog(uiState_.language); }, mixlab::ID_Menu_Settings);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) {
    uiState_.language = (uiState_.language == Language::English)
        ? Language::German
        : Language::English;
    updateLanguage();
  }, mixlab::ID_Menu_Language);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showSaveReactionDialog(uiState_, uiState_.language); }, mixlab::ID_Menu_SaveReaction);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showSavedReactionsDialog(uiState_, uiState_.language); }, mixlab::ID_Menu_OpenSaved);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) { mixlab::showExportDialog(uiState_.currentResult, uiState_.language); }, mixlab::ID_Menu_ExportText);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) {
    reactionController_.toggleDarkMode();
    canvas_->setDarkMode(uiState_.darkMode);
    sidebar_->setDarkMode(uiState_.darkMode);
    applyTheme();
  }, mixlab::ID_Menu_ToggleTheme);
  Bind(wxEVT_MENU, [this](wxCommandEvent&) {
    hasFirstSelection_ = false;
    hasSecondSelection_ = false;
    reactionController_.setElement1("");
    reactionController_.setElement2("");
    reactionController_.setCount1(0);
    reactionController_.setCount2(0);
    updateCountTexts();
    updateSelectionText();
    updateCanvasAtoms(canvas_, uiState_);
    updateReactionResult(reactionController_.recompute());
  }, mixlab::ID_Menu_Reset);

  // Main field with animation
  canvas_ = new AtomCanvas(this);
  updateCanvasAtoms(canvas_, uiState_);
  rightSizer->Add(canvas_, 1, wxEXPAND);

  // Result row
  resultFormulaText_ = new wxStaticText(this, wxID_ANY, localize(uiState_.language, "Formula: -", "Formel: -"));
  resultNameText_ = new wxStaticText(this, wxID_ANY, localize(uiState_.language, "Name: -", "Name: -"));
  resultStatusText_ = new wxStaticText(this, wxID_ANY, localize(uiState_.language, "Status: -", "Status: -"));
  resultHintText_ = new wxStaticText(this, wxID_ANY, localize(uiState_.language, "Note: -", "Hinweis: -"));
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

  auto setElementLabel = [this](wxStaticText* text,
                                const std::string& element,
                                const wxString& fallback) {
    if (!text) return;

    const wxString label = element.empty()
      ? fallback
      : wxString::FromUTF8(translateElementName(getElementDisplayName(element), uiState_.language));
    text->SetLabel(label);
  };

  setElementLabel(element1Text_, uiState_.input.element1,
                  localize(uiState_.language, "Element 1", "Element 1"));
  setElementLabel(element2Text_, uiState_.input.element2,
                  localize(uiState_.language, "Element 2", "Element 2"));

  Layout();
}

void AtomFrame::updateReactionResult(const ReactionResult& result) {
  if (!resultFormulaText_ || !resultNameText_ || !resultStatusText_ || !resultHintText_) return;

  resultFormulaText_->SetLabel(localize(uiState_.language, "Formula: ", "Formel: ") + wxString::FromUTF8(result.formula));
  resultNameText_->SetLabel(localize(uiState_.language, "Name: ", "Name: ") + wxString::FromUTF8(translateCompoundName(result.name, uiState_.language)));
  resultStatusText_->SetLabel(localize(uiState_.language, "Status: ", "Status: ") + getStatusText(result.status, uiState_.language));
  resultHintText_->SetLabel(localize(uiState_.language, "Note: ", "Hinweis: ") + wxString::FromUTF8(translateHint(result.hint, uiState_.language)));

  resultStatusText_->SetForegroundColour(getStatusColour(result.status, uiState_.darkMode));
  resultStatusText_->Refresh();

  if (canvas_) {
    canvas_->setReactionStatus(result.status, getStatusText(result.status, uiState_.language).ToStdString());
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
//  sidebar_->addHistoryEntry(result.formula, result.name); // Only add to history when user clicks "Mix" button
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

void AtomFrame::onMixClicked() {

    const ReactionResult& result = reactionController_.recompute();
    
    updateReactionResult(result);
    updateCanvasAtoms(canvas_, uiState_);

    if (hasFirstSelection_ &&
        hasSecondSelection_ &&
        result.formula != "-" &&
        !result.formula.empty()) {
        sidebar_->addHistoryEntry(result.formula, result.name);
    }
}

void AtomFrame::updateCountTexts() {
  if (count1ValueText_) {
    count1ValueText_->SetLabel(wxString::Format("%d", uiState_.input.count1));
  }
  if (count2ValueText_) {
    count2ValueText_->SetLabel(wxString::Format("%d", uiState_.input.count2));
  }
}

void AtomFrame::updateLanguage() {
  if (menuBar_) {
    SetMenuBar(nullptr);
    delete menuBar_;
    menuBar_ = nullptr;
  }
  menuBar_ = createMenuBar(uiState_.language);
  SetMenuBar(menuBar_);

  if (speedLabel_) {
    speedLabel_->SetLabel(localize(uiState_.language, "Speed:", "Geschwindigkeit:"));
  }
  if (pauseBtn_) {
    pauseBtn_->SetLabel(localize(uiState_.language, "Pause", "Pause"));
  }
  if (themeBtn_) {
    themeBtn_->SetLabel(localize(uiState_.language, "Light/Dark", "Hell/Dunkel"));
  }
  if (langBtn_) {
    langBtn_->SetLabel(localize(uiState_.language, "En/De", "En/De"));
  }
  if (selectionText_) {
    selectionText_->SetLabel(localize(uiState_.language, "Reaction:", "Reaktion:"));
  }
  if (element1Text_) {
    element1Text_->SetLabel(localize(uiState_.language, "Element 1", "Element 1"));
  }
  if (element2Text_) {
    element2Text_->SetLabel(localize(uiState_.language, "Element 2", "Element 2"));
  }
  if (resultFormulaText_) {
    resultFormulaText_->SetLabel(localize(uiState_.language, "Formula: -", "Formel: -"));
  }
  if (resultNameText_) {
    resultNameText_->SetLabel(localize(uiState_.language, "Name: -", "Name: -"));
  }
  if (resultStatusText_) {
    resultStatusText_->SetLabel(localize(uiState_.language, "Status: -", "Status: -"));
  }
  if (resultHintText_) {
    resultHintText_->SetLabel(localize(uiState_.language, "Note: -", "Hinweis: -"));
  }

  if (sidebar_) {
    sidebar_->setLanguage(uiState_.language);
  }

  updateSelectionText();
  const ReactionResult& result = reactionController_.recompute();
  updateReactionResult(result);
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