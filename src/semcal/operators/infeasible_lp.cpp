#include "semcal/operators/infeasible_lp.h"

namespace semcal {
namespace operators {

util::OpResult<void, InfeasibleWitness>
LpInfeasibleOp::apply(const state::SemanticState& σ) {
  auto r = backend.refute(σ);
  if (r.status == util::OpStatus::UNSAT) {
    return util::OpResult<void, InfeasibleWitness>::unsat(
      { r.witness.certificate }
    );
  }
  return util::OpResult<void, InfeasibleWitness>::unknown();
}

} // namespace operators
} // namespace semcal
