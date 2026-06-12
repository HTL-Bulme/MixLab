#include "atom_mixer.hpp"
#include "molecule_data.hpp"

namespace mixlab {

ReactionResult mixElements(const ReactionInput& input) {
    const auto result = calculateReaction(input);
    return result;
}

} // namespace mixlab
