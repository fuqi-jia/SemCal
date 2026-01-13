#include "../../include/operators/infeasible.h"
#include "../../include/state/semantic_state.h"
#include "../../include/core/semantics.h"
#include "../../include/domain/concretization.h"

namespace semcal {
namespace operators {

util::OpResult<void, InfeasibleWitness>
DefaultInfeasibleOp::apply(const state::SemanticState& σ) {
  // Default implementation: always returns UNKNOWN
  // In a real implementation, this would check if the state is empty
  return util::OpResult<void, InfeasibleWitness>::unknown();
}

} // namespace operators
} // namespace semcal
