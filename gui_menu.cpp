#include "gui_menu.hpp"
#include <wx/wx.h>

namespace mixlab {

wxMenuBar* createMenuBar(Language language) {
    wxMenuBar* menuBar = new wxMenuBar;

    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_Menu_SaveReaction, localize(language, "Save Reaction\tCtrl+S", "Reaktion speichern\tCtrl+S"));
    fileMenu->Append(ID_Menu_OpenSaved, localize(language, "Saved Reactions...", "Gespeicherte Reaktionen..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_Menu_ExportText, localize(language, "Export Reaction as TXT", "Reaktion als TXT exportieren"));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_Menu_Reset, localize(language, "Reset All", "Alles zurücksetzen"));
    menuBar->Append(fileMenu, localize(language, "File", "Datei"));

    wxMenu* reactionsMenu = new wxMenu;
    reactionsMenu->Append(ID_Menu_AllReactions, localize(language, "All Reactions\tCtrl+R", "Alle Reaktionen\tCtrl+R"));
    reactionsMenu->Append(ID_Menu_DangerousReactions, localize(language, "Dangerous", "Gefährlich"));
    reactionsMenu->Append(ID_Menu_SafeReactions, localize(language, "Safe", "Ungefährlich"));
    reactionsMenu->AppendSeparator();
    reactionsMenu->Append(ID_Menu_ElementTable, localize(language, "Element Table", "Elementtabelle"));
    menuBar->Append(reactionsMenu, localize(language, "Reactions", "Reaktionen"));

    wxMenu* settingsMenu = new wxMenu;
    settingsMenu->Append(ID_Menu_ToggleTheme, localize(language, "Dark / Light Theme", "Dunkel / Hell Theme"));
    settingsMenu->Append(ID_Menu_Settings, localize(language, "Animation Settings", "Animationseinstellungen"));
    settingsMenu->AppendSeparator();
    settingsMenu->Append(ID_Menu_Language, menuLanguageLabel(language));
    menuBar->Append(settingsMenu, localize(language, "Settings", "Einstellungen"));

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(ID_Menu_Help, localize(language, "Manual", "Handbuch"));
    helpMenu->Append(ID_Menu_Valence, localize(language, "About Valence", "Über Valenz"));
    helpMenu->AppendSeparator();
    helpMenu->Append(ID_Menu_About, localize(language, "About Program...", "Über das Programm..."));
    menuBar->Append(helpMenu, localize(language, "Help", "Hilfe"));

    return menuBar;
}

} // namespace mixlab
