#include "orchestration/strategy.h"
#include "orchestration/pipeline.h"
#include <queue>
#include <algorithm>
#include <vector>
#include <functional>

namespace semcal {
namespace orchestration {

DepthFirstStrategy::DepthFirstStrategy(int maxDepth)
    : maxDepth_(maxDepth) {
}

std::vector<std::unique_ptr<state::SemanticState>> DepthFirstStrategy::execute(
    const state::SemanticState& initialState,
    Pipeline& pipeline) const {
    std::vector<std::unique_ptr<state::SemanticState>> result;
    std::vector<std::pair<std::unique_ptr<state::SemanticState>, int>> stack;
    
    stack.push_back({initialState.clone(), 0});
    
    while (!stack.empty()) {
        auto [state, depth] = std::move(stack.back());
        stack.pop_back();
        
        if (depth > maxDepth_) {
            continue;
        }
        
        // Check if infeasible
        if (pipeline.getInfeasible().check(
                *state,
                pipeline.getSemantics(),
                pipeline.getConcretization())) {
            continue;
        }
        
        // Try to decompose
        auto decomposed = pipeline.getDecompose().applyToState(*state);
        
        if (decomposed.size() == 1 && decomposed[0]->toString() == state->toString()) {
            // No decomposition occurred, this is a leaf
            result.push_back(std::move(state));
        } else {
            // Add decomposed states to stack
            for (auto& decomposedState : decomposed) {
                stack.push_back({std::move(decomposedState), depth + 1});
            }
        }
    }
    
    return result;
}

BreadthFirstStrategy::BreadthFirstStrategy(int maxWidth)
    : maxWidth_(maxWidth) {
}

std::vector<std::unique_ptr<state::SemanticState>> BreadthFirstStrategy::execute(
    const state::SemanticState& initialState,
    Pipeline& pipeline) const {
    std::vector<std::unique_ptr<state::SemanticState>> result;
    std::queue<std::unique_ptr<state::SemanticState>> queue;
    
    queue.push(initialState.clone());
    
    while (!queue.empty() && result.size() < static_cast<size_t>(maxWidth_)) {
        auto state = std::move(queue.front());
        queue.pop();
        
        // Check if infeasible
        if (pipeline.getInfeasible().check(
                *state,
                pipeline.getSemantics(),
                pipeline.getConcretization())) {
            continue;
        }
        
        // Try to decompose
        auto decomposed = pipeline.getDecompose().applyToState(*state);
        
        if (decomposed.size() == 1 && decomposed[0]->toString() == state->toString()) {
            // No decomposition occurred, this is a leaf
            result.push_back(std::move(state));
        } else {
            // Add decomposed states to queue
            for (auto& decomposedState : decomposed) {
                queue.push(std::move(decomposedState));
            }
        }
    }
    
    return result;
}

BestFirstStrategy::BestFirstStrategy(
    std::function<double(const state::SemanticState&)> heuristic)
    : heuristic_(std::move(heuristic)) {
}

std::vector<std::unique_ptr<state::SemanticState>> BestFirstStrategy::execute(
    const state::SemanticState& initialState,
    Pipeline& pipeline) const {
    std::vector<std::unique_ptr<state::SemanticState>> result;
    
    // Use a priority queue with the heuristic
    auto compare = [this](const std::unique_ptr<state::SemanticState>& a,
                          const std::unique_ptr<state::SemanticState>& b) {
        return heuristic_(*a) > heuristic_(*b); // Lower heuristic value = higher priority
    };
    
    std::priority_queue<
        std::unique_ptr<state::SemanticState>,
        std::vector<std::unique_ptr<state::SemanticState>>,
        decltype(compare)> queue(compare);
    
    queue.push(initialState.clone());
    
    while (!queue.empty()) {
        auto state = std::move(const_cast<std::unique_ptr<state::SemanticState>&>(queue.top()));
        queue.pop();
        
        // Check if infeasible
        if (pipeline.getInfeasible().check(
                *state,
                pipeline.getSemantics(),
                pipeline.getConcretization())) {
            continue;
        }
        
        // Try to decompose
        auto decomposed = pipeline.getDecompose().applyToState(*state);
        
        if (decomposed.size() == 1 && decomposed[0]->toString() == state->toString()) {
            // No decomposition occurred, this is a leaf
            result.push_back(std::move(state));
        } else {
            // Add decomposed states to queue
            for (auto& decomposedState : decomposed) {
                queue.push(std::move(decomposedState));
            }
        }
    }
    
    return result;
}

} // namespace orchestration
} // namespace semcal
