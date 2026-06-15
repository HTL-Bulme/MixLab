#include "gui_dialogs.hpp"
#include "molecule_data.hpp"
#include <wx/wx.h>
#include <wx/filedlg.h>
#include <fstream>

namespace mixlab {

static wxString makeMessageText(const std::vector<std::string>& lines) {
    wxString result;
    for (const auto& line : lines) {
        result += wxString::FromUTF8(line) + wxT("\n");
    }
    return result;
}

void showAboutDialog() {
    wxString text = wxT("Atom Mixer v1.0\n");
    text += wxT("School project — C++ / wxWidgets\n\n");
    text += wxT("Authors: [Name 1], [Name 2]\n");
    text += wxT("License: MIT\n\n");
    text += wxString::Format(wxT("Known reactions in database: %zu\n"), getMoleculeDatabase().size());
    text += wxString::Format(wxT("Elements: %zu"), getElementData().size());
    wxMessageBox(text, wxT("About Application"), wxICON_INFORMATION | wxOK);
}

void showHelpDialog() {
    wxString text = wxT("1. Select two elements in the sidebar.\n");
    text += wxT("2. Adjust atom counts using the spin controls.\n");
    text += wxT("3. Watch the animation and review the result panel.\n\n");
    text += wxT("Green = safe ✓\n");
    text += wxT("Orange = caution ⚠\n");
    text += wxT("Red = dangerous ☠");
    wxMessageBox(text, wxT("User Manual"), wxICON_INFORMATION | wxOK);
}

void showValenceDialog() {
    wxString text;
    for (const auto& element : getElementData()) {
        text += wxString::FromUTF8(element.symbol) + wxT(": ")
            + wxString::Format(wxT("%d\n"), element.valence);
    }
    wxMessageBox(text, wxT("Valence Overview"), wxICON_INFORMATION | wxOK);
}

void showElementTableDialog() {
    wxString text;
    for (const auto& element : getElementData()) {
        text += wxString::FromUTF8(element.symbol) + wxT(" — ")
            + wxString::FromUTF8(element.name) + wxT(" (Valence ")
            + wxString::Format(wxT("%d"), element.valence) + wxT(")\n");
    }
    wxMessageBox(text, wxT("Element Table"), wxICON_INFORMATION | wxOK);
}

void showAllReactionsDialog() {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        lines.push_back(info.formula + " — " + info.name);
    }
    wxMessageBox(makeMessageText(lines), wxT("All Reactions"), wxICON_INFORMATION | wxOK);
}

void showDangerousReactionsDialog() {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        if (info.status == ReactionStatus::Dangerous) {
            lines.push_back(std::string(info.formula) + " — " + info.name + " — " + info.statusText);
        }
    }
    if (lines.empty()) {
        lines.push_back("No dangerous reactions found.");
    }
    wxMessageBox(makeMessageText(lines), wxT("Dangerous Reactions"), wxICON_WARNING | wxOK);
}

void showSafeReactionsDialog() {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        if (info.status == ReactionStatus::Safe) {
            lines.push_back(std::string(info.formula) + " — " + info.name);
        }
    }
    if (lines.empty()) {
        lines.push_back("No safe reactions found.");
    }
    wxMessageBox(makeMessageText(lines), wxT("Safe Reactions"), wxICON_INFORMATION | wxOK);
}

void showSettingsDialog() {
    wxString text = wxT("Use the speed slider to control animation speed.\n\n");
    text += wxT("Increase the speed for faster reaction animation.\n");
    text += wxT("Reduce the number of atoms for a calmer display.");
    wxMessageBox(text, wxT("Animation Settings"), wxICON_INFORMATION | wxOK);
}

void showSaveReactionDialog(UiState& state) {
    const ReactionResult& current = state.currentResult;
    if (!current.known || current.formula.empty()) {
        wxMessageBox(wxT("There is no valid reaction to save."), wxT("Save Reaction"), wxICON_INFORMATION | wxOK);
        return;
    }

    for (const auto& saved : state.savedReactions) {
        if (saved.formula == current.formula && saved.name == current.name) {
            wxMessageBox(wxT("This reaction is already saved."), wxT("Save Reaction"), wxICON_INFORMATION | wxOK);
            return;
        }
    }

    state.savedReactions.push_back(current);
    state.savedCount = static_cast<int>(state.savedReactions.size());
    wxMessageBox(wxString::Format(wxT("Reaction saved. Total saved reactions: %d."), state.savedCount),
                 wxT("Save Reaction"), wxICON_INFORMATION | wxOK);
}

void showSavedReactionsDialog(const UiState& state) {
    if (state.savedReactions.empty()) {
        wxMessageBox(wxT("No saved reactions yet."), wxT("Saved Reactions"), wxICON_INFORMATION | wxOK);
        return;
    }

    std::vector<std::string> lines;
    for (const auto& saved : state.savedReactions) {
        lines.push_back(std::string(saved.formula) + " — " + saved.name + " (" + saved.statusText + ")");
    }
    wxMessageBox(makeMessageText(lines), wxT("Saved Reactions"), wxICON_INFORMATION | wxOK);
}

void showExportDialog(const ReactionResult& currentResult) {
    if (!currentResult.known || currentResult.formula.empty()) {
        wxMessageBox(wxT("There is no valid reaction to export."), wxT("Export Reaction"), wxICON_INFORMATION | wxOK);
        return;
    }

    wxFileDialog dialog(nullptr,
                        wxT("Export Reaction as Text"),
                        wxEmptyString,
                        wxString::FromUTF8((currentResult.formula.empty() ? "reaction" : currentResult.formula) + ".txt"),
                        wxT("Text files (*.txt)|*.txt"),
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (dialog.ShowModal() != wxID_OK) {
        return;
    }

    const std::string path = std::string(dialog.GetPath().ToUTF8());
    std::ofstream output(path);
    if (!output) {
        wxMessageBox(wxT("Could not write the export file."), wxT("Export Reaction"), wxICON_ERROR | wxOK);
        return;
    }

    output << "Formula: " << currentResult.formula << "\n";
    output << "Name: " << currentResult.name << "\n";
    output << "Status: " << currentResult.statusText << "\n";
    output << "Note: " << currentResult.hint << "\n";

    if (!output) {
        wxMessageBox(wxT("Failed to save the export file."), wxT("Export Reaction"), wxICON_ERROR | wxOK);
        return;
    }

    wxMessageBox(wxT("Reaction exported successfully."), wxT("Export Reaction"), wxICON_INFORMATION | wxOK);
}

} // namespace mixlab
