#include "gui_dialogs.hpp"
#include "molecule_data.hpp"
#include "language.hpp"
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

void showAboutDialog(Language language) {
    wxString text = localize(language, "Atom Mixer v1.0\n", "Atom Mixer v1.0\n");
    text += localize(language, "School project — C++ / wxWidgets\n\n", "Schulprojekt — C++ / wxWidgets\n\n");
    text += localize(language, "Authors: [Name 1], [Name 2]\n", "Autoren: [Name 1], [Name 2]\n");
    text += localize(language, "License: MIT\n\n", "Lizenz: MIT\n\n");
    text += wxString::Format(localize(language, "Known reactions in database: %zu\n", "Bekannte Reaktionen in der Datenbank: %zu\n"), getMoleculeDatabase().size());
    text += wxString::Format(localize(language, "Elements: %zu", "Elemente: %zu"), getElementData().size());
    wxMessageBox(text, localize(language, "About Application", "Über die Anwendung"), wxICON_INFORMATION | wxOK);
}

void showHelpDialog(Language language) {
    wxString text = localize(language, "1. Select two elements in the sidebar.\n", "1. Wählen Sie zwei Elemente in der Seitenleiste aus.\n");
    text += localize(language, "2. Adjust atom counts using the spin controls.\n", "2. Passen Sie die Atomzahlen mit den Steuerelementen an.\n");
    text += localize(language, "3. Watch the animation and review the result panel.\n\n", "3. Beobachten Sie die Animation und prüfen Sie das Ergebnisfeld.\n\n");
    text += localize(language, "Green = safe ✓\n", "Grün = ungefährlich ✓\n");
    text += localize(language, "Orange = caution ⚠\n", "Orange = Vorsicht ⚠\n");
    text += localize(language, "Red = dangerous ☠", "Rot = gefährlich ☠");
    wxMessageBox(text, localize(language, "User Manual", "Handbuch"), wxICON_INFORMATION | wxOK);
}

void showValenceDialog(Language language) {
    wxString text;
    for (const auto& element : getElementData()) {
        text += wxString::FromUTF8(element.symbol) + wxT(": ")
            + wxString::Format(wxT("%d\n"), element.valence);
    }
    wxMessageBox(text, localize(language, "Valence Overview", "Valenzübersicht"), wxICON_INFORMATION | wxOK);
}

void showElementTableDialog(Language language) {
    wxString text;
    for (const auto& element : getElementData()) {
        const std::string elementName = translateElementName(element.name, language);
        text += wxString::FromUTF8(element.symbol) + wxT(" — ")
            + wxString::FromUTF8(elementName) + wxT(" (Valence ")
            + wxString::Format(wxT("%d"), element.valence) + wxT(")\n");
    }
    wxMessageBox(text, localize(language, "Element Table", "Elementtabelle"), wxICON_INFORMATION | wxOK);
}

void showAllReactionsDialog(Language language) {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        lines.push_back(info.formula + " — " + translateCompoundName(info.name, language));
    }
    wxMessageBox(makeMessageText(lines), localize(language, "All Reactions", "Alle Reaktionen"), wxICON_INFORMATION | wxOK);
}

void showDangerousReactionsDialog(Language language) {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        if (info.status == ReactionStatus::Dangerous) {
            lines.push_back(std::string(info.formula) + " — "
                + translateCompoundName(info.name, language) + " — "
                + std::string(getStatusText(info.status, language).ToUTF8()));
        }
    }
    if (lines.empty()) {
        lines.push_back(localizeStd(language, "No dangerous reactions found.", "Keine gefährlichen Reaktionen gefunden."));
    }
    wxMessageBox(makeMessageText(lines), localize(language, "Dangerous Reactions", "Gefährliche Reaktionen"), wxICON_WARNING | wxOK);
}

void showSafeReactionsDialog(Language language) {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        if (info.status == ReactionStatus::Safe) {
            lines.push_back(std::string(info.formula) + " — " + translateCompoundName(info.name, language));
        }
    }
    if (lines.empty()) {
        lines.push_back(localizeStd(language, "No safe reactions found.", "Keine ungefährlichen Reaktionen gefunden."));
    }
    wxMessageBox(makeMessageText(lines), localize(language, "Safe Reactions", "Ungefährliche Reaktionen"), wxICON_INFORMATION | wxOK);
}

void showSettingsDialog(Language language) {
    wxString text = localize(language, "Use the speed slider to control animation speed.\n\n", "Verwenden Sie den Geschwindigkeitsregler, um die Animationsgeschwindigkeit zu steuern.\n\n");
    text += localize(language, "Increase the speed for faster reaction animation.\n", "Erhöhen Sie die Geschwindigkeit für eine schnellere Reaktionsanimation.\n");
    text += localize(language, "Reduce the number of atoms for a calmer display.", "Reduzieren Sie die Anzahl der Atome für eine ruhigere Darstellung.");
    wxMessageBox(text, localize(language, "Animation Settings", "Animationseinstellungen"), wxICON_INFORMATION | wxOK);
}

void showSaveReactionDialog(UiState& state, Language language) {
    const ReactionResult& current = state.currentResult;
    if (!current.known || current.formula.empty()) {
        wxMessageBox(localize(language, "There is no valid reaction to save.", "Es gibt keine gültige Reaktion zum Speichern."),
                     localize(language, "Save Reaction", "Reaktion speichern"),
                     wxICON_INFORMATION | wxOK);
        return;
    }

    for (const auto& saved : state.savedReactions) {
        if (saved.formula == current.formula && saved.name == current.name) {
            wxMessageBox(localize(language, "This reaction is already saved.", "Diese Reaktion ist bereits gespeichert."),
                         localize(language, "Save Reaction", "Reaktion speichern"),
                         wxICON_INFORMATION | wxOK);
            return;
        }
    }

    state.savedReactions.push_back(current);
    state.savedCount = static_cast<int>(state.savedReactions.size());
    wxMessageBox(wxString::Format(localize(language, "Reaction saved. Total saved reactions: %d.", "Reaktion gespeichert. Insgesamt gespeicherte Reaktionen: %d."),
                 state.savedCount),
                 localize(language, "Save Reaction", "Reaktion speichern"),
                 wxICON_INFORMATION | wxOK);
}

void showSavedReactionsDialog(const UiState& state, Language language) {
    if (state.savedReactions.empty()) {
        wxMessageBox(localize(language, "No saved reactions yet.", "Noch keine gespeicherten Reaktionen."),
                     localize(language, "Saved Reactions", "Gespeicherte Reaktionen"),
                     wxICON_INFORMATION | wxOK);
        return;
    }

    std::vector<std::string> lines;
    for (const auto& saved : state.savedReactions) {
        lines.push_back(std::string(saved.formula) + " — "
            + translateCompoundName(saved.name, language) + " ("
            + std::string(getStatusText(saved.status, language).ToUTF8()) + ")");
    }
    wxMessageBox(makeMessageText(lines), localize(language, "Saved Reactions", "Gespeicherte Reaktionen"), wxICON_INFORMATION | wxOK);
}

void showExportDialog(const ReactionResult& currentResult, Language language) {
    if (!currentResult.known || currentResult.formula.empty()) {
        wxMessageBox(localize(language, "There is no valid reaction to export.", "Es gibt keine gültige Reaktion zum Exportieren."),
                     localize(language, "Export Reaction", "Reaktion exportieren"),
                     wxICON_INFORMATION | wxOK);
        return;
    }

    wxFileDialog dialog(nullptr,
                        localize(language, "Export Reaction as Text", "Reaktion als Text exportieren"),
                        wxEmptyString,
                        wxString::FromUTF8((currentResult.formula.empty() ? "reaction" : currentResult.formula) + ".txt"),
                        localize(language, "Text files (*.txt)|*.txt", "Textdateien (*.txt)|*.txt"),
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (dialog.ShowModal() != wxID_OK) {
        return;
    }

    const std::string path = std::string(dialog.GetPath().ToUTF8());
    std::ofstream output(path);
    if (!output) {
        wxMessageBox(localize(language, "Could not write the export file.", "Die Exportdatei konnte nicht geschrieben werden."),
                     localize(language, "Export Reaction", "Reaktion exportieren"),
                     wxICON_ERROR | wxOK);
        return;
    }

    output << localizeStd(language, "Formula: ", "Formel: ") << currentResult.formula << "\n";
    output << localizeStd(language, "Name: ", "Name: ") << currentResult.name << "\n";
    output << localizeStd(language, "Status: ", "Status: ") << std::string(getStatusText(currentResult.status, language).ToUTF8()) << "\n";
    output << localizeStd(language, "Note: ", "Hinweis: ") << translateHint(currentResult.hint, language) << "\n";

    if (!output) {
        wxMessageBox(localize(language, "Failed to save the export file.", "Die Exportdatei konnte nicht gespeichert werden."),
                     localize(language, "Export Reaction", "Reaktion exportieren"),
                     wxICON_ERROR | wxOK);
        return;
    }

    wxMessageBox(localize(language, "Reaction exported successfully.", "Reaktion erfolgreich exportiert."),
                 localize(language, "Export Reaction", "Reaktion exportieren"),
                 wxICON_INFORMATION | wxOK);
}

} // namespace mixlab
