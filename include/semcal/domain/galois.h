#ifndef SEMCAL_DOMAIN_GALOIS_H
#define SEMCAL_DOMAIN_GALOIS_H

#include "abstract_domain.h"
#include "concretization.h"
#include "semcal/core/semantics.h"
#include <memory>

namespace semcal {
namespace domain {

/**
 * @brief Abstraction function.
 * 
 * Maps concrete model sets to abstract elements:
 * α : ℘(M) → A
 */
class Abstraction {
public:
    virtual ~Abstraction() = default;

    /**
     * @brief Abstract a model set.
     * 
     * Returns the most precise abstract element that represents the set.
     * 
     * @param modelSet The model set to abstract
     * @return The abstract element α(modelSet)
     */
    virtual std::unique_ptr<AbstractElement> abstract(const core::ModelSet& modelSet) const = 0;
};

/**
 * @brief Galois connection between concrete and abstract domains.
 * 
 * A Galois connection satisfies:
 * α(S) ⊑ a ⟺ S ⊆ γ(a)
 * 
 * This enables optimal abstraction and refinement.
 */
class GaloisConnection {
private:
    std::unique_ptr<Abstraction> abstraction_;
    std::unique_ptr<Concretization> concretization_;

public:
    /**
     * @brief Construct a Galois connection.
     * @param abstraction The abstraction function
     * @param concretization The concretization function
     */
    GaloisConnection(
        std::unique_ptr<Abstraction> abstraction,
        std::unique_ptr<Concretization> concretization);

    /**
     * @brief Get the abstraction function.
     * @return Reference to the abstraction function
     */
    Abstraction& getAbstraction() const { return *abstraction_; }

    /**
     * @brief Get the concretization function.
     * @return Reference to the concretization function
     */
    Concretization& getConcretization() const { return *concretization_; }

    /**
     * @brief Check if the Galois connection property holds.
     * 
     * Verifies: α(S) ⊑ a ⟺ S ⊆ γ(a)
     * 
     * @param modelSet A concrete model set S
     * @param element An abstract element a
     * @return true if the property holds
     */
    bool satisfiesGaloisProperty(
        const core::ModelSet& modelSet,
        const AbstractElement& element) const;
};

/**
 * @brief Default implementation of abstraction.
 */
class DefaultAbstraction : public Abstraction {
public:
    std::unique_ptr<AbstractElement> abstract(const core::ModelSet& modelSet) const override;
};

} // namespace domain
} // namespace semcal

#endif // SEMCAL_DOMAIN_GALOIS_H
