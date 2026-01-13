#include "../../include/operators/lift.h"

namespace semcal {
namespace operators {

util::OpResult<std::unique_ptr<state::SemanticState>>
DefaultLiftOp::apply(const state::SemanticState& low,
                     const state::SemanticState& high_context) {
  // Default implementation: returns the low-dimensional state
  return util::OpResult<std::unique_ptr<state::SemanticState>>::ok(
    low.clone()
  );
}

} // namespace operators
} // namespace semcal
