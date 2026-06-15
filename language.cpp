#include "language.hpp"

#include <vector>

namespace mixlab {

wxString menuLanguageLabel(Language lang) {
    return localize(lang, "Language: English", "Sprache: Deutsch");
}

wxString menuLanguageRootLabel(Language lang) {
    return localize(lang, "Language", "Sprache");
}

wxString getStatusText(ReactionStatus status, Language lang) {
    switch (status) {
        case ReactionStatus::Safe:
            return localize(lang, "Safe", "Ungefährlich");
        case ReactionStatus::Warning:
            return localize(lang, "Warning", "Vorsicht");
        case ReactionStatus::Dangerous:
            return localize(lang, "Dangerous", "Gefährlich");
        default:
            return localize(lang, "Unknown", "Unbekannt");
    }
}

std::string translateHint(const std::string& hint, Language lang) {
    if (lang == Language::German) {
        if (hint == "Correct ratio: 2 H + 1 O") return "Verhältnis korrekt: 2 H + 1 O";
        if (hint == "Known compound, handle with care.") return "Bekannte Verbindung, vorsichtig behandeln.";
        if (hint == "Correct ratio: 1 Na + 1 Cl") return "Verhältnis korrekt: 1 Na + 1 Cl";
        if (hint == "Known compound: 1 C + 2 O") return "Bekannte Verbindung: 1 C + 2 O";
        if (hint == "Known dangerous compound.") return "Bekannte gefährliche Verbindung.";
        if (hint == "Known compound: 1 N + 3 H") return "Bekannte Verbindung: 1 N + 3 H";
        if (hint == "This combination is not available in the demo logic yet.") return "Diese Kombination ist in der Demo-Logik noch nicht verfügbar.";
        if (hint == "The number of atoms must be greater than 0.") return "Die Anzahl der Atome muss größer als 0 sein.";
    }
    return hint;
}

std::string translateCompoundName(const std::string& name, Language lang) {
    static const std::vector<std::pair<std::string, std::string>> translations = {
        {"Water", "Wasser"},
        {"Hydrogen peroxide", "Wasserstoffperoxid"},
        {"Hydrogen chloride", "Chlorwasserstoff"},
        {"Ammonia", "Ammoniak"},
        {"Sodium chloride", "Kochsalz"},
        {"Carbon dioxide", "Kohlendioxid"},
        {"Carbon monoxide", "Kohlenmonoxid"},
        {"Sulfur dioxide", "Schwefeldioxid"},
        {"Invalid input", "Ungültige Eingabe"},
        {"Unknown compound", "Unbekannte Verbindung"}
    };

    for (const auto& pair : translations) {
        if (name == pair.first) {
            return lang == Language::German ? pair.second : pair.first;
        }
        if (name == pair.second) {
            return lang == Language::German ? pair.second : pair.first;
        }
    }
    return name;
}

std::string translateElementName(const std::string& name, Language lang) {
    static const std::vector<std::pair<std::string, std::string>> translations = {
        {"Hydrogen", "Wasserstoff"},
        {"Oxygen", "Sauerstoff"},
        {"Sodium", "Natrium"},
        {"Chlorine", "Chlor"},
        {"Iron", "Eisen"},
        {"Carbon", "Kohlenstoff"},
        {"Nitrogen", "Stickstoff"},
        {"Sulfur", "Schwefel"}
    };

    for (const auto& pair : translations) {
        if (name == pair.first) {
            return lang == Language::German ? pair.second : pair.first;
        }
        if (name == pair.second) {
            return lang == Language::German ? pair.second : pair.first;
        }
    }
    return name;
}

} // namespace mixlab
