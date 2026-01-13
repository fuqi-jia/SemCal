#include "operators/infeasible.h"
#include "state/semantic_state.h"

namespace semcal {
namespace operators {

bool Infeasible::check(
    const core::Formula& formula,
    const domain::AbstractElement& element,
    const core::Semantics& semantics,
    const domain::Concretization& concretization) const {
    // Create a temporary semantic state
    auto tempFormula = std::make_unique<core::ConcreteFormula>(formula.toString());
    auto tempState = std::make_unique<state::SemanticState>(
        std::move(tempFormula),
        element.clone()
    );
    return check(*tempState, semantics, concretization);
}

bool DefaultInfeasible::check(
    const state::SemanticState& state,
    const core::Semantics& semantics,
    const domain::Concretization& concretization) const {
    // Default implementation: check if concretization is empty
    return state.isEmpty(semantics, concretization);
}

} // namespace operators
} // namespace semcal
