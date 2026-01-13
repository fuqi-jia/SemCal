#pragma once
#include "operator.h"
#include "../util/op_result.h"
#include "../state/semantic_state.h"
#include <vector>
#include <string>

namespace semcal {
namespace operators {

/**
 * @brief Projection operator (Axiom P).
 * 
 * Semantic contract:
 * Let π be a variable projection.
 * For Proj_π: a ↦ b, the following must hold:
 * ∃_π(γ(a)) ⊆ γ(b)
 * 
 * Approximation direction: OVER_APPROX
 * Projection must safely over-approximate existential elimination.
 * May introduce spurious models, but preserves all real ones.
 */
class ProjectOp : public SemanticOperator {
public:
  virtual ~ProjectOp() = default;

  /**
   * @brief Project a semantic state onto a subset of variables.
   * 
   * @param σ The semantic state to project
   * @param elim_vars Variables to eliminate
   * @return OpResult with projected state
   */
  virtual util::OpResult<std::unique_ptr<state::SemanticState>>
  apply(const state::SemanticState& σ,
        const std::vector<std::string>& elim_vars) = 0;
};

/**
 * @brief Default implementation of projection.
 */
class DefaultProjectOp : public ProjectOp {
public:
  util::OpResult<std::unique_ptr<state::SemanticState>>
  apply(const state::SemanticState& σ,
        const std::vector<std::string>& elim_vars) override;
};

} // namespace operators
} // namespace semcal
