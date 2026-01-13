#ifndef SEMCAL_OPERATORS_DECOMPOSE_H
#define SEMCAL_OPERATORS_DECOMPOSE_H

#include "../domain/abstract_domain.h"
#include "../state/semantic_state.h"
#include <vector>
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Decomposition operator (Axiom D).
 * 
 * Decomp : A → P_fin(A)
 * 
 * Soundness requirement:
 * γ(a) ⊆ ∪_{a_i ∈ Decomp(a)} γ(a_i)
 * 
 * Decomposition splits the search space,
 * but must cover all possibilities.
 */
class Decompose {
public:
    virtual ~Decompose() = default;

    /**
     * @brief Decompose an abstract element.
     * 
     * @param element The abstract element a to decompose
     * @return A vector of abstract elements {a_i} such that
     *         γ(a) ⊆ ∪_i γ(a_i)
     */
    virtual std::vector<std::unique_ptr<domain::AbstractElement>> apply(
        const domain::AbstractElement& element) const = 0;

    /**
     * @brief Decompose a semantic state.
     * 
     * Decomposes the abstract element component.
     * 
     * @param state The semantic state to decompose
     * @return A vector of semantic states with decomposed abstract elements
     */
    virtual std::vector<std::unique_ptr<state::SemanticState>> applyToState(
        const state::SemanticState& state) const;
};

/**
 * @brief Default implementation of decomposition.
 */
class DefaultDecompose : public Decompose {
public:
    std::vector<std::unique_ptr<domain::AbstractElement>> apply(
        const domain::AbstractElement& element) const override;
};

} // namespace operators
} // namespace semcal

#endif // SEMCAL_OPERATORS_DECOMPOSE_H
