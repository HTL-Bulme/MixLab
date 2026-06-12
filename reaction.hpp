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

// Demo metadata provider for UI while no full element database is connected.
// Kept in reaction module to avoid duplicating element stubs in UI files.
std::string getElementDisplayName(const std::string& symbol);

// Shared interface between the application and the reaction module.
// The simple implementation in reaction.cpp is only used for demonstration
// and for testing the application core.
// The full chemical logic can be connected later through the same function.
ReactionResult calculateReaction(const ReactionInput& input);

} // namespace mixlab