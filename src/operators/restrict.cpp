#include "operators/restrict.h"
#include "core/formula.h"
#include "state/semantic_state.h"

namespace semcal {
namespace operators {

std::unique_ptr<state::SemanticState> Restrict::applyToState(
    const state::SemanticState& state,
    const core::Formula& additionalFormula) const {
    auto restrictedElement = apply(additionalFormula, state.getAbstractElement());
    // In a real implementation, we might combine formulas
    return std::make_unique<state::SemanticState>(
        state.getFormula().clone(),
        std::move(restrictedElement)
    );
}

std::unique_ptr<domain::AbstractElement> DefaultRestrict::apply(
    const core::Formula& formula,
    const domain::AbstractElement& element) const {
    // Default implementation: returns a copy of the element
    // In a real implementation, this would narrow the abstract element
    (void)formula; // Suppress unused parameter warning
    return element.clone();
}

} // namespace operators
} // namespace semcal
