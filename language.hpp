#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

#include <string>
#include <wx/string.h>
#include "reaction.hpp"

namespace mixlab {

enum class Language {
    English,
    German
};

inline std::string toStdString(Language lang) {
    return lang == Language::German ? "German" : "English";
}

inline wxString localize(Language lang,
                         const char* english,
                         const char* german) {
    return wxString::FromUTF8(lang == Language::German ? german : english);
}

inline std::string localizeStd(Language lang,
                              const char* english,
                              const char* german) {
    return lang == Language::German ? german : english;
}

wxString menuLanguageLabel(Language lang);
wxString menuLanguageRootLabel(Language lang);

wxString getStatusText(ReactionStatus status, Language lang);
std::string translateHint(const std::string& hint, Language lang);
std::string translateCompoundName(const std::string& name, Language lang);
std::string translateElementName(const std::string& name, Language lang);

} // namespace mixlab

#endif // LANGUAGE_HPP
