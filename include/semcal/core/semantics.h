#ifndef SEMCAL_CORE_SEMANTICS_H
#define SEMCAL_CORE_SEMANTICS_H

#include "model.h"
#include "formula.h"
#include <set>
#include <memory>

namespace semcal {
namespace core {

/**
 * @brief Represents the set of all models M.
 */
using ModelSet = std::set<std::shared_ptr<Model>>;

/**
 * @brief Semantic interpretation function.
 * 
 * For a formula F, returns its model set:
 * [[F]] = { M ∈ M | M ⊨ F }
 */
class Semantics {
public:
    virtual ~Semantics() = default;

    /**
     * @brief Compute the model set of a formula.
     * @param formula The formula to interpret
     * @return The set of models that satisfy the formula
     */
    virtual ModelSet interpret(const Formula& formula) const = 0;

    /**
     * @brief Check if a model satisfies a formula.
     * @param model The model to check
     * @param formula The formula to check against
     * @return true if model ⊨ formula
     */
    virtual bool satisfies(const Model& model, const Formula& formula) const = 0;

    /**
     * @brief Check if two formulas are logically equivalent.
     * 
     * F ≡ G ⟺ [[F]] = [[G]]
     * 
     * @param f1 First formula
     * @param f2 Second formula
     * @return true if formulas are logically equivalent
     */
    virtual bool areEquivalent(const Formula& f1, const Formula& f2) const = 0;

    /**
     * @brief Compute the intersection of two model sets.
     * @param s1 First model set
     * @param s2 Second model set
     * @return Intersection of the two sets
     */
    virtual ModelSet intersect(const ModelSet& s1, const ModelSet& s2) const;

    /**
     * @brief Compute the union of two model sets.
     * @param s1 First model set
     * @param s2 Second model set
     * @return Union of the two sets
     */
    virtual ModelSet unionSet(const ModelSet& s1, const ModelSet& s2) const;

    /**
     * @brief Check if a model set is empty.
     * @param modelSet The model set to check
     * @return true if the set is empty
     */
    virtual bool isEmpty(const ModelSet& modelSet) const;
};

/**
 * @brief Default implementation of semantic interpretation.
 */
class DefaultSemantics : public Semantics {
public:
    ModelSet interpret(const Formula& formula) const override;
    bool satisfies(const Model& model, const Formula& formula) const override;
    bool areEquivalent(const Formula& f1, const Formula& f2) const override;
};

} // namespace core
} // namespace semcal

#endif // SEMCAL_CORE_SEMANTICS_H
