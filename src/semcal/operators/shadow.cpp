#include "semcal/operators/shadow.h"
#include "semcal/state/semantic_state.h"

namespace semcal {
namespace operators {

util::OpResult<std::unique_ptr<state::SemanticState>>
DefaultShadowOp::apply(const state::SemanticState& σ,
                       const std::vector<std::string>& elim_vars) {
  // Default implementation: return UNKNOWN
  // Real implementations should compute ∃_π([[F]] ∩ γ(a)) ⊆ γ(b)
  return util::OpResult<std::unique_ptr<state::SemanticState>>::unknown();
}

} // namespace operators
} // namespace semcal
