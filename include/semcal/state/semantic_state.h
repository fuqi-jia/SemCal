#ifndef SEMCAL_STATE_SEMANTIC_STATE_H
#define SEMCAL_STATE_SEMANTIC_STATE_H

#include "semcal/core/formula.h"
#include "semcal/core/partial_model.h"
#include "semcal/domain/abstract_domain.h"
#include "semcal/core/semantics.h"
#include "semcal/domain/concretization.h"
#include <memory>

namespace semcal {
namespace state {

/**
 * @brief Semantic state.
 * 
 * A semantic state is a triple σ = (F, a, μ) where:
 * - F is a constraint (formula)
 * - a ∈ A is an abstract element
 * - μ is a partial model (partial valuation)
 * 
 * The concrete meaning is:
 * Conc(σ) = { M ∈ [[F]] ∩ γ(a) | M ⊇ μ }
 */
class SemanticState {
private:
    std::unique_ptr<core::Formula> formula_;
    std::unique_ptr<domain::AbstractElement> abstractElement_;
    std::unique_ptr<core::PartialModel> partialModel_;

public:
    /**
     * @brief Construct a semantic state.
     * @param formula The constraint formula F
     * @param abstractElement The abstract element a
     * @param partialModel The partial model μ (optional, defaults to empty)
     */
    SemanticState(
        std::unique_ptr<core::Formula> formula,
        std::unique_ptr<domain::AbstractElement> abstractElement,
        std::unique_ptr<core::PartialModel> partialModel = nullptr);

    /**
     * @brief Get the formula component.
     * @return Reference to the formula
     */
    const core::Formula& getFormula() const { return *formula_; }

    /**
     * @brief Get the abstract element component.
     * @return Reference to the abstract element
     */
    const domain::AbstractElement& getAbstractElement() const { return *abstractElement_; }

    /**
     * @brief Get the partial model component.
     * @return Reference to the partial model
     */
    const core::PartialModel& getPartialModel() const { return *partialModel_; }

    /**
     * @brief Get mutable reference to partial model.
     * @return Reference to the partial model
     */
    core::PartialModel& getPartialModel() { return *partialModel_; }

    /**
     * @brief Compute the concrete meaning of this state.
     * 
     * Conc(σ) = { M ∈ [[F]] ∩ γ(a) | M ⊇ μ }
     * 
     * @param semantics The semantic interpretation function
     * @param concretization The concretization function
     * @return The concrete model set
     */
    core::ModelSet concretize(
        const core::Semantics& semantics,
        const domain::Concretization& concretization) const;

    /**
     * @brief Check if the state is empty (has no models).
     * @param semantics The semantic interpretation function
     * @param concretization The concretization function
     * @return true if Conc(σ) = ∅
     */
    bool isEmpty(
        const core::Semantics& semantics,
        const domain::Concretization& concretization) const;

    /**
     * @brief Get a string representation.
     * @return String representation
     */
    std::string toString() const;

    /**
     * @brief Clone the semantic state.
     * @return A new copy
     */
    std::unique_ptr<SemanticState> clone() const;
};

} // namespace state
} // namespace semcal

#endif // SEMCAL_STATE_SEMANTIC_STATE_H
