#ifndef GUI_SIDEBAR_HPP
#define GUI_SIDEBAR_HPP

#include <wx/wx.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <string>
#include <vector>

namespace mixlab {

// Sidebar panel: element list on the left + reaction history below
class GuiSidebar : public wxPanel {
public:
    GuiSidebar(wxWindow* parent);

    // Returns the symbol of the currently selected element, e.g. "H"
    std::string getSelectedElement() const;

    // Appends one entry to the reaction history list
    void addHistoryEntry(const std::string& formula, const std::string& name);

    // Switches between dark and light visual mode
    void setDarkMode(bool dark);

private:
    void buildLayout();
    void onElementSelected(wxCommandEvent& event);

    wxListBox*  elementList_;
    wxListBox*  historyList_;
    wxStaticText* elementsLabel_;
    wxStaticText* historyLabel_;

    bool darkMode_ = true;

    wxDECLARE_EVENT_TABLE();
};

} // namespace mixlab

#endif // GUI_SIDEBAR_HPP