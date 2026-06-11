// Minimal formal tests for the reaction logic.
#include "reaction.hpp"

#include <cassert>

using mixlab::ReactionInput;
using mixlab::ReactionStatus;

int main() {
	{
		ReactionInput input;
		input.element1 = "H";
		input.count1 = 2;
		input.element2 = "O";
		input.count2 = 1;

		const auto result = mixlab::calculateReaction(input);
		assert(result.status == ReactionStatus::Safe);
		assert(result.formula == "H2O");
		assert(result.known);
	}

	{
		ReactionInput input;
		input.element1 = "C";
		input.count1 = 1;
		input.element2 = "O";
		input.count2 = 1;

		const auto result = mixlab::calculateReaction(input);
		assert(result.status == ReactionStatus::Dangerous);
		assert(result.formula == "CO");
		assert(result.known);
	}

	{
		ReactionInput input;
		input.element1 = "Fe";
		input.count1 = 1;
		input.element2 = "S";
		input.count2 = 1;

		const auto result = mixlab::calculateReaction(input);
		assert(result.status == ReactionStatus::Unknown);
		assert(!result.known);
	}

	return 0;
}
