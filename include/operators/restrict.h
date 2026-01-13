#ifndef SEMCAL_OPERATORS_RESTRICT_H
#define SEMCAL_OPERATORS_RESTRICT_H

#include "../state/semantic_state.h"
#include "../domain/abstract_domain.h"
#include "../core/formula.h"
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Restriction operator (Axiom R).
 * 
 * Semantic contract:
 * A restriction operator Restrict: (F,a) ↦ (F,a') is sound if:
 * [[F]] ∩ γ(a) = [[F]] ∩ γ(a')
 * 
 * Equivalently:
 * γ(a') ⊆ γ(a)  and  γ(a) ∩ [[F]] ⊆ γ(a')
 * 
 * Approximation direction: PRESERVING
 * Restriction may remove spurious candidates,
 * but must never remove a genuine satisfying model.
 */
class Restrict {
public:
    virtual ~Restrict() = default;

    /**
     * @brief Apply restriction to an abstract element.
     * 
     * @param formula The constraint F to restrict with
     * @param element The abstract element a to restrict
     * @return The restricted abstract element Restrict_F(a)
     */
    virtual std::unique_ptr<domain::AbstractElement> apply(
        const core::Formula& formula,
        const domain::AbstractElement& element) const = 0;

    /**
     * @brief Apply restriction to a semantic state.
     * 
     * Restricts the abstract element component with the formula.
     * 
     * @param state The semantic state to restrict
     * @param additionalFormula Additional constraint to apply
     * @return A new restricted semantic state
     */
    virtual std::unique_ptr<state::SemanticState> applyToState(
        const state::SemanticState& state,
        const core::Formula& additionalFormula) const;
};

/**
 * @brief Default implementation of restriction.
 */
class DefaultRestrict : public Restrict {
public:
    std::unique_ptr<domain::AbstractElement> apply(
        const core::Formula& formula,
        const domain::AbstractElement& element) const override;
};

} // namespace operators
} // namespace semcal

#endif // SEMCAL_OPERATORS_RESTRICT_H
