#pragma once
#include "infeasible.h"
#include "../backends/lp_backend.h"

namespace semcal {
namespace operators {

/**
 * @brief LP-based infeasibility operator.
 * 
 * Thin wrapper around LpBackend::refute.
 * 
 * Important: This operator is sound only when applied to
 * under-approximations or implied linear constraints.
 */
class LpInfeasibleOp : public InfeasibleOp {
  backends::LpBackend& backend;

public:
  explicit LpInfeasibleOp(backends::LpBackend& b) : backend(b) {}

  util::OpResult<void, InfeasibleWitness>
  apply(const state::SemanticState& σ) override;
};

} // namespace operators
} // namespace semcal
