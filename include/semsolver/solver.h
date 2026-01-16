#pragma once
#include "semcal/state/semantic_state.h"
#include "semcal/operators/operator.h"
#include "semkernel/kernel.h"
#include "semsearch/search_engine.h"
#include "semcal/util/op_result.h"
#include <memory>
#include <vector>
#include <string>

namespace semcal {
namespace solver {

/**
 * @brief Solver strategy program.
 * 
 * A solver strategy is a program composed of:
 * - semantic operators (Restrict, Decompose, Infeasible, Relax, Refine, Shadow, Lift)
 * - search actions (extend partial model, branch, push/pop)
 * - control flow (conditionals, loops, nondeterministic choice)
 */
class SolverStrategy {
public:
  virtual ~SolverStrategy() = default;

  /**
   * @brief Execute the strategy on a semantic state.
   * 
   * @param state Current semantic state
   * @return Search result
   */
  virtual search::SearchResult execute(state::SemanticState& state) = 0;
};

/**
 * @brief SemSolver: Concrete Solver Programs
 * 
 * A SemSolver is defined by:
 * - a solver strategy (program)
 * - a set of semantic operators
 * - backend oracles
 * - a search policy
 * 
 * Each SemSolver is:
 * - a concrete, reproducible artifact
 * - executable under SemSearch
 * - validated by SemKernel
 * - comparable to other solvers
 */
class SemSolver {
private:
  std::unique_ptr<SolverStrategy> strategy_;
  std::unique_ptr<kernel::SemKernel> kernel_;
  std::unique_ptr<search::SearchEngine> searchEngine_;
  search::SearchPolicy searchPolicy_;

public:
  /**
   * @brief Construct a SemSolver.
   * 
   * @param strategy Solver strategy
   * @param kernel Semantic kernel for validation
   * @param searchEngine Search engine
   * @param searchPolicy Search policy
   */
  SemSolver(
      std::unique_ptr<SolverStrategy> strategy,
      std::unique_ptr<kernel::SemKernel> kernel,
      std::unique_ptr<search::SearchEngine> searchEngine,
      search::SearchPolicy searchPolicy = search::SearchPolicy::DFS);

  /**
   * @brief Solve a constraint problem.
   * 
   * @param initialState Initial semantic state
   * @return Search result
   */
  search::SearchResult solve(const state::SemanticState& initialState);

  /**
   * @brief Get the solver strategy.
   * @return Reference to strategy
   */
  SolverStrategy& getStrategy() const { return *strategy_; }

  /**
   * @brief Get the semantic kernel.
   * @return Reference to kernel
   */
  kernel::SemKernel& getKernel() const { return *kernel_; }

  /**
   * @brief Get the search engine.
   * @return Reference to search engine
   */
  search::SearchEngine& getSearchEngine() const { return *searchEngine_; }
};

/**
 * @brief Factory for creating default solvers.
 */
class SolverFactory {
public:
  /**
   * @brief Create a default solver with default components.
   * @return Default solver
   */
  static std::unique_ptr<SemSolver> createDefault();
};

} // namespace solver
} // namespace semcal
