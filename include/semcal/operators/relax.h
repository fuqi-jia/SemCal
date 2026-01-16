#pragma once
#include "operator.h"
#include "semcal/util/op_result.h"
#include "semcal/core/formula.h"
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Witness for relaxation operations.
 */
struct RelaxWitness {
  std::string explanation;
};

/**
 * @brief Relaxation operator (Axiom A).
 * 
 * Semantic contract:
 * For a relaxation operator Relax: F ↦ F^α, the following must hold:
 * [[F]] ⊆ [[F^α]]
 * 
 * Semantic guarantee: OVER_APPROX
 * Relaxation may admit spurious models,
 * but must preserve all real ones.
 * By definition, relaxation is an over-approximation.
 */
class RelaxOp : public SemanticOperator {
public:
  virtual ~RelaxOp() = default;

  /**
   * @brief Relax a formula.
   * 
   * @param formula The formula F to relax
   * @return OpResult with relaxed formula F^α such that [[F]] ⊆ [[F^α]]
   */
  virtual util::OpResult<std::unique_ptr<core::Formula>, RelaxWitness>
  apply(const core::Formula& formula) const = 0;
};

/**
 * @brief Default implementation of relaxation.
 */
class DefaultRelaxOp : public RelaxOp {
public:
  util::OpResult<std::unique_ptr<core::Formula>, RelaxWitness>
  apply(const core::Formula& formula) const override;
};

} // namespace operators
} // namespace semcal
