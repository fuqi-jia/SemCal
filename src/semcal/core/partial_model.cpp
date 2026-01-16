#include "semcal/core/partial_model.h"
#include <sstream>
#include <vector>

namespace semcal {
namespace core {

PartialModel::PartialModel(const std::unordered_map<std::string, std::string>& assignments)
  : assignments_(assignments) {
}

void PartialModel::setAssignment(const std::string& variable, const std::string& value) {
  assignments_[variable] = value;
}

std::string PartialModel::getAssignment(const std::string& variable) const {
  auto it = assignments_.find(variable);
  if (it != assignments_.end()) {
    return it->second;
  }
  return "";
}

bool PartialModel::hasAssignment(const std::string& variable) const {
  return assignments_.find(variable) != assignments_.end();
}

bool PartialModel::isExtendedBy(const Model& model) const {
  // Check if all assignments in this partial model are consistent with the total model
  // This is a simplified check; real implementations should use proper model checking
  for (const auto& [var, val] : assignments_) {
    // For now, we assume the model can check consistency
    // Real implementations should use proper theory-specific checking
    (void)model;
    (void)var;
    (void)val;
  }
  return true; // Placeholder
}

std::vector<std::string> PartialModel::getAssignedVariables() const {
  std::vector<std::string> vars;
  vars.reserve(assignments_.size());
  for (const auto& [var, _] : assignments_) {
    vars.push_back(var);
  }
  return vars;
}

bool PartialModel::isEmpty() const {
  return assignments_.empty();
}

std::string PartialModel::toString() const {
  if (assignments_.empty()) {
    return "{}";
  }
  std::ostringstream oss;
  oss << "{";
  bool first = true;
  for (const auto& [var, val] : assignments_) {
    if (!first) oss << ", ";
    oss << var << " = " << val;
    first = false;
  }
  oss << "}";
  return oss.str();
}

std::unique_ptr<PartialModel> PartialModel::clone() const {
  return std::make_unique<PartialModel>(assignments_);
}

} // namespace core
} // namespace semcal
