#include "semcal/operators/infeasible.h"
#include "semcal/state/semantic_state.h"
#include "semcal/core/semantics.h"
#include "semcal/domain/concretization.h"

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
