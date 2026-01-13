#include "../../include/operators/project.h"

namespace semcal {
namespace operators {

util::OpResult<std::unique_ptr<state::SemanticState>>
DefaultProjectOp::apply(const state::SemanticState& σ,
                        const std::vector<std::string>& elim_vars) {
  // Default implementation: returns the same state (no projection)
  return util::OpResult<std::unique_ptr<state::SemanticState>>::ok(
    σ.clone()
  );
}

} // namespace operators
} // namespace semcal
