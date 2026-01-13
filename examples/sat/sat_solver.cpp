#include "semcal.h"
#include <iostream>
#include <memory>

using namespace semcal;

/**
 * @brief Example SAT solver built using SemCal operators.
 * 
 * This demonstrates how to construct a satisfiability solver
 * by orchestrating semantic operators.
 */
class SimpleSATSolver {
private:
    std::unique_ptr<orchestration::Pipeline> pipeline_;

public:
    SimpleSATSolver() {
        pipeline_ = orchestration::PipelineFactory::createDefault();
    }

    /**
     * @brief Check if a formula is satisfiable.
     * @param formula The formula to check
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
        orchestration::DepthFirstStrategy strategy(100);
        auto results = strategy.execute(*state, *pipeline_);

        // If we found any feasible states, the formula is satisfiable
        return !results.empty();
    }
};

int main() {
    std::cout << "SemCal SAT Solver Example" << std::endl;
    std::cout << "=========================" << std::endl;

    // Create a simple formula: (x > 0) ∧ (x < 10)
    auto formula1 = std::make_unique<core::ConcreteFormula>("(and (> x 0) (< x 10))");
    
    SimpleSATSolver solver;
    bool result = solver.isSatisfiable(*formula1);
    
    std::cout << "Formula: " << formula1->toString() << std::endl;
    std::cout << "Satisfiable: " << (result ? "Yes" : "No") << std::endl;

    return 0;
}
