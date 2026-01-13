#include "../../include/operators/decompose.h"

namespace semcal {
namespace operators {

util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>
DefaultDecomposeOp::apply(const state::SemanticState& σ) {
  // Default implementation: returns a single-element vector (no decomposition)
  std::vector<std::unique_ptr<state::SemanticState>> result;
  result.push_back(σ.clone());
  return util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>::ok(
    std::move(result)
  );
}

} // namespace operators
} // namespace semcal
