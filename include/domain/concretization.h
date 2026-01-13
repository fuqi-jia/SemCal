#ifndef SEMCAL_DOMAIN_CONCRETIZATION_H
#define SEMCAL_DOMAIN_CONCRETIZATION_H

#include "abstract_domain.h"
#include "../core/semantics.h"
#include <memory>

namespace semcal {
namespace domain {

/**
 * @brief Concretization function.
 * 
 * Maps abstract elements to concrete model sets:
 * γ : A → ℘(M)
 */
class Concretization {
public:
    virtual ~Concretization() = default;

    /**
     * @brief Concretize an abstract element.
     * 
     * Returns the set of models represented by the abstract element.
     * 
     * @param element The abstract element to concretize
     * @return The set of models γ(element)
     */
    virtual core::ModelSet concretize(const AbstractElement& element) const = 0;

    /**
     * @brief Check if an abstract element represents an empty set.
     * @param element The abstract element to check
     * @return true if γ(element) = ∅
     */
    virtual bool isEmpty(const AbstractElement& element) const;

    /**
     * @brief Check if one abstract element is a subset of another.
     * 
     * Checks if γ(a) ⊆ γ(b).
     * 
     * @param a First abstract element
     * @param b Second abstract element
     * @return true if γ(a) ⊆ γ(b)
     */
    virtual bool isSubset(const AbstractElement& a, const AbstractElement& b) const;
};

/**
 * @brief Default implementation of concretization.
 */
class DefaultConcretization : public Concretization {
public:
    core::ModelSet concretize(const AbstractElement& element) const override;
};

} // namespace domain
} // namespace semcal

#endif // SEMCAL_DOMAIN_CONCRETIZATION_H
