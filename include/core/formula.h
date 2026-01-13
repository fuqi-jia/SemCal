#ifndef SEMCAL_CORE_FORMULA_H
#define SEMCAL_CORE_FORMULA_H

#include <memory>
#include <string>
#include <vector>

namespace semcal {
namespace core {

/**
 * @brief Represents a constraint (formula) in the semantic calculus.
 * 
 * A formula is a logical expression that can be evaluated
 * over models to determine satisfaction.
 */
class Formula {
public:
    virtual ~Formula() = default;

    /**
     * @brief Get the string representation of the formula.
     * @return String representation
     */
    virtual std::string toString() const = 0;

    /**
     * @brief Clone the formula.
     * @return A new copy of the formula
     */
    virtual std::unique_ptr<Formula> clone() const = 0;

    /**
     * @brief Check if this formula is logically equivalent to another.
     * @param other The other formula to compare
     * @return true if formulas are logically equivalent
     */
    virtual bool isEquivalent(const Formula& other) const = 0;
};

/**
 * @brief A concrete formula implementation with SMT-LIB style syntax.
 */
class ConcreteFormula : public Formula {
private:
    std::string expression_;

public:
    explicit ConcreteFormula(const std::string& expression);
    explicit ConcreteFormula(const char* expression);

    std::string toString() const override;
    std::unique_ptr<Formula> clone() const override;
    bool isEquivalent(const Formula& other) const override;

    const std::string& getExpression() const { return expression_; }
    void setExpression(const std::string& expression) { expression_ = expression; }
};

/**
 * @brief Factory functions for creating formulas.
 */
namespace FormulaFactory {
    std::unique_ptr<Formula> createConjunction(const std::vector<std::unique_ptr<Formula>>& formulas);
    std::unique_ptr<Formula> createDisjunction(const std::vector<std::unique_ptr<Formula>>& formulas);
    std::unique_ptr<Formula> createNegation(std::unique_ptr<Formula> formula);
    std::unique_ptr<Formula> createImplication(std::unique_ptr<Formula> premise, 
                                               std::unique_ptr<Formula> conclusion);
}

} // namespace core
} // namespace semcal

#endif // SEMCAL_CORE_FORMULA_H
