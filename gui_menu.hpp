#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <wx/wx.h>

namespace mixlab {

enum MenuId {
    ID_Menu_SaveReaction = 30000,
    ID_Menu_OpenSaved,
    ID_Menu_ExportText,
    ID_Menu_Reset,
    ID_Menu_AllReactions,
    ID_Menu_DangerousReactions,
    ID_Menu_SafeReactions,
    ID_Menu_ElementTable,
    ID_Menu_ToggleTheme,
    ID_Menu_Settings,
    ID_Menu_Language,
    ID_Menu_Help,
    ID_Menu_Valence,
    ID_Menu_About,
};

wxMenuBar* createMenuBar();

} // namespace mixlab

#endif // GUI_MENU_HPP
