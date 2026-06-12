#ifndef MOLECULE_DATA_HPP
#define MOLECULE_DATA_HPP

#include "reaction.hpp"

#include <optional>
#include <string>
#include <vector>

namespace mixlab {

struct ElementInfo {
    std::string symbol;
    std::string name;
    int valence = 0;
    std::string background;
    std::string textColor;
};

struct MoleculeInfo {
    std::string formula;
    std::string name;
    ReactionStatus status = ReactionStatus::Unknown;
    std::string statusText;
    std::string hint;
    std::string reactionType;
    std::string state;
};

const std::vector<ElementInfo>& getElementData();
const std::vector<MoleculeInfo>& getMoleculeDatabase();
std::optional<ElementInfo> findElementInfo(const std::string& symbol);
std::optional<MoleculeInfo> findMoleculeInfo(const std::string& formula);

} // namespace mixlab

#endif // MOLECULE_DATA_HPP
