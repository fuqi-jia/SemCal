#include "domain/concretization.h"

namespace semcal {
namespace domain {

bool Concretization::isEmpty(const AbstractElement& element) const {
    auto modelSet = concretize(element);
    return modelSet.empty();
}

bool Concretization::isSubset(const AbstractElement& a, const AbstractElement& b) const {
    auto setA = concretize(a);
    auto setB = concretize(b);
    
    for (const auto& model : setA) {
        bool found = false;
        for (const auto& otherModel : setB) {
            if (model->equals(*otherModel)) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

core::ModelSet DefaultConcretization::concretize(const AbstractElement& element) const {
    // Default implementation: returns empty set
    // In a real implementation, this would compute the actual model set
    (void)element; // Suppress unused parameter warning
    return core::ModelSet();
}

} // namespace domain
} // namespace semcal
