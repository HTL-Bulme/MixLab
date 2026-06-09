#ifndef ATOM_FRAME_HPP
#define ATOM_FRAME_HPP
#include "reaction_controller.hpp"
#include "ui_state.hpp"
#include "gui_sidebar.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace mixlab {

class AtomCanvas; // forward declaration

class AtomFrame : public wxFrame {
public:
    AtomFrame(const wxString& title);

private:
    AtomCanvas* canvas_ = nullptr;
    GuiSidebar* sidebar_ = nullptr;

    wxSlider* speedSlider_ = nullptr;
    wxSpinCtrl* count1_ = nullptr;
    wxSpinCtrl* count2_ = nullptr;
    wxStaticText* element1Text_ = nullptr;
    wxStaticText* element2Text_ = nullptr;
    UiState uiState_;
    ReactionController reactionController_;

    wxStaticText* selectionText_ = nullptr;
    void updateSelectionText();

    wxStaticText* resultFormulaText_ = nullptr;
    wxStaticText* resultNameText_ = nullptr;
    wxStaticText* resultStatusText_ = nullptr;
    wxStaticText* resultHintText_ = nullptr;
    void updateReactionResult(const ReactionResult& result);

    bool hasFirstSelection_ = false;
    bool hasSecondSelection_ = false;
    void applySidebarSelection();
    void applyAtomCounts();
};

} // namespace mixlab

#endif // ATOM_FRAME_HPP
