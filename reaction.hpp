#pragma once

#include <string>

namespace mixlab {

enum class ReactionStatus {
    Safe,
    Warning,
    Dangerous,
    Unknown
};

struct ReactionInput {
    std::string element1;
    int count1 = 1;

    std::string element2;
    int count2 = 1;
};

struct ReactionResult {
    std::string formula;
    std::string name;
    std::string statusText;
    std::string hint;

    ReactionStatus status = ReactionStatus::Unknown;
    bool known = false;
};

// Gemeinsame Schnittstelle zwischen Anwendung und Reaktionsmodul.
// Die einfache Implementierung in reaction.cpp dient nur zur Demonstration
// und zum Testen des Anwendungskerns.
// Die vollstaendige chemische Logik kann spaeter ueber dieselbe Funktion
// angeschlossen werden.
ReactionResult calculateReaction(const ReactionInput& input);

} // namespace mixlab