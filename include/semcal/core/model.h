#ifndef SEMCAL_CORE_MODEL_H
#define SEMCAL_CORE_MODEL_H

#include <memory>
#include <string>
#include <unordered_map>

namespace semcal {
namespace core {

/**
 * @brief Represents a model in the semantic calculus.
 * 
 * A model is an assignment that satisfies constraints.
 * This is an abstract base class that can be specialized
 * for different theories (e.g., integer, real, bit-vector).
 */
class Model {
public:
    virtual ~Model() = default;

    /**
     * @brief Check if this model satisfies a given constraint.
     * @param constraint The constraint to check
     * @return true if the model satisfies the constraint
     */
    virtual bool satisfies(const std::string& constraint) const = 0;

    /**
     * @brief Get a string representation of the model.
     * @return String representation
     */
    virtual std::string toString() const = 0;

    /**
     * @brief Clone the model.
     * @return A new copy of the model
     */
    virtual std::unique_ptr<Model> clone() const = 0;

    /**
     * @brief Check equality with another model.
     * @param other The other model to compare
     * @return true if models are equal
     */
    virtual bool equals(const Model& other) const = 0;
};

/**
 * @brief A concrete model implementation with variable assignments.
 */
class ConcreteModel : public Model {
private:
    std::unordered_map<std::string, std::string> assignments_;

public:
    ConcreteModel() = default;
    explicit ConcreteModel(const std::unordered_map<std::string, std::string>& assignments);

    void setAssignment(const std::string& variable, const std::string& value);
    std::string getAssignment(const std::string& variable) const;
    bool hasAssignment(const std::string& variable) const;

    bool satisfies(const std::string& constraint) const override;
    std::string toString() const override;
    std::unique_ptr<Model> clone() const override;
    bool equals(const Model& other) const override;
};

} // namespace core
} // namespace semcal

#endif // SEMCAL_CORE_MODEL_H
