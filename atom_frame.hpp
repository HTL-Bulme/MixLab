#ifndef ATOM_FRAME_HPP
#define ATOM_FRAME_HPP
#include "reaction.hpp"
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

    wxSlider* speedSlider_;
    bool dunkelModus_ = true;

    ReactionInput currentInput_;
    wxStaticText* selectionText_ = nullptr;
    void updateSelectionText();

    wxStaticText* resultFormulaText_ = nullptr;
    wxStaticText* resultNameText_ = nullptr;
    wxStaticText* resultStatusText_ = nullptr;
    void updateReactionResult(const ReactionResult& result);
};

} // namespace mixlab

#endif // ATOM_FRAME_HPP
