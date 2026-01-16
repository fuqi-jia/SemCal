#pragma once
#include "model.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace semcal {
namespace core {

/**
 * @brief Partial model (partial valuation).
 * 
 * A partial model μ is a partial valuation:
 * μ : Var ⇀ Val
 * 
 * A total model M extends μ iff M ⊇ μ.
 * Partial models represent search decisions.
 */
class PartialModel {
private:
  std::unordered_map<std::string, std::string> assignments_;

public:
  PartialModel() = default;
  explicit PartialModel(const std::unordered_map<std::string, std::string>& assignments);

  /**
   * @brief Set an assignment for a variable.
   * @param variable Variable name
   * @param value Value assignment
   */
  void setAssignment(const std::string& variable, const std::string& value);

  /**
   * @brief Get the assignment for a variable.
   * @param variable Variable name
   * @return Value if assigned, empty string if not assigned
   */
  std::string getAssignment(const std::string& variable) const;

  /**
   * @brief Check if a variable is assigned.
   * @param variable Variable name
   * @return true if variable is assigned
   */
  bool hasAssignment(const std::string& variable) const;

  /**
   * @brief Check if a total model extends this partial model.
   * @param model Total model
   * @return true if model ⊇ μ
   */
  bool isExtendedBy(const Model& model) const;

  /**
   * @brief Get all assigned variables.
   * @return Set of variable names
   */
  std::vector<std::string> getAssignedVariables() const;

  /**
   * @brief Check if this is empty (no assignments).
   * @return true if empty
   */
  bool isEmpty() const;

  /**
   * @brief Get a string representation.
   * @return String representation
   */
  std::string toString() const;

  /**
   * @brief Clone the partial model.
   * @return A new copy
   */
  std::unique_ptr<PartialModel> clone() const;
};

} // namespace core
} // namespace semcal
