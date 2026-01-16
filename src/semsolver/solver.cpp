#include "semsolver/solver.h"
#include "semkernel/kernel.h"
#include "semsearch/search_engine.h"

namespace semcal {
namespace solver {

SemSolver::SemSolver(
    std::unique_ptr<SolverStrategy> strategy,
    std::unique_ptr<kernel::SemKernel> kernel,
    std::unique_ptr<search::SearchEngine> searchEngine,
    search::SearchPolicy searchPolicy)
    : strategy_(std::move(strategy)),
      kernel_(std::move(kernel)),
      searchEngine_(std::move(searchEngine)),
      searchPolicy_(searchPolicy) {
}

search::SearchResult SemSolver::solve(const state::SemanticState& initialState) {
  return searchEngine_->execute(
    initialState,
    [this](state::SemanticState& state) -> search::SearchResult {
      return strategy_->execute(state);
    },
    searchPolicy_
  );
}

std::unique_ptr<SemSolver> SolverFactory::createDefault() {
  auto kernel = std::make_unique<kernel::DefaultSemKernel>();
  auto searchEngine = std::make_unique<search::DefaultSearchEngine>();
  
  // Create a default strategy (placeholder)
  class DefaultStrategy : public SolverStrategy {
  public:
    search::SearchResult execute(state::SemanticState& state) override {
      // Default strategy: just return UNKNOWN
      // Real implementations should define proper solver logic
      return search::SearchResult::UNKNOWN;
    }
  };
  
  auto strategy = std::make_unique<DefaultStrategy>();
  
  return std::make_unique<SemSolver>(
    std::move(strategy),
    std::move(kernel),
    std::move(searchEngine),
    search::SearchPolicy::DFS
  );
}

} // namespace solver
} // namespace semcal
