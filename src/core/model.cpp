#include "core/model.h"
#include <sstream>

namespace semcal {
namespace core {

ConcreteModel::ConcreteModel(const std::unordered_map<std::string, std::string>& assignments)
    : assignments_(assignments) {
}

void ConcreteModel::setAssignment(const std::string& variable, const std::string& value) {
    assignments_[variable] = value;
}

std::string ConcreteModel::getAssignment(const std::string& variable) const {
    auto it = assignments_.find(variable);
    if (it != assignments_.end()) {
        return it->second;
    }
    return "";
}

bool ConcreteModel::hasAssignment(const std::string& variable) const {
    return assignments_.find(variable) != assignments_.end();
}

bool ConcreteModel::satisfies(const std::string& constraint) const {
    // Default implementation: check if constraint string contains variable assignments
    // In a real implementation, this would parse and evaluate the constraint
    (void)constraint; // Suppress unused parameter warning
    return true; // Placeholder
}

std::string ConcreteModel::toString() const {
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& pair : assignments_) {
        if (!first) {
            oss << ", ";
        }
        oss << pair.first << " = " << pair.second;
        first = false;
    }
    oss << "}";
    return oss.str();
}

std::unique_ptr<Model> ConcreteModel::clone() const {
    return std::make_unique<ConcreteModel>(assignments_);
}

bool ConcreteModel::equals(const Model& other) const {
    const auto* otherConcrete = dynamic_cast<const ConcreteModel*>(&other);
    if (!otherConcrete) {
        return false;
    }
    return assignments_ == otherConcrete->assignments_;
}

} // namespace core
} // namespace semcal
