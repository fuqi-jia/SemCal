#ifndef SEMCAL_OPERATORS_RELAX_H
#define SEMCAL_OPERATORS_RELAX_H

#include "../core/formula.h"
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Relaxation operator (Axiom A).
 * 
 * Relax : F → F'
 * 
 * Soundness requirement:
 * [[F]] ⊆ [[Relax(F)]]
 * 
 * Relaxation may admit extra models,
 * but must preserve all real ones.
 */
class Relax {
public:
    virtual ~Relax() = default;

    /**
     * @brief Relax a formula.
     * 
     * @param formula The formula F to relax
     * @return A relaxed formula F' such that [[F]] ⊆ [[F']]
     */
    virtual std::unique_ptr<core::Formula> apply(
        const core::Formula& formula) const = 0;

    /**
     * @brief Check if a formula is a relaxation of another.
     * 
     * @param original The original formula F
     * @param candidate The candidate relaxation F'
     * @param semantics The semantic interpretation function
     * @return true if [[F]] ⊆ [[F']]
     */
    virtual bool isRelaxation(
        const core::Formula& original,
        const core::Formula& candidate,
        const core::Semantics& semantics) const;
};

/**
 * @brief Default implementation of relaxation.
 */
class DefaultRelax : public Relax {
public:
    std::unique_ptr<core::Formula> apply(
        const core::Formula& formula) const override;
};

} // namespace operators
} // namespace semcal

#endif // SEMCAL_OPERATORS_RELAX_H
