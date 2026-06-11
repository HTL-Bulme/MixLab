#include "atom_canvas.hpp"
#include <wx/dcbuffer.h>
#include <algorithm>
#include <cmath>
#include <string>

namespace {

wxString makeFormulaPart(const wxString& symbol, int count) {
    if (count <= 1) {
        return symbol;
    }
    return symbol + wxString::Format("%d", count);
}

struct StatusStyle {
    wxColour glow;
    wxColour core;
    wxColour label;
};

StatusStyle styleForStatus(mixlab::ReactionStatus status, bool darkMode) {
    switch (status) {
        case mixlab::ReactionStatus::Safe:
            return {wxColour(76, 201, 120, 40), wxColour(52, 180, 97), wxColour(210, 255, 222)};
        case mixlab::ReactionStatus::Warning:
            return {wxColour(248, 184, 64, 48), wxColour(237, 167, 41), wxColour(255, 243, 196)};
        case mixlab::ReactionStatus::Dangerous:
            return {wxColour(232, 86, 86, 52), wxColour(214, 68, 68), wxColour(255, 222, 222)};
        case mixlab::ReactionStatus::Unknown:
        default:
            if (darkMode) {
                return {wxColour(110, 134, 170, 40), wxColour(96, 121, 158), wxColour(217, 231, 250)};
            }
            return {wxColour(122, 145, 179, 40), wxColour(112, 130, 160), wxColour(40, 58, 82)};
    }
}

} // namespace

namespace mixlab {

wxBEGIN_EVENT_TABLE(AtomCanvas, wxPanel)
    EVT_PAINT(AtomCanvas::OnPaint)
    EVT_TIMER(wxID_ANY, AtomCanvas::OnTimer)
wxEND_EVENT_TABLE()

AtomCanvas::AtomCanvas(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    timer_ = new wxTimer(this);
    timer_->Start(16); // 16мс = ~60 FPS
}

void AtomCanvas::OnTimer(wxTimerEvent&) {
    const bool hasAtoms = !symbol1_.IsEmpty() && !symbol2_.IsEmpty() && count1_ > 0 && count2_ > 0;
    if (!hasAtoms) {
        reactionPhase_ = false;
        phaseFrame_ = 0;
        globalZeit_ = 0.0f;
        Refresh();
        return;
    }

    if (reactionPhase_) {
        ++phaseFrame_;
        if (phaseFrame_ > 90) {
            reactionPhase_ = false;
            phaseFrame_ = 0;
            globalZeit_ = 0.0f;
        }
    } else {
        globalZeit_ += 1.0f;
        if (globalZeit_ >= static_cast<float>(approachFrames_)) {
            reactionPhase_ = true;
            phaseFrame_ = 0;
        }
    }

    Refresh();
}

void AtomCanvas::OnPaint(wxPaintEvent&) {
    wxAutoBufferedPaintDC dc(this);
    int W = GetSize().GetWidth();
    int H = GetSize().GetHeight();

    dc.SetBackground(wxBrush(
        dunkelModus_ ? wxColour(13,17,23) : wxColour(240,244,248)
    ));
    dc.Clear();

    // Netz
    dc.SetPen(wxPen(
        dunkelModus_ ? wxColour(255,255,255,15) : wxColour(0,0,0,20), 1
    ));
    for (int x = 0; x < W; x += 28) dc.DrawLine(x, 0, x, H);
    for (int y = 0; y < H; y += 28) dc.DrawLine(0, y, W, y);

    const bool hasAtoms = !symbol1_.IsEmpty() && !symbol2_.IsEmpty() && count1_ > 0 && count2_ > 0;
    if (!hasAtoms) {
        return;
    }

    const int radius = 16;
    const int spacing = 38;
    const float wf = static_cast<float>(W);
    const float hf = static_cast<float>(H);
    const float radiusf = static_cast<float>(radius);
    const float spacingf = static_cast<float>(spacing);
    const float progress = std::clamp(globalZeit_ / static_cast<float>(approachFrames_), 0.0f, 1.0f);
    const float leftBase = -40.0f + ((wf * 0.5f - 70.0f) + 40.0f) * progress;
    const float rightBase = (wf + 40.0f) + ((wf * 0.5f + 70.0f) - (wf + 40.0f)) * progress;

    auto drawAtoms = [&](const wxString& symbol, int count, float y, float baseX,
                         const wxColour& atomColor, const wxColour& textColor,
                         int offsetSign) {
        if (symbol.IsEmpty() || count <= 0) {
            return;
        }

        dc.SetBrush(wxBrush(atomColor));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetTextForeground(textColor);

        for (int i = 0; i < count; ++i) {
            const float atomX = baseX + static_cast<float>(offsetSign) * static_cast<float>(i) * spacingf;
            if (atomX < -radiusf || atomX > (wf + radiusf)) {
                continue;
            }

            dc.DrawCircle(static_cast<int>(atomX), static_cast<int>(y), radius);

            int textW = 0;
            int textH = 0;
            dc.GetTextExtent(symbol, &textW, &textH);
            dc.DrawText(symbol,
                        static_cast<int>(atomX - static_cast<float>(textW) * 0.5f),
                        static_cast<int>(y - static_cast<float>(textH) * 0.5f));
        }
    };

    if (!reactionPhase_) {
        const float yLevel = hf * 0.5f;
        drawAtoms(symbol1_, count1_, yLevel, leftBase,
                  wxColour(181, 212, 244), wxColour(12, 68, 124), -1);
        drawAtoms(symbol2_, count2_, yLevel, rightBase,
                  wxColour(159, 225, 203), wxColour(8, 80, 65), 1);
        return;
    }

    const int totalAtoms = count1_ + count2_;
    const int reactionRadius = std::min(54, 24 + totalAtoms * 3);
    const float reactionRadiusf = static_cast<float>(reactionRadius);
    const float pulse = reactionRadiusf + std::sin(static_cast<float>(phaseFrame_) * 0.22f) * 3.0f;

    const StatusStyle style = styleForStatus(reactionStatus_, dunkelModus_);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(style.glow));
    dc.DrawCircle(W / 2, H / 2, reactionRadius * 3);
    dc.SetBrush(wxBrush(wxColour(style.glow.Red(), style.glow.Green(), style.glow.Blue(), 24)));
    dc.DrawCircle(W / 2, H / 2, reactionRadius * 2);
    dc.SetBrush(wxBrush(wxColour(style.glow.Red(), style.glow.Green(), style.glow.Blue(), 14)));
    dc.DrawCircle(W / 2, H / 2, reactionRadius * 4);

    dc.SetPen(*wxTRANSPARENT_PEN);

    dc.SetBrush(wxBrush(style.core));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawCircle(W / 2, H / 2, static_cast<int>(pulse));

    const wxString formula = makeReactionFormula();
    dc.SetTextForeground(dunkelModus_ ? wxColour(245, 250, 255) : wxColour(20, 32, 48));
    wxFont formulaFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    dc.SetFont(formulaFont);
    int fw = 0;
    int fh = 0;
    dc.GetTextExtent(formula, &fw, &fh);
    dc.DrawText(formula, W / 2 - fw / 2, H / 2 - fh / 2);

    wxFont captionFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    dc.SetFont(captionFont);
    dc.SetTextForeground(style.label);
    wxString caption = wxT("Status: ") + reactionStatusText_;
    if (reactionStatusText_.IsEmpty()) {
        caption = wxT("Status: Unbekannt");
    }
    int cw = 0;
    int ch = 0;
    dc.GetTextExtent(caption, &cw, &ch);
    dc.DrawText(caption, W / 2 - cw / 2, H / 2 + fh / 2 + 6);
}

void AtomCanvas::setDunkelModus(bool dunkel) {
    dunkelModus_ = dunkel;
    Refresh();
}

void AtomCanvas::setGeschwindigkeit(int wert) {
    geschwindigkeit_ = wert;
}

void AtomCanvas::setAtoms(const std::string& symbol1, int count1,
                         const std::string& symbol2, int count2) {
    symbol1_ = wxString::FromUTF8(symbol1);
    symbol2_ = wxString::FromUTF8(symbol2);
    count1_ = std::max(0, count1);
    count2_ = std::max(0, count2);

    if (symbol1_.IsEmpty()) {
        count1_ = 0;
    }
    if (symbol2_.IsEmpty()) {
        count2_ = 0;
    }

    reactionPhase_ = false;
    phaseFrame_ = 0;
    globalZeit_ = 0.0f;

    const int speed = std::max(1, geschwindigkeit_);
    approachFrames_ = std::max(45, 140 - speed * 20);

    Refresh();
}

wxString AtomCanvas::makeReactionFormula() const {
    return makeFormulaPart(symbol1_, count1_) + makeFormulaPart(symbol2_, count2_);
}

void AtomCanvas::setReactionStatus(ReactionStatus status, const std::string& statusText) {
    reactionStatus_ = status;
    reactionStatusText_ = wxString::FromUTF8(statusText);
    if (reactionStatusText_.IsEmpty()) {
        reactionStatusText_ = wxT("Unbekannt");
    }
    Refresh();
}

} // namespace mixlab
