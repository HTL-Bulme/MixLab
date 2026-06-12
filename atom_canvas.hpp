#ifndef ATOM_CANVAS_HPP
#define ATOM_CANVAS_HPP

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/timer.h>
#include <string>
#include "reaction.hpp"

namespace mixlab {

class AtomCanvas : public wxPanel {
public:
    AtomCanvas(wxWindow* parent);

    void setDarkMode(bool dark);
    void setSpeed(int value);
    void setAnimationPaused(bool paused);
    bool isAnimationPaused() const;
    void setAtoms(const std::string& symbol1, int count1,
                  const std::string& symbol2, int count2);
    void setReactionStatus(ReactionStatus status, const std::string& statusText);

private:
    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    wxString makeReactionFormula() const;

    wxTimer* timer_;
    bool darkMode_ = true;
    int speed_ = 3;
    wxString symbol1_;
    wxString symbol2_;
    int count1_ = 0;
    int count2_ = 0;
    bool reactionPhase_ = false;
    int phaseFrame_ = 0;
    int approachFrames_ = 120;
    ReactionStatus reactionStatus_ = ReactionStatus::Unknown;
    wxString reactionStatusText_ = wxT("");
    bool animationPaused_ = false;

    float globalTime_ = 0;
    
    wxDECLARE_EVENT_TABLE();
};

} // namespace mixlab

#endif // ATOM_CANVAS_HPP
