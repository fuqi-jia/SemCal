#pragma once
#include "operator.h"
#include "../util/op_result.h"
#include "../state/semantic_state.h"
#include <vector>

namespace semcal {
namespace operators {

/**
 * @brief Decomposition operator (Axiom D).
 * 
 * Semantic contract:
 * For Decomp: a ↦ {a₁, ..., aₙ}, the following must hold:
 * γ(a) ⊆ ∪ᵢ γ(aᵢ)
 * 
 * Equivalently, for semantic states:
 * Conc(σ) ⊆ ∪ᵢ Conc(σᵢ)
 * where Conc(σ) = [[F]] ∩ γ(a) and Conc(σᵢ) = [[F]] ∩ γ(aᵢ)
 * 
 * Approximation direction: PRESERVING
 * Decomposition splits the search space but must cover all possibilities.
 * Overlap and redundancy are permitted.
 */
class DecomposeOp : public SemanticOperator {
public:
  virtual ~DecomposeOp() = default;

  /**
   * @brief Decompose a semantic state into multiple states.
   * 
   * @param σ The semantic state to decompose
   * @return OpResult with vector of decomposed states
   */
  virtual util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>
  apply(const state::SemanticState& σ) = 0;
};

/**
 * @brief Default implementation of decomposition.
 */
class DefaultDecomposeOp : public DecomposeOp {
public:
  util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>
  apply(const state::SemanticState& σ) override;
};

} // namespace operators
} // namespace semcal
