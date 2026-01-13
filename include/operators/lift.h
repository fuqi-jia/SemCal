#ifndef SEMCAL_OPERATORS_LIFT_H
#define SEMCAL_OPERATORS_LIFT_H

#include "../domain/abstract_domain.h"
#include <vector>
#include <string>
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Lifting operator (Axiom L).
 * 
 * Lift_π : A_k → A
 * 
 * Soundness requirement:
 * γ(Lift_π(b)) ⊆ π^{-1}(γ_k(b))
 * 
 * Lifting reconstructs higher-dimensional candidates
 * without introducing invalid ones.
 */
class Lift {
public:
    virtual ~Lift() = default;

    /**
     * @brief Lift a projected abstract element back to full space.
     * 
     * @param projectedElement The projected abstract element b
     * @param variables The set of variables π that were projected onto
     * @return The lifted abstract element Lift_π(b)
     */
    virtual std::unique_ptr<domain::AbstractElement> apply(
        const domain::AbstractElement& projectedElement,
        const std::vector<std::string>& variables) const = 0;

    /**
     * @brief Lift from a single variable.
     * 
     * @param projectedElement The projected abstract element
     * @param variable The variable that was projected onto
     * @return The lifted abstract element
     */
    virtual std::unique_ptr<domain::AbstractElement> apply(
        const domain::AbstractElement& projectedElement,
        const std::string& variable) const;
};

/**
 * @brief Default implementation of lifting.
 */
class DefaultLift : public Lift {
public:
    std::unique_ptr<domain::AbstractElement> apply(
        const domain::AbstractElement& projectedElement,
        const std::vector<std::string>& variables) const override;
};

} // namespace operators
} // namespace semcal

#endif // SEMCAL_OPERATORS_LIFT_H
