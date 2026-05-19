#ifndef ATOM_FRAME_HPP
#define ATOM_FRAME_HPP

#include <wx/wx.h>

namespace mixlab {

class AtomCanvas; // Forward declaration

class AtomFrame : public wxFrame {
public:
    AtomFrame(const wxString& title);
private:
    AtomCanvas* canvas_ = nullptr;
};

} // namespace mixlab

#endif // ATOM_FRAME_HPP
