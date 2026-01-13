#include "../../include/operators/infeasible_cad.h"

namespace semcal {
namespace operators {

util::OpResult<void, InfeasibleWitness>
CadInfeasibleOp::apply(const state::SemanticState& σ) {
  auto r = backend.refute(σ);
  if (r.status == util::OpStatus::UNSAT) {
    return util::OpResult<void, InfeasibleWitness>::unsat(
      { r.witness.reason }
    );
  }
  return util::OpResult<void, InfeasibleWitness>::unknown();
}

} // namespace operators
} // namespace semcal
