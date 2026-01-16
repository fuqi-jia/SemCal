#pragma once
#include "operator.h"
#include "semcal/util/op_result.h"
#include "semcal/state/semantic_state.h"
#include <vector>
#include <string>
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Shadowing operator (Axiom S).
 * 
 * Semantic contract:
 * Let π be a variable projection.
 * A shadowing operator Shadow_π: (F,a) ↦ b is sound if:
 * ∃_π([[F]] ∩ γ(a)) ⊆ γ(b)
 * 
 * Semantic guarantee: OVER_APPROX
 * Shadowing safely collapses a constrained semantic space
 * onto a lower-dimensional space,
 * preserving all feasible *shadows* of satisfying models.
 * 
 * Shadowing is a **semantic operation**.
 * It is distinct from structural or syntactic projection
 * (e.g., CAD polynomial projection).
 * 
 * May introduce spurious models, but preserves all real ones.
 */
class ShadowOp : public SemanticOperator {
public:
  virtual ~ShadowOp() = default;

  /**
   * @brief Shadow a semantic state onto a subset of variables.
   * 
   * This is a semantic operation that projects the constrained
   * semantic space [[F]] ∩ γ(a) onto lower dimensions.
   * 
   * @param σ The semantic state (F, a) to shadow
   * @param elim_vars Variables to eliminate (projection π)
   * @return OpResult with shadowed abstract element b
   *         satisfying: ∃_π([[F]] ∩ γ(a)) ⊆ γ(b)
   */
  virtual util::OpResult<std::unique_ptr<state::SemanticState>>
  apply(const state::SemanticState& σ,
        const std::vector<std::string>& elim_vars) = 0;
};

/**
 * @brief Default implementation of shadowing.
 */
class DefaultShadowOp : public ShadowOp {
public:
  util::OpResult<std::unique_ptr<state::SemanticState>>
  apply(const state::SemanticState& σ,
        const std::vector<std::string>& elim_vars) override;
};

} // namespace operators
} // namespace semcal
