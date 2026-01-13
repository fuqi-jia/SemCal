#include "operators/lift.h"

namespace semcal {
namespace operators {

std::unique_ptr<domain::AbstractElement> Lift::apply(
    const domain::AbstractElement& projectedElement,
    const std::string& variable) const {
    std::vector<std::string> variables = {variable};
    return apply(projectedElement, variables);
}

std::unique_ptr<domain::AbstractElement> DefaultLift::apply(
    const domain::AbstractElement& projectedElement,
    const std::vector<std::string>& variables) const {
    // Default implementation: returns a copy
    // In a real implementation, this would lift to the full space
    (void)variables; // Suppress unused parameter warning
    return projectedElement.clone();
}

} // namespace operators
} // namespace semcal
