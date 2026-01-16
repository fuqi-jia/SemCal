#include "semx.h"
#include <iostream>
#include <memory>

using namespace semcal;

/**
 * @brief Example model counting solver.
 * 
 * This demonstrates how to count models using decomposition
 * and aggregation over semantic states.
 */
class CountingSolver {
private:
    std::unique_ptr<solver::strategies::LegacyOperatorPipeline> pipeline_;

public:
    CountingSolver() {
        pipeline_ = solver::strategies::LegacyPipelineFactory::createDefault();
    }

    /**
     * @brief Count the number of models satisfying a formula.
     * @param formula The formula to count models for
     * @return The number of models (approximate)
     */
    size_t countModels(const core::Formula& formula) {
        // Create initial state
        auto topElement = std::make_unique<domain::TopElement>();
        auto state = std::make_unique<state::SemanticState>(
            formula.clone(),
            std::move(topElement)
        );

        // Use breadth-first search to explore all states
        solver::strategies::LegacyBreadthFirstStrategy strategy(1000);
        auto results = strategy.execute(*state, *pipeline_);

        // Count models in each resulting state
        size_t totalCount = 0;
        for (const auto& resultState : results) {
            auto models = resultState->concretize(
                pipeline_->getSemantics(),
                pipeline_->getConcretization()
            );
            totalCount += models.size();
        }

        return totalCount;
    }
};

int main() {
    std::cout << "SemCal Model Counting Example" << std::endl;
    std::cout << "==============================" << std::endl;

    // Example: count models for (x > 0) ∧ (x < 10)
    auto formula = std::make_unique<core::ConcreteFormula>("(and (> x 0) (< x 10))");
    
    CountingSolver solver;
    size_t count = solver.countModels(*formula);
    
    std::cout << "Formula: " << formula->toString() << std::endl;
    std::cout << "Model count: " << count << std::endl;

    return 0;
}
