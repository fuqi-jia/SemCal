#pragma once
#include "operator.h"
#include "../util/op_result.h"
#include <string>

namespace semcal {
namespace operators {

/**
 * @brief Witness for infeasibility refutation.
 */
struct InfeasibleWitness {
  std::string explanation;
};

/**
 * @brief Infeasibility operator (Axiom I).
 * 
 * Semantic contract:
 * If Infeasible(F,a) = true (returns UNSAT), then:
 * [[F]] ∩ γ(a) = ∅
 * 
 * Equivalently: Conc(σ) = ∅
 * 
 * Approximation direction: REFUTE_CERTIFIED
 * UNSAT claims must be globally correct.
 * 
 * This operator refutes a semantic state, proving it has no models.
 */
class InfeasibleOp : public SemanticOperator {
public:
  virtual ~InfeasibleOp() = default;

  /**
   * @brief Check if a semantic state is infeasible.
   * 
   * @param σ The semantic state to check
   * @return OpResult with UNSAT if refuted, UNKNOWN otherwise
   */
  virtual util::OpResult<void, InfeasibleWitness>
  apply(const state::SemanticState& σ) = 0;
};

/**
 * @brief Default implementation of infeasibility checking.
 */
class DefaultInfeasibleOp : public InfeasibleOp {
public:
  util::OpResult<void, InfeasibleWitness>
  apply(const state::SemanticState& σ) override;
};

} // namespace operators
} // namespace semcal
