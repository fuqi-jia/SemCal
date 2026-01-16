#include "semx.h"
#include <iostream>
#include <memory>

using namespace semcal;

/**
 * @brief Example SAT solver built using SemCal operators.
 * 
 * This demonstrates how to construct a pure Boolean satisfiability (SAT) solver
 * by orchestrating semantic operators for propositional logic.
 */
class SimpleSATSolver {
private:
    std::unique_ptr<solver::strategies::LegacyOperatorPipeline> pipeline_;

public:
    SimpleSATSolver() {
        pipeline_ = solver::strategies::LegacyPipelineFactory::createDefault();
    }

    /**
     * @brief Check if a Boolean formula is satisfiable.
     * @param formula The propositional formula to check
     * @return true if satisfiable, false otherwise
     */
    bool isSatisfiable(const core::Formula& formula) {
        // Create initial semantic state with top abstract element
        auto topElement = std::make_unique<domain::TopElement>();
        auto state = std::make_unique<state::SemanticState>(
            formula.clone(),
            std::move(topElement)
        );

        // Use depth-first search strategy
        solver::strategies::LegacyDepthFirstStrategy strategy(100);
        auto results = strategy.execute(*state, *pipeline_);

        // If we found any feasible states, the formula is satisfiable
        return !results.empty();
    }
};

int main() {
    std::cout << "SemCal SAT Solver Example" << std::endl;
    std::cout << "=========================" << std::endl;

    // Create a simple Boolean formula: (a ∨ b) ∧ (¬a ∨ c)
    auto formula1 = std::make_unique<core::ConcreteFormula>("(and (or a b) (or (not a) c))");
    
    SimpleSATSolver solver;
    bool result = solver.isSatisfiable(*formula1);
    
    std::cout << "Formula: " << formula1->toString() << std::endl;
    std::cout << "Satisfiable: " << (result ? "Yes" : "No") << std::endl;

    return 0;
}
