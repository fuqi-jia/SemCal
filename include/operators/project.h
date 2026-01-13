#ifndef SEMCAL_OPERATORS_PROJECT_H
#define SEMCAL_OPERATORS_PROJECT_H

#include "../domain/abstract_domain.h"
#include <vector>
#include <string>
#include <memory>

namespace semcal {
namespace operators {

/**
 * @brief Projection operator (Axiom P).
 * 
 * Proj_π : A → A_k
 * 
 * Soundness requirement:
 * ∃_π(γ(a)) ⊆ γ_k(Proj_π(a))
 * 
 * Projection must safely over-approximate existential elimination.
 */
class Project {
public:
    virtual ~Project() = default;

    /**
     * @brief Project an abstract element onto a subset of variables.
     * 
     * @param element The abstract element a to project
     * @param variables The set of variables π to project onto
     * @return The projected abstract element Proj_π(a)
     */
    virtual std::unique_ptr<domain::AbstractElement> apply(
        const domain::AbstractElement& element,
        const std::vector<std::string>& variables) const = 0;

    /**
     * @brief Project onto a single variable.
     * 
     * @param element The abstract element to project
     * @param variable The variable to project onto
     * @return The projected abstract element
     */
    virtual std::unique_ptr<domain::AbstractElement> apply(
        const domain::AbstractElement& element,
        const std::string& variable) const;
};

/**
 * @brief Default implementation of projection.
 */
class DefaultProject : public Project {
public:
    std::unique_ptr<domain::AbstractElement> apply(
        const domain::AbstractElement& element,
        const std::vector<std::string>& variables) const override;
};

} // namespace operators
} // namespace semcal

#endif // SEMCAL_OPERATORS_PROJECT_H
