#include "reaction.hpp"

#include <string>
#include <utility>
#include <vector>

namespace mixlab {

namespace {

struct ElementStub {
    const char* symbol;
    const char* displayName;
};

const std::vector<ElementStub> kElementStubs = {
    {"H", "Hydrogen"},
    {"O", "Oxygen"},
    {"Na", "Sodium"},
    {"Cl", "Chlorine"},
    {"Fe", "Iron"},
    {"C", "Carbon"},
    {"N", "Nitrogen"},
    {"S", "Sulfur"},
};

std::string makeFormulaPart(const std::string& symbol, int count) {
    if (count <= 1) {
        return symbol;
    }

    return symbol + std::to_string(count);
}

std::string makeInputFormula(const ReactionInput& input) {
    return makeFormulaPart(input.element1, input.count1)
         + makeFormulaPart(input.element2, input.count2);
}

bool matches(const ReactionInput& input,
             const std::string& a, int countA,
             const std::string& b, int countB) {
    const bool normalOrder =
        input.element1 == a && input.count1 == countA &&
        input.element2 == b && input.count2 == countB;

    const bool reversedOrder =
        input.element1 == b && input.count1 == countB &&
        input.element2 == a && input.count2 == countA;

    return normalOrder || reversedOrder;
}

ReactionResult makeResult(std::string formula,
                          std::string name,
                          ReactionStatus status,
                          std::string statusText,
                          std::string hint,
                          bool known = true) {
    return ReactionResult{
        std::move(formula),
        std::move(name),
        std::move(statusText),
        std::move(hint),
        status,
        known
    };
}

} // namespace

std::string getElementDisplayName(const std::string& symbol) {
    for (const auto& e : kElementStubs) {
        if (symbol == e.symbol) {
            return e.displayName;
        }
    }

    return symbol.empty() ? std::string("Element") : symbol;
}

ReactionResult calculateReaction(const ReactionInput& input) {
    if (input.count1 <= 0 || input.count2 <= 0) {
        return makeResult(
            makeInputFormula(input),
            "Ungueltige Eingabe",
            ReactionStatus::Unknown,
            "Unbekannt",
            "Die Anzahl der Atome muss groesser als 0 sein.",
            false
        );
    }

    // Wasser: 2 H + 1 O -> H2O
    if (matches(input, "H", 2, "O", 1)) {
        return makeResult(
            "H2O",
            "Wasser",
            ReactionStatus::Safe,
            "Ungefaehrlich",
            "Verhaeltnis korrekt: 2 H + 1 O"
        );
    }

    // Wasserstoffperoxid: 2 H + 2 O -> H2O2
    if (matches(input, "H", 2, "O", 2)) {
        return makeResult(
            "H2O2",
            "Wasserstoffperoxid",
            ReactionStatus::Warning,
            "Vorsicht",
            "Bekannte Verbindung, aber vorsichtig behandeln."
        );
    }

    // Kochsalz: 1 Na + 1 Cl -> NaCl
    if (matches(input, "Na", 1, "Cl", 1)) {
        return makeResult(
            "NaCl",
            "Kochsalz",
            ReactionStatus::Safe,
            "Ungefaehrlich",
            "Verhaeltnis korrekt: 1 Na + 1 Cl"
        );
    }

    // Kohlendioxid: 1 C + 2 O -> CO2
    if (matches(input, "C", 1, "O", 2)) {
        return makeResult(
            "CO2",
            "Kohlendioxid",
            ReactionStatus::Warning,
            "Maessig",
            "Bekannte Verbindung: 1 C + 2 O"
        );
    }

    // Kohlenmonoxid: 1 C + 1 O -> CO
    if (matches(input, "C", 1, "O", 1)) {
        return makeResult(
            "CO",
            "Kohlenmonoxid",
            ReactionStatus::Dangerous,
            "Gefaehrlich",
            "Bekannte gefaehrliche Verbindung."
        );
    }

    // Ammoniak: 1 N + 3 H -> NH3
    if (matches(input, "N", 1, "H", 3)) {
        return makeResult(
            "NH3",
            "Ammoniak",
            ReactionStatus::Warning,
            "Vorsicht",
            "Bekannte Verbindung: 1 N + 3 H"
        );
    }

    // Chlorwasserstoff: 1 H + 1 Cl -> HCl
    if (matches(input, "H", 1, "Cl", 1)) {
        return makeResult(
            "HCl",
            "Chlorwasserstoff",
            ReactionStatus::Dangerous,
            "Gefaehrlich",
            "Bekannte gefaehrliche Verbindung."
        );
    }

    // Schwefeldioxid: 1 S + 2 O -> SO2
    if (matches(input, "S", 1, "O", 2)) {
        return makeResult(
            "SO2",
            "Schwefeldioxid",
            ReactionStatus::Dangerous,
            "Gefaehrlich",
            "Bekannte gefaehrliche Verbindung."
        );
    }

    return makeResult(
        makeInputFormula(input),
        "Unbekannte Verbindung",
        ReactionStatus::Unknown,
        "Unbekannt",
        "Diese Kombination ist in der Demo-Logik noch nicht gespeichert.",
        false
    );
}

} // namespace mixlab