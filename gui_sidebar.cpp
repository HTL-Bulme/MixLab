#include "gui_sidebar.hpp"
#include "language.hpp"
#include <wx/sizer.h>

namespace mixlab {

struct SidebarElementInfo {
    std::string symbol;
    std::string englishLabel;
    std::string germanLabel;
};

static const std::vector<SidebarElementInfo> kElements = {
    {"H",  "H   Hydrogen   Val. 1", "H   Wasserstoff   Wertigkeit 1"},
    {"O",  "O   Oxygen     Val. 2", "O   Sauerstoff   Wertigkeit 2"},
    {"Na", "Na  Sodium     Val. 1", "Na  Natrium     Wertigkeit 1"},
    {"Cl", "Cl  Chlorine   Val. 1", "Cl  Chlor      Wertigkeit 1"},
    {"Fe", "Fe  Iron       Val. 3", "Fe  Eisen      Wertigkeit 3"},
    {"C",  "C   Carbon     Val. 4", "C   Kohlenstoff Wertigkeit 4"},
    {"N",  "N   Nitrogen   Val. 3", "N   Stickstoff  Wertigkeit 3"},
    {"S",  "S   Sulfur     Val. 2", "S   Schwefel   Wertigkeit 2"},
};

wxBEGIN_EVENT_TABLE(GuiSidebar, wxPanel)
    EVT_LISTBOX(wxID_ANY, GuiSidebar::onElementSelected)
wxEND_EVENT_TABLE()

GuiSidebar::GuiSidebar(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) {
    buildLayout();
    setLanguage(language_);
}

void GuiSidebar::buildLayout() {
    auto* sizer = new wxBoxSizer(wxVERTICAL);

    elementsLabel_ = new wxStaticText(this, wxID_ANY, "ELEMENTS");
    sizer->Add(elementsLabel_, 0, wxLEFT | wxTOP, 10);

    elementList_ = new wxListBox(this, wxID_ANY);
    for (const auto& element : kElements) {
        elementList_->Append(element.englishLabel);
    }
    sizer->Add(elementList_, 1, wxEXPAND | static_cast<int>(wxALL), 6);

    historyLabel_ = new wxStaticText(this, wxID_ANY, "HISTORY");
    sizer->Add(historyLabel_, 0, wxLEFT | wxTOP, 10);

    historyList_ = new wxListBox(this, wxID_ANY);
    sizer->Add(historyList_, 1, wxEXPAND | static_cast<int>(wxALL), 6);

    SetSizer(sizer);
}

void GuiSidebar::onElementSelected(wxCommandEvent& event) {
    // Parent frame can bind to this panel's list directly if needed
    event.Skip();
}

std::string GuiSidebar::getSelectedElement() const {
    int idx = elementList_->GetSelection();
    if (idx == wxNOT_FOUND) return "";
    return kElements[static_cast<size_t>(idx)].symbol;
}

void GuiSidebar::setLanguage(Language language) {
    language_ = language;
    if (elementsLabel_) {
        elementsLabel_->SetLabel(localize(language_, "ELEMENTS", "ELEMENTE"));
    }
    if (historyLabel_) {
        historyLabel_->SetLabel(localize(language_, "HISTORY", "VERLAUF"));
    }

    if (elementList_) {
        elementList_->Clear();
        for (const auto& element : kElements) {
            const std::string label = (language_ == Language::German)
                ? element.germanLabel
                : element.englishLabel;
            elementList_->Append(label);
        }
    }
    Layout();
}

void GuiSidebar::addHistoryEntry(const std::string& formula,
                                  const std::string& name) {
    std::string entry = formula + "  " + name;
    historyList_->Append(entry);
    // Scroll to the latest entry
    historyList_->SetFirstItem(
        static_cast<int>(historyList_->GetCount()) - 1
    );
}

void GuiSidebar::setDarkMode(bool dark) {
    darkMode_ = dark;
    wxColour bg  = dark ? wxColour(22, 27, 34)   : wxColour(248, 246, 240);
    wxColour fg  = dark ? wxColour(230, 237, 243) : wxColour(30, 30, 30);
    SetBackgroundColour(bg);
    SetForegroundColour(fg);
    elementList_->SetBackgroundColour(bg);
    elementList_->SetForegroundColour(fg);
    historyList_->SetBackgroundColour(bg);
    historyList_->SetForegroundColour(fg);
    elementsLabel_->SetForegroundColour(fg);
    historyLabel_->SetForegroundColour(fg);
    Refresh();
}

} // namespace mixlab