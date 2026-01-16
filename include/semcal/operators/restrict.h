#pragma once
#include "operator.h"
#include "semcal/util/op_result.h"
#include "semcal/state/semantic_state.h"
#include "semcal/core/formula.h"
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Witness for restriction operations.
 */
struct RestrictWitness {
  std::string explanation;
  
  // Indicates whether this is an exact or safe restriction
  bool is_exact = false;
};

/**
 * @brief Restriction operator (Axiom R).
 * 
 * Semantic contract:
 * 
 * **Exact Restriction:**
 * A restriction operator Restrict: (F,a) ↦ (F,a') is exactly sound if:
 * [[F]] ∩ γ(a) = [[F]] ∩ γ(a')
 * 
 * Equivalently:
 * γ(a') ⊆ γ(a)  and  [[F]] ∩ γ(a) ⊆ γ(a')
 * 
 * **Safe Restriction:**
 * A restriction operator is safely sound if:
 * [[F]] ∩ γ(a) ⊆ [[F]] ∩ γ(a')
 * 
 * Semantic guarantee: PRESERVING / SAFE
 * - Exact restriction preserves the concrete meaning
 * - Safe restriction preserves satisfiability only
 * 
 * Restriction may remove spurious candidates,
 * but must never remove genuine satisfying models.
 */
class RestrictOp : public SemanticOperator {
public:
  virtual ~RestrictOp() = default;

  /**
   * @brief Apply restriction to a semantic state.
   * 
   * @param σ The semantic state (F, a) to restrict
   * @param additionalFormula Additional constraint to apply (optional, can be empty)
   * @return OpResult with restricted state (F, a')
   *         - If exact: [[F]] ∩ γ(a) = [[F]] ∩ γ(a')
   *         - If safe: [[F]] ∩ γ(a) ⊆ [[F]] ∩ γ(a')
   */
  virtual util::OpResult<std::unique_ptr<state::SemanticState>, RestrictWitness>
  apply(const state::SemanticState& σ,
        const core::Formula& additionalFormula) const = 0;
};

/**
 * @brief Default implementation of restriction.
 */
class DefaultRestrictOp : public RestrictOp {
public:
  util::OpResult<std::unique_ptr<state::SemanticState>, RestrictWitness>
  apply(const state::SemanticState& σ,
        const core::Formula& additionalFormula) const override;
};

} // namespace operators
} // namespace semcal
