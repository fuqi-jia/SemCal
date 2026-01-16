#include "semcal/state/semantic_state.h"
#include <sstream>

namespace semcal {
namespace state {

SemanticState::SemanticState(
    std::unique_ptr<core::Formula> formula,
    std::unique_ptr<domain::AbstractElement> abstractElement,
    std::unique_ptr<core::PartialModel> partialModel)
    : formula_(std::move(formula)),
      abstractElement_(std::move(abstractElement)),
      partialModel_(partialModel ? std::move(partialModel) : std::make_unique<core::PartialModel>()) {
}

core::ModelSet SemanticState::concretize(
    const core::Semantics& semantics,
    const domain::Concretization& concretization) const {
    // Conc(σ) = { M ∈ [[F]] ∩ γ(a) | M ⊇ μ }
    auto formulaModels = semantics.interpret(getFormula());
    auto abstractModels = concretization.concretize(getAbstractElement());
    auto baseSet = semantics.intersect(formulaModels, abstractModels);
    
    // Filter models that extend the partial model
    // Real implementations should use proper model filtering
    return baseSet; // Placeholder: should filter by M ⊇ μ
}

bool SemanticState::isEmpty(
    const core::Semantics& semantics,
    const domain::Concretization& concretization) const {
    auto concreteSet = concretize(semantics, concretization);
    return semantics.isEmpty(concreteSet);
}

std::string SemanticState::toString() const {
    std::ostringstream oss;
    oss << "(" << formula_->toString() << ", " 
        << abstractElement_->toString() << ", " 
        << partialModel_->toString() << ")";
    return oss.str();
}

std::unique_ptr<SemanticState> SemanticState::clone() const {
    return std::make_unique<SemanticState>(
        formula_->clone(),
        abstractElement_->clone(),
        partialModel_->clone()
    );
}

} // namespace state
} // namespace semcal
