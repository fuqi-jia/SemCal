#pragma once
#include "operator.h"
#include "semcal/util/op_result.h"
#include "semcal/core/formula.h"
#include "semcal/core/model.h"
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Witness for refinement operations.
 */
struct RefineWitness {
  std::string explanation;
};

/**
 * @brief Restoration (Refinement) operator (Axiom C).
 * 
 * Semantic contract:
 * Given:
 * - a relaxation F^α,
 * - a spurious model M ⊨ F^α with M ⊭ F,
 * 
 * a refinement constraint R is **sound** if:
 * F ⇒ R  and  M ⊭ R
 * 
 * Equivalently:
 * [[F]] ⊆ [[F^α ∧ R]]  and  M ⊭ R
 * 
 * Semantic guarantee: PRESERVING
 * Refinement removes spurious behavior
 * without excluding genuine solutions.
 * Real models must be preserved.
 */
class RefineOp : public SemanticOperator {
public:
  virtual ~RefineOp() = default;

  /**
   * @brief Generate a refinement constraint.
   * 
   * @param originalFormula The original formula F
   * @param relaxedFormula The relaxed formula F^α
   * @param spuriousModel The spurious model M that satisfies F^α but not F
   * @return OpResult with refinement constraint R such that:
   *         - F ⇒ R (i.e., [[F]] ⊆ [[F^α ∧ R]])
   *         - M ⊭ R
   */
  virtual util::OpResult<std::unique_ptr<core::Formula>, RefineWitness>
  apply(const core::Formula& originalFormula,
        const core::Formula& relaxedFormula,
        const core::Model& spuriousModel) const = 0;
};

/**
 * @brief Default implementation of refinement.
 */
class DefaultRefineOp : public RefineOp {
public:
  util::OpResult<std::unique_ptr<core::Formula>, RefineWitness>
  apply(const core::Formula& originalFormula,
        const core::Formula& relaxedFormula,
        const core::Model& spuriousModel) const override;
};

} // namespace operators
} // namespace semcal
