#include "semcal/core/semantics.h"
#include <algorithm>
#include <iterator>

namespace semcal {
namespace core {

ModelSet Semantics::intersect(const ModelSet& s1, const ModelSet& s2) const {
    ModelSet result;
    std::set_intersection(
        s1.begin(), s1.end(),
        s2.begin(), s2.end(),
        std::inserter(result, result.begin())
    );
    return result;
}

ModelSet Semantics::unionSet(const ModelSet& s1, const ModelSet& s2) const {
    ModelSet result;
    std::set_union(
        s1.begin(), s1.end(),
        s2.begin(), s2.end(),
        std::inserter(result, result.begin())
    );
    return result;
}

bool Semantics::isEmpty(const ModelSet& modelSet) const {
    return modelSet.empty();
}

ModelSet DefaultSemantics::interpret(const Formula& formula) const {
    // Default implementation: returns empty set
    // In a real implementation, this would use a solver to compute models
    (void)formula; // Suppress unused parameter warning
    return ModelSet();
}

bool DefaultSemantics::satisfies(const Model& model, const Formula& formula) const {
    // Default implementation: delegate to model
    return model.satisfies(formula.toString());
}

bool DefaultSemantics::areEquivalent(const Formula& f1, const Formula& f2) const {
    // Default implementation: delegate to formula
    return f1.isEquivalent(f2);
}

} // namespace core
} // namespace semcal
