#ifndef SEMCAL_SOLVER_STRATEGIES_STRATEGY_H
#define SEMCAL_SOLVER_STRATEGIES_STRATEGY_H

#include "../../semcal/state/semantic_state.h"
#include "pipeline.h"
#include <vector>
#include <memory>
#include <functional>

namespace semcal {
namespace solver {
namespace strategies {

/**
 * @brief Legacy search strategy (deprecated).
 * 
 * @deprecated This class is deprecated. Use SemSolver with SolverLegacySearchStrategy instead.
 * 
 * Strategies define how to explore the search space
 * using semantic operators. This was the old way before SemX architecture.
 */
class LegacySearchStrategy {
public:
    virtual ~LegacySearchStrategy() = default;

    /**
     * @brief Execute the strategy on an initial state.
     * 
     * @param initialState The initial semantic state
     * @param pipeline The pipeline of semantic operators
     * @return A vector of resulting semantic states
     */
    virtual std::vector<std::unique_ptr<semcal::state::SemanticState>> execute(
        const semcal::state::SemanticState& initialState,
        LegacyOperatorPipeline& pipeline) const = 0;
};

/**
 * @brief Depth-first search strategy.
 */
class LegacyDepthFirstStrategy : public LegacySearchStrategy {
private:
    int maxDepth_;

public:
    explicit LegacyDepthFirstStrategy(int maxDepth = 1000);
    std::vector<std::unique_ptr<semcal::state::SemanticState>> execute(
        const semcal::state::SemanticState& initialState,
        LegacyOperatorPipeline& pipeline) const override;
};

/**
 * @brief Breadth-first search strategy.
 */
class LegacyBreadthFirstStrategy : public LegacySearchStrategy {
private:
    int maxWidth_;

public:
    explicit LegacyBreadthFirstStrategy(int maxWidth = 1000);
    std::vector<std::unique_ptr<semcal::state::SemanticState>> execute(
        const semcal::state::SemanticState& initialState,
        LegacyOperatorPipeline& pipeline) const override;
};

/**
 * @brief Best-first search strategy.
 */
class LegacyBestFirstStrategy : public LegacySearchStrategy {
private:
    std::function<double(const semcal::state::SemanticState&)> heuristic_;

public:
    explicit LegacyBestFirstStrategy(
        std::function<double(const semcal::state::SemanticState&)> heuristic);
    std::vector<std::unique_ptr<semcal::state::SemanticState>> execute(
        const semcal::state::SemanticState& initialState,
        LegacyOperatorPipeline& pipeline) const override;
};

} // namespace strategies
} // namespace solver
} // namespace semcal

#endif // SEMCAL_SOLVER_STRATEGIES_STRATEGY_H
