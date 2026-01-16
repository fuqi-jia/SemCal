#pragma once
#include "infeasible.h"
#include "semcal/backends/cad_backend.h"

namespace semcal {
namespace operators {

/**
 * @brief CAD-based infeasibility operator.
 * 
 * Thin wrapper around CadBackend::refute.
 */
class CadInfeasibleOp : public InfeasibleOp {
  backends::CadBackend& backend;

public:
  explicit CadInfeasibleOp(backends::CadBackend& b) : backend(b) {}

  util::OpResult<void, InfeasibleWitness>
  apply(const state::SemanticState& σ) override;
};

} // namespace operators
} // namespace semcal
