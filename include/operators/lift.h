#pragma once
#include "operator.h"
#include "../util/op_result.h"
#include "../state/semantic_state.h"

namespace semcal {
namespace operators {

/**
 * @brief Lifting operator (Axiom L).
 * 
 * Semantic contract:
 * Let π be a projection.
 * For Lift_π: b ↦ a, the following must hold:
 * γ(a) ⊆ π^{-1}(γ(b))
 * 
 * Approximation direction: UNDER_APPROX
 * Lifting reconstructs higher-dimensional candidates without introducing invalid ones.
 * May miss some valid candidates, but never introduces spurious ones.
 */
class LiftOp : public SemanticOperator {
public:
  virtual ~LiftOp() = default;

  /**
   * @brief Lift a projected state back to higher dimension.
   * 
   * @param low The projected (lower-dimensional) state
   * @param high_context The context from higher dimension
   * @return OpResult with lifted state
   */
  virtual util::OpResult<std::unique_ptr<state::SemanticState>>
  apply(const state::SemanticState& low,
        const state::SemanticState& high_context) = 0;
};

/**
 * @brief Default implementation of lifting.
 */
class DefaultLiftOp : public LiftOp {
public:
  util::OpResult<std::unique_ptr<state::SemanticState>>
  apply(const state::SemanticState& low,
        const state::SemanticState& high_context) override;
};

} // namespace operators
} // namespace semcal
