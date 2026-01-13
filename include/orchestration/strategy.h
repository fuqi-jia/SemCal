#ifndef SEMCAL_ORCHESTRATION_STRATEGY_H
#define SEMCAL_ORCHESTRATION_STRATEGY_H

#include "../state/semantic_state.h"
#include "pipeline.h"
#include <vector>
#include <memory>
#include <functional>

namespace semcal {
namespace orchestration {

/**
 * @brief Search strategy for solver construction.
 * 
 * Strategies define how to explore the search space
 * using semantic operators. This is outside the semantic
 * calculus but enables solver construction.
 */
class Strategy {
public:
    virtual ~Strategy() = default;

    /**
     * @brief Execute the strategy on an initial state.
     * 
     * @param initialState The initial semantic state
     * @param pipeline The pipeline of semantic operators
     * @return A vector of resulting semantic states
     */
    virtual std::vector<std::unique_ptr<state::SemanticState>> execute(
        const state::SemanticState& initialState,
        Pipeline& pipeline) const = 0;
};

/**
 * @brief Depth-first search strategy.
 */
class DepthFirstStrategy : public Strategy {
private:
    int maxDepth_;

public:
    explicit DepthFirstStrategy(int maxDepth = 1000);
    std::vector<std::unique_ptr<state::SemanticState>> execute(
        const state::SemanticState& initialState,
        Pipeline& pipeline) const override;
};

/**
 * @brief Breadth-first search strategy.
 */
class BreadthFirstStrategy : public Strategy {
private:
    int maxWidth_;

public:
    explicit BreadthFirstStrategy(int maxWidth = 1000);
    std::vector<std::unique_ptr<state::SemanticState>> execute(
        const state::SemanticState& initialState,
        Pipeline& pipeline) const override;
};

/**
 * @brief Best-first search strategy.
 */
class BestFirstStrategy : public Strategy {
private:
    std::function<double(const state::SemanticState&)> heuristic_;

public:
    explicit BestFirstStrategy(
        std::function<double(const state::SemanticState&)> heuristic);
    std::vector<std::unique_ptr<state::SemanticState>> execute(
        const state::SemanticState& initialState,
        Pipeline& pipeline) const override;
};

} // namespace orchestration
} // namespace semcal

#endif // SEMCAL_ORCHESTRATION_STRATEGY_H
