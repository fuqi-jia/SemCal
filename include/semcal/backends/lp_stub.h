#pragma once
#include "lp_backend.h"

namespace semcal {
namespace backends {

/**
 * @brief Stub LP backend implementation.
 * 
 * This is a placeholder that always returns UNKNOWN.
 * Replace with actual LP solver (e.g., GLPK, CPLEX, Gurobi).
 */
class LpStubBackend : public LpBackend {
public:
  util::OpResult<void, LpWitness>
  refute(const state::SemanticState& σ) override;
};

} // namespace backends
} // namespace semcal
