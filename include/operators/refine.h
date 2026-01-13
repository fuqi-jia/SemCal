#ifndef SEMCAL_OPERATORS_REFINE_H
#define SEMCAL_OPERATORS_REFINE_H

#include "../core/formula.h"
#include "../core/model.h"
#include "../core/semantics.h"
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Restoration (Refinement) operator (Axiom C).
 * 
 * Semantic contract:
 * Given:
 * - a relaxation F^α,
 * - a spurious model M ⊨ F^α with M ⊭ F,
 * 
 * a refinement constraint R is sound if:
 * [[F]] ⊆ [[F^α ∧ R]]  and  M ⊭ R
 * 
 * Approximation direction: PRESERVING
 * Refinement removes spurious behavior
 * without excluding genuine solutions.
 */
class Refine {
public:
    virtual ~Refine() = default;

    /**
     * @brief Generate a refinement constraint.
     * 
     * @param originalFormula The original formula F
     * @param relaxedFormula The relaxed formula F^α
     * @param spuriousModel The spurious model M that satisfies F^α but not F
     * @param semantics The semantic interpretation function
     * @return A refinement constraint R such that:
     *         - [[F]] ⊆ [[F^α ∧ R]]
     *         - M ⊭ R
     */
    virtual std::unique_ptr<core::Formula> generate(
        const core::Formula& originalFormula,
        const core::Formula& relaxedFormula,
        const core::Model& spuriousModel,
        const core::Semantics& semantics) const = 0;

    /**
     * @brief Refine a relaxed formula with a constraint.
     * 
     * @param relaxedFormula The relaxed formula F^α
     * @param refinementConstraint The refinement constraint R
     * @return The refined formula F^α ∧ R
     */
    virtual std::unique_ptr<core::Formula> apply(
        const core::Formula& relaxedFormula,
        const core::Formula& refinementConstraint) const;
};

/**
 * @brief Default implementation of refinement.
 */
class DefaultRefine : public Refine {
public:
    std::unique_ptr<core::Formula> generate(
        const core::Formula& originalFormula,
        const core::Formula& relaxedFormula,
        const core::Model& spuriousModel,
        const core::Semantics& semantics) const override;
};

} // namespace operators
} // namespace semcal

#endif // SEMCAL_OPERATORS_REFINE_H
