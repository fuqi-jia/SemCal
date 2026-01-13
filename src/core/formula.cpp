#include "core/formula.h"
#include <sstream>

namespace semcal {
namespace core {

ConcreteFormula::ConcreteFormula(const std::string& expression)
    : expression_(expression) {
}

ConcreteFormula::ConcreteFormula(const char* expression)
    : expression_(expression ? expression : "") {
}

std::string ConcreteFormula::toString() const {
    return expression_;
}

std::unique_ptr<Formula> ConcreteFormula::clone() const {
    return std::make_unique<ConcreteFormula>(expression_);
}

bool ConcreteFormula::isEquivalent(const Formula& other) const {
    const auto* otherConcrete = dynamic_cast<const ConcreteFormula*>(&other);
    if (!otherConcrete) {
        return false;
    }
    // In a real implementation, this would check semantic equivalence
    // For now, we do syntactic comparison
    return expression_ == otherConcrete->expression_;
}

namespace FormulaFactory {

std::unique_ptr<Formula> createConjunction(const std::vector<std::unique_ptr<Formula>>& formulas) {
    if (formulas.empty()) {
        return std::make_unique<ConcreteFormula>("true");
    }
    
    std::ostringstream oss;
    oss << "(and";
    for (const auto& f : formulas) {
        oss << " " << f->toString();
    }
    oss << ")";
    return std::make_unique<ConcreteFormula>(oss.str());
}

std::unique_ptr<Formula> createDisjunction(const std::vector<std::unique_ptr<Formula>>& formulas) {
    if (formulas.empty()) {
        return std::make_unique<ConcreteFormula>("false");
    }
    
    std::ostringstream oss;
    oss << "(or";
    for (const auto& f : formulas) {
        oss << " " << f->toString();
    }
    oss << ")";
    return std::make_unique<ConcreteFormula>(oss.str());
}

std::unique_ptr<Formula> createNegation(std::unique_ptr<Formula> formula) {
    std::ostringstream oss;
    oss << "(not " << formula->toString() << ")";
    return std::make_unique<ConcreteFormula>(oss.str());
}

std::unique_ptr<Formula> createImplication(std::unique_ptr<Formula> premise, 
                                           std::unique_ptr<Formula> conclusion) {
    std::ostringstream oss;
    oss << "(=> " << premise->toString() << " " << conclusion->toString() << ")";
    return std::make_unique<ConcreteFormula>(oss.str());
}

} // namespace FormulaFactory

} // namespace core
} // namespace semcal
