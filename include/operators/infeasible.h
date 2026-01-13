#ifndef SEMCAL_OPERATORS_INFEASIBLE_H
#define SEMCAL_OPERATORS_INFEASIBLE_H

#include "../state/semantic_state.h"
#include "../domain/abstract_domain.h"
#include "../core/formula.h"
#include "../core/semantics.h"
#include "../domain/concretization.h"

namespace semcal {
namespace operators {

/**
 * @brief Local infeasibility operator (Axiom I).
 * 
 * Infeasible : (F, a) → bool
 * 
 * Soundness requirement:
 * If Infeasible(F, a) = true, then [[F]] ∩ γ(a) = ∅
 * 
 * Declaring a region infeasible must be globally correct.
 */
class Infeasible {
public:
    virtual ~Infeasible() = default;

    /**
     * @brief Check if a semantic state is infeasible.
     * 
     * @param state The semantic state (F, a) to check
     * @param semantics The semantic interpretation function
     * @param concretization The concretization function
     * @return true if [[F]] ∩ γ(a) = ∅
     */
    virtual bool check(
        const state::SemanticState& state,
        const core::Semantics& semantics,
        const domain::Concretization& concretization) const = 0;

    /**
     * @brief Check if an abstract element is infeasible with respect to a formula.
     * 
     * @param formula The constraint F
     * @param element The abstract element a
     * @param semantics The semantic interpretation function
     * @param concretization The concretization function
     * @return true if [[F]] ∩ γ(a) = ∅
     */
    virtual bool check(
        const core::Formula& formula,
        const domain::AbstractElement& element,
        const core::Semantics& semantics,
        const domain::Concretization& concretization) const;
};

/**
 * @brief Default implementation of infeasibility checking.
 */
class DefaultInfeasible : public Infeasible {
public:
    bool check(
        const state::SemanticState& state,
        const core::Semantics& semantics,
        const domain::Concretization& concretization) const override;
};

} // namespace operators
} // namespace semcal

#endif // SEMCAL_OPERATORS_INFEASIBLE_H
