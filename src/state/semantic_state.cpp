#include "state/semantic_state.h"
#include <sstream>

namespace semcal {
namespace state {

SemanticState::SemanticState(
    std::unique_ptr<core::Formula> formula,
    std::unique_ptr<domain::AbstractElement> abstractElement)
    : formula_(std::move(formula)),
      abstractElement_(std::move(abstractElement)) {
}

core::ModelSet SemanticState::concretize(
    const core::Semantics& semantics,
    const domain::Concretization& concretization) const {
    // Conc(σ) = [[F]] ∩ γ(a)
    auto formulaModels = semantics.interpret(getFormula());
    auto abstractModels = concretization.concretize(getAbstractElement());
    
    return semantics.intersect(formulaModels, abstractModels);
}

bool SemanticState::isEmpty(
    const core::Semantics& semantics,
    const domain::Concretization& concretization) const {
    auto concreteSet = concretize(semantics, concretization);
    return semantics.isEmpty(concreteSet);
}

std::string SemanticState::toString() const {
    std::ostringstream oss;
    oss << "(" << formula_->toString() << ", " << abstractElement_->toString() << ")";
    return oss.str();
}

std::unique_ptr<SemanticState> SemanticState::clone() const {
    return std::make_unique<SemanticState>(
        formula_->clone(),
        abstractElement_->clone()
    );
}

} // namespace state
} // namespace semcal
