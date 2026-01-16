#include "semsolver/strategies/strategy.h"
#include "semsolver/strategies/pipeline.h"
#include "semcal/util/op_result.h"
#include <queue>
#include <algorithm>
#include <vector>
#include <functional>

namespace semcal {
namespace solver {
namespace strategies {

LegacyDepthFirstStrategy::LegacyDepthFirstStrategy(int maxDepth)
    : maxDepth_(maxDepth) {
}

std::vector<std::unique_ptr<semcal::state::SemanticState>> LegacyDepthFirstStrategy::execute(
    const semcal::state::SemanticState& initialState,
    LegacyOperatorPipeline& pipeline) const {
    std::vector<std::unique_ptr<semcal::state::SemanticState>> result;
    std::vector<std::pair<std::unique_ptr<semcal::state::SemanticState>, int>> stack;
    
    stack.push_back({initialState.clone(), 0});
    
    while (!stack.empty()) {
        auto [state, depth] = std::move(stack.back());
        stack.pop_back();
        
        if (depth > maxDepth_) {
            continue;
        }
        
        // Check if infeasible
        auto infeasibleResult = pipeline.getInfeasible().apply(*state);
        if (infeasibleResult.status == semcal::util::OpStatus::UNSAT) {
            continue;
        }
        
        // Try to decompose
        auto decomposeResult = pipeline.getDecompose().apply(*state);
        if (decomposeResult.status != semcal::util::OpStatus::OK || !decomposeResult.value.has_value()) {
            continue;
        }
        auto& decomposed = decomposeResult.value.value();
        
        if (decomposed.size() == 1 && decomposed[0]->toString() == state->toString()) {
            // No decomposition occurred, this is a leaf
            result.push_back(std::move(state));
        } else {
            // Add decomposed states to stack
            for (size_t i = 0; i < decomposed.size(); ++i) {
                auto decomposedState = std::move(decomposed[i]);
                stack.push_back({std::move(decomposedState), depth + 1});
            }
        }
    }
    
    return result;
}

LegacyBreadthFirstStrategy::LegacyBreadthFirstStrategy(int maxWidth)
    : maxWidth_(maxWidth) {
}

std::vector<std::unique_ptr<semcal::state::SemanticState>> LegacyBreadthFirstStrategy::execute(
    const semcal::state::SemanticState& initialState,
    LegacyOperatorPipeline& pipeline) const {
    std::vector<std::unique_ptr<semcal::state::SemanticState>> result;
    std::queue<std::unique_ptr<semcal::state::SemanticState>> queue;
    
    queue.push(initialState.clone());
    
    while (!queue.empty() && result.size() < static_cast<size_t>(maxWidth_)) {
        auto state = std::move(queue.front());
        queue.pop();
        
        // Check if infeasible
        auto infeasibleResult = pipeline.getInfeasible().apply(*state);
        if (infeasibleResult.status == semcal::util::OpStatus::UNSAT) {
            continue;
        }
        
        // Try to decompose
        auto decomposeResult = pipeline.getDecompose().apply(*state);
        if (decomposeResult.status != semcal::util::OpStatus::OK || !decomposeResult.value.has_value()) {
            continue;
        }
        auto& decomposed = decomposeResult.value.value();
        
        if (decomposed.size() == 1 && decomposed[0]->toString() == state->toString()) {
            // No decomposition occurred, this is a leaf
            result.push_back(std::move(state));
        } else {
            // Add decomposed states to queue
            for (size_t i = 0; i < decomposed.size(); ++i) {
                auto decomposedState = std::move(decomposed[i]);
                queue.push(std::move(decomposedState));
            }
        }
    }
    
    return result;
}

LegacyBestFirstStrategy::LegacyBestFirstStrategy(
    std::function<double(const semcal::state::SemanticState&)> heuristic)
    : heuristic_(std::move(heuristic)) {
}

std::vector<std::unique_ptr<semcal::state::SemanticState>> LegacyBestFirstStrategy::execute(
    const semcal::state::SemanticState& initialState,
    LegacyOperatorPipeline& pipeline) const {
    std::vector<std::unique_ptr<semcal::state::SemanticState>> result;
    
    // Use a priority queue with the heuristic
    auto compare = [this](const std::unique_ptr<semcal::state::SemanticState>& a,
                          const std::unique_ptr<semcal::state::SemanticState>& b) {
        return heuristic_(*a) > heuristic_(*b); // Lower heuristic value = higher priority
    };
    
    std::priority_queue<
        std::unique_ptr<semcal::state::SemanticState>,
        std::vector<std::unique_ptr<semcal::state::SemanticState>>,
        decltype(compare)> queue(compare);
    
    queue.push(initialState.clone());
    
    while (!queue.empty()) {
        auto state = std::move(const_cast<std::unique_ptr<semcal::state::SemanticState>&>(queue.top()));
        queue.pop();
        
        // Check if infeasible
        auto infeasibleResult = pipeline.getInfeasible().apply(*state);
        if (infeasibleResult.status == semcal::util::OpStatus::UNSAT) {
            continue;
        }
        
        // Try to decompose
        auto decomposeResult = pipeline.getDecompose().apply(*state);
        if (decomposeResult.status != semcal::util::OpStatus::OK || !decomposeResult.value.has_value()) {
            continue;
        }
        auto& decomposed = decomposeResult.value.value();
        
        if (decomposed.size() == 1 && decomposed[0]->toString() == state->toString()) {
            // No decomposition occurred, this is a leaf
            result.push_back(std::move(state));
        } else {
            // Add decomposed states to queue
            for (size_t i = 0; i < decomposed.size(); ++i) {
                auto decomposedState = std::move(decomposed[i]);
                queue.push(std::move(decomposedState));
            }
        }
    }
    
    return result;
}

} // namespace strategies
} // namespace solver
} // namespace semcal
