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
            "Invalid input",
            ReactionStatus::Unknown,
            "Unknown",
            "The number of atoms must be greater than 0.",
            false
        );
    }

    // Water: 2 H + 1 O -> H2O
    if (matches(input, "H", 2, "O", 1)) {
        return makeResult(
            "H2O",
            "Water",
            ReactionStatus::Safe,
            "Safe",
            "Correct ratio: 2 H + 1 O"
        );
    }

    // Hydrogen peroxide: 2 H + 2 O -> H2O2
    if (matches(input, "H", 2, "O", 2)) {
        return makeResult(
            "H2O2",
            "Hydrogen peroxide",
            ReactionStatus::Warning,
            "Warning",
            "Known compound, handle with care."
        );
    }

    // Sodium chloride: 1 Na + 1 Cl -> NaCl
    if (matches(input, "Na", 1, "Cl", 1)) {
        return makeResult(
            "NaCl",
            "Sodium chloride",
            ReactionStatus::Safe,
            "Safe",
            "Correct ratio: 1 Na + 1 Cl"
        );
    }

    // Carbon dioxide: 1 C + 2 O -> CO2
    if (matches(input, "C", 1, "O", 2)) {
        return makeResult(
            "CO2",
            "Carbon dioxide",
            ReactionStatus::Warning,
            "Moderate",
            "Known compound: 1 C + 2 O"
        );
    }

    // Carbon monoxide: 1 C + 1 O -> CO
    if (matches(input, "C", 1, "O", 1)) {
        return makeResult(
            "CO",
            "Carbon monoxide",
            ReactionStatus::Dangerous,
            "Dangerous",
            "Known dangerous compound."
        );
    }

    // Ammonia: 1 N + 3 H -> NH3
    if (matches(input, "N", 1, "H", 3)) {
        return makeResult(
            "NH3",
            "Ammonia",
            ReactionStatus::Warning,
            "Warning",
            "Known compound: 1 N + 3 H"
        );
    }

    // Hydrogen chloride: 1 H + 1 Cl -> HCl
    if (matches(input, "H", 1, "Cl", 1)) {
        return makeResult(
            "HCl",
            "Hydrogen chloride",
            ReactionStatus::Dangerous,
            "Dangerous",
            "Known dangerous compound."
        );
    }

    // Sulfur dioxide: 1 S + 2 O -> SO2
    if (matches(input, "S", 1, "O", 2)) {
        return makeResult(
            "SO2",
            "Sulfur dioxide",
            ReactionStatus::Dangerous,
            "Dangerous",
            "Known dangerous compound."
        );
    }

    return makeResult(
        makeInputFormula(input),
        "Unknown compound",
        ReactionStatus::Unknown,
        "Unknown",
        "This combination is not available in the demo logic yet.",
        false
    );
}

} // namespace mixlab