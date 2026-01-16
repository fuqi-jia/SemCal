#include "semcal/operators/restrict.h"
#include "semcal/core/formula.h"
#include "semcal/state/semantic_state.h"

namespace semcal {
namespace operators {

util::OpResult<std::unique_ptr<state::SemanticState>, RestrictWitness>
DefaultRestrictOp::apply(const state::SemanticState& σ,
                         const core::Formula& additionalFormula) const {
  // Default implementation: returns the same state (no restriction)
  // In a real implementation, this would narrow the abstract element
  // to satisfy: [[F]] ∩ γ(a) ⊆ [[F]] ∩ γ(a') (safe) or = (exact)
  (void)additionalFormula; // Suppress unused parameter warning
  
  RestrictWitness witness;
  witness.explanation = "Default implementation: no restriction applied";
  witness.is_exact = false; // Safe restriction by default
  
  util::OpResult<std::unique_ptr<state::SemanticState>, RestrictWitness> result;
  result.status = util::OpStatus::OK;
  result.value = σ.clone();
  result.witness = witness;
  return result;
}

} // namespace operators
} // namespace semcal
