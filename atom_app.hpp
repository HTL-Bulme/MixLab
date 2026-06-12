#ifndef ATOM_APP_HPP
#define ATOM_APP_HPP

#include <wx/wx.h>

namespace mixlab {

    class AtomApp : public wxApp {
        public:
            bool OnInit();
    };

} // namespace mixlab

#endif // ATOM_APP_HPP