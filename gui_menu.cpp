#include "gui_menu.hpp"
#include <wx/wx.h>

namespace mixlab {

wxMenuBar* createMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar;

    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_Menu_SaveReaction, "Save Reaction\tCtrl+S");
    fileMenu->Append(ID_Menu_OpenSaved, "Saved Reactions...");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_Menu_ExportText, "Export Reaction as TXT");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_Menu_Reset, "Reset All");
    menuBar->Append(fileMenu, "File");

    wxMenu* reactionsMenu = new wxMenu;
    reactionsMenu->Append(ID_Menu_AllReactions, "All Reactions\tCtrl+R");
    reactionsMenu->Append(ID_Menu_DangerousReactions, "Dangerous");
    reactionsMenu->Append(ID_Menu_SafeReactions, "Safe");
    reactionsMenu->AppendSeparator();
    reactionsMenu->Append(ID_Menu_ElementTable, "Element Table");
    menuBar->Append(reactionsMenu, "Reactions");

    wxMenu* settingsMenu = new wxMenu;
    settingsMenu->Append(ID_Menu_ToggleTheme, "Dark / Light Theme");
    settingsMenu->Append(ID_Menu_Settings, "Animation Settings");
    settingsMenu->AppendSeparator();
    settingsMenu->Append(ID_Menu_Language, "Language: English");
    menuBar->Append(settingsMenu, "Settings");

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(ID_Menu_Help, "Manual");
    helpMenu->Append(ID_Menu_Valence, "About Valence");
    helpMenu->AppendSeparator();
    helpMenu->Append(ID_Menu_About, "About Program...");
    menuBar->Append(helpMenu, "Help");

    return menuBar;
}

} // namespace mixlab
