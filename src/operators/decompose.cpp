#include "operators/decompose.h"
#include "state/semantic_state.h"

namespace semcal {
namespace operators {

std::vector<std::unique_ptr<state::SemanticState>> Decompose::applyToState(
    const state::SemanticState& state) const {
    auto decomposedElements = apply(state.getAbstractElement());
    std::vector<std::unique_ptr<state::SemanticState>> result;
    
    for (auto& element : decomposedElements) {
        result.push_back(std::make_unique<state::SemanticState>(
            state.getFormula().clone(),
            std::move(element)
        ));
    }
    
    return result;
}

std::vector<std::unique_ptr<domain::AbstractElement>> DefaultDecompose::apply(
    const domain::AbstractElement& element) const {
    // Default implementation: returns a single-element vector
    // In a real implementation, this would split the element
    std::vector<std::unique_ptr<domain::AbstractElement>> result;
    result.push_back(element.clone());
    return result;
}

} // namespace operators
} // namespace semcal
