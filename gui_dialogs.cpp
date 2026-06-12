#include "gui_dialogs.hpp"
#include "molecule_data.hpp"
#include <wx/wx.h>
#include <sstream>

namespace mixlab {

static wxString makeMessageText(const std::vector<std::string>& lines) {
    wxString result;
    for (const auto& line : lines) {
        result += wxString::FromUTF8(line) + wxT("\n");
    }
    return result;
}

void showAboutDialog() {
    wxString text = wxT("Atom-Mixer v1.0\n");
    text += wxT("Schulprojekt — C++ / wxWidgets\n\n");
    text += wxT("Autoren: [Name 1], [Name 2]\n");
    text += wxT("Lizenz: MIT\n\n");
    text += wxString::Format(wxT("Reaktionen in der Datenbank: %zu\n"), getMoleculeDatabase().size());
    text += wxString::Format(wxT("Elemente: %zu"), getElementData().size());
    wxMessageBox(text, wxT("Über das Programm"), wxICON_INFORMATION | wxOK);
}

void showHelpDialog() {
    wxString text = wxT("1. Wählen Sie zwei Elemente in der Seitenleiste aus.\n");
    text += wxT("2. Stellen Sie die Atomanzahl mit den Spin-Feldern ein.\n");
    text += wxT("3. Beobachten Sie die Animation und das Ergebnisfeld.\n\n");
    text += wxT("Grün = ungefährlich ✓\n");
    text += wxT("Orange = Vorsicht ⚠\n");
    text += wxT("Rot = gefährlich ☠");
    wxMessageBox(text, wxT("Bedienungsanleitung"), wxICON_INFORMATION | wxOK);
}

void showValenceDialog() {
    wxString text;
    for (const auto& element : getElementData()) {
        text += wxString::FromUTF8(element.symbol) + wxT(": ")
            + wxString::Format(wxT("%d\n"), element.valence);
    }
    wxMessageBox(text, wxT("Über Valenz"), wxICON_INFORMATION | wxOK);
}

void showElementTableDialog() {
    wxString text;
    for (const auto& element : getElementData()) {
        text += wxString::FromUTF8(element.symbol) + wxT(" — ")
            + wxString::FromUTF8(element.name) + wxT(" (Valenz ")
            + wxString::Format(wxT("%d"), element.valence) + wxT(")\n");
    }
    wxMessageBox(text, wxT("Elementtabelle"), wxICON_INFORMATION | wxOK);
}

static wxString makeReactionSummary(const MoleculeInfo& info) {
    return wxString::FromUTF8(info.formula) + wxT(" — ")
         + wxString::FromUTF8(info.name) + wxT(" (")
         + wxString::FromUTF8(info.statusText) + wxT(")");
}

void showAllReactionsDialog() {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        lines.push_back(info.formula + " — " + info.name);
    }
    wxMessageBox(makeMessageText(lines), wxT("Alle Reaktionen"), wxICON_INFORMATION | wxOK);
}

void showDangerousReactionsDialog() {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        if (info.status == ReactionStatus::Dangerous) {
            lines.push_back(std::string(info.formula) + " — " + info.name + " — " + info.statusText);
        }
    }
    if (lines.empty()) {
        lines.push_back("Keine gefährlichen Reaktionen gefunden.");
    }
    wxMessageBox(makeMessageText(lines), wxT("Gefährliche Reaktionen"), wxICON_WARNING | wxOK);
}

void showSafeReactionsDialog() {
    std::vector<std::string> lines;
    for (const auto& info : getMoleculeDatabase()) {
        if (info.status == ReactionStatus::Safe) {
            lines.push_back(std::string(info.formula) + " — " + info.name);
        }
    }
    if (lines.empty()) {
        lines.push_back("Keine ungefährlichen Reaktionen gefunden.");
    }
    wxMessageBox(makeMessageText(lines), wxT("Ungefährliche Reaktionen"), wxICON_INFORMATION | wxOK);
}

void showSettingsDialog() {
    wxString text = wxT("Geschwindigkeit und Animation werden über den Schieberegler gesteuert.\n\n");
    text += wxT("Erhöhen Sie die Geschwindigkeit für schnellere Reaktionsanimationen.\n");
    text += wxT("Reduzieren Sie die Anzahl der Teilchen, um die Darstellung ruhiger wirken zu lassen.");
    wxMessageBox(text, wxT("Animationseinstellungen"), wxICON_INFORMATION | wxOK);
}

void showSaveReactionDialog() {
    wxMessageBox(wxT("Diese Funktion ist noch nicht implementiert."), wxT("Reaktion speichern"), wxICON_INFORMATION | wxOK);
}

void showSavedReactionsDialog() {
    wxMessageBox(wxT("Noch keine gespeicherten Reaktionen."), wxT("Gespeicherte Reaktionen"), wxICON_INFORMATION | wxOK);
}

void showExportDialog() {
    wxMessageBox(wxT("Export in eine Textdatei wird später unterstützt."), wxT("Export"), wxICON_INFORMATION | wxOK);
}

} // namespace mixlab
