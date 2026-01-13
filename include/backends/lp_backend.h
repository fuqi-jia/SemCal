#pragma once
#include "../state/semantic_state.h"
#include "../util/op_result.h"
#include <string>

namespace semcal {
namespace backends {

/**
 * @brief Witness for LP refutation (Farkas certificate).
 */
struct LpWitness {
  std::string certificate;
};

/**
 * @brief LP/Simplex backend capability interface.
 * 
 * LP provides:
 * - refute: LP infeasibility with Farkas certificate (Axiom I)
 * 
 * Important: LP refute must be applied to under-approx or implied constraints
 * to ensure soundness. If applied to over-approx relaxations, it cannot
 * conclude UNSAT for the original problem.
 */
class LpBackend {
public:
  virtual ~LpBackend() = default;

  /**
   * @brief Refute a semantic state using LP infeasibility (Axiom I).
   * 
   * Semantic contract:
   * If refute(σ) returns UNSAT, then [[F]] ∩ γ(a) = ∅
   * 
   * Approximation direction: REFUTE_CERTIFIED
   * UNSAT claims must be globally correct (proven by Farkas certificate).
   * 
   * Requires: The constraints in σ must be linear or linearizable,
   * and must form an under-approximation or implied constraint set.
   * If applied to over-approx relaxations, cannot conclude UNSAT for original.
   * 
   * @param σ The semantic state (F, a) to refute
   * @return OpResult with UNSAT if refuted (with Farkas certificate), UNKNOWN otherwise
   */
  virtual util::OpResult<void, LpWitness>
  refute(const state::SemanticState& σ) = 0;
};

} // namespace backends
} // namespace semcal
