#ifndef ATOM_FRAME_HPP
#define ATOM_FRAME_HPP
#include "reaction_controller.hpp"
#include "ui_state.hpp"
#include "gui_sidebar.hpp"

#include <wx/wx.h>

namespace mixlab {

class AtomCanvas; // forward declaration

class AtomFrame : public wxFrame {
public:
    AtomFrame(const wxString& title);

private:
    AtomCanvas* canvas_ = nullptr;
    GuiSidebar* sidebar_ = nullptr;
    wxMenuBar* menuBar_ = nullptr;

    wxPanel*      toolbar_    = nullptr;
    wxButton*     themeBtn_   = nullptr;
    wxButton*     langBtn_    = nullptr;
    wxStaticText* speedLabel_ = nullptr;
    wxSlider* speedSlider_ = nullptr;
    wxButton* pauseBtn_ = nullptr;
    wxButton* count1MinusBtn_ = nullptr;
    wxStaticText* count1ValueText_ = nullptr;
    wxButton* count1PlusBtn_ = nullptr;
    wxButton* count2MinusBtn_ = nullptr;
    wxStaticText* count2ValueText_ = nullptr;
    wxButton* count2PlusBtn_ = nullptr;
    wxButton* mixBtn_ = nullptr;
    wxStaticText* element1Text_ = nullptr;
    wxStaticText* element2Text_ = nullptr;
    wxStaticText* plusLabel_    = nullptr;
    UiState uiState_;
    ReactionController reactionController_;

    wxStaticText* selectionText_ = nullptr;
    void updateSelectionText();
    void updateLanguage();

    wxStaticText* resultFormulaText_ = nullptr;
    wxStaticText* resultNameText_ = nullptr;
    wxStaticText* resultStatusText_ = nullptr;
    wxStaticText* resultHintText_ = nullptr;
    void updateReactionResult(const ReactionResult& result);
    void applyTheme();
    void toggleAnimationPause();
    void adjustCount1(int delta);
    void adjustCount2(int delta);
    void onMixClicked();
    void updateCountTexts();

    bool hasFirstSelection_ = false;
    bool hasSecondSelection_ = false;
    bool animationPaused_ = false;
    void applySidebarSelection();
    void applyAtomCounts();
};

} // namespace mixlab

#endif // ATOM_FRAME_HPP
