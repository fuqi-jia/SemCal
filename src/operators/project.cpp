#include "operators/project.h"

namespace semcal {
namespace operators {

std::unique_ptr<domain::AbstractElement> Project::apply(
    const domain::AbstractElement& element,
    const std::string& variable) const {
    std::vector<std::string> variables = {variable};
    return apply(element, variables);
}

std::unique_ptr<domain::AbstractElement> DefaultProject::apply(
    const domain::AbstractElement& element,
    const std::vector<std::string>& variables) const {
    // Default implementation: returns a copy
    // In a real implementation, this would project onto the specified variables
    (void)variables; // Suppress unused parameter warning
    return element.clone();
}

} // namespace operators
} // namespace semcal
