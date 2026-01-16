#pragma once
#include "semcal/state/semantic_state.h"
#include <vector>
#include <memory>
#include <functional>
#include <queue>

namespace semcal {
namespace search {

/**
 * @brief Search policy for state scheduling.
 */
enum class SearchPolicy {
  DFS,      // Depth-first search
  BFS,      // Breadth-first search
  BEST_FIRST // Best-first search (requires heuristic)
};

/**
 * @brief Search result.
 */
enum class SearchResult {
  SAT,       // Satisfiable (found a model)
  UNSAT,     // Unsatisfiable (proven no model)
  UNKNOWN,   // Unknown (timeout or resource limit)
  ERROR      // Error occurred
};

/**
 * @brief SemSearch: Generic Search and Execution Engine
 * 
 * SemSearch provides search infrastructure only:
 * - state scheduling (DFS, BFS, best-first)
 * - backtracking and branching
 * - execution of solver strategies
 * - resource management
 * 
 * SemSearch is solver-agnostic.
 * It must NOT:
 * - decide which operator is applied
 * - claim SAT or UNSAT
 * - interpret semantic meaning
 * 
 * SemSearch executes solver programs, it does not define them.
 */
class SearchEngine {
public:
  virtual ~SearchEngine() = default;

  /**
   * @brief Execute a solver strategy.
   * 
   * @param initialState Initial semantic state
   * @param strategy Solver strategy function
   * @param policy Search policy
   * @return Search result
   */
  virtual SearchResult execute(
      const state::SemanticState& initialState,
      std::function<SearchResult(state::SemanticState&)> strategy,
      SearchPolicy policy = SearchPolicy::DFS) = 0;

  /**
   * @brief Push a state onto the search stack.
   * @param state State to push
   */
  virtual void pushState(std::unique_ptr<state::SemanticState> state) = 0;

  /**
   * @brief Pop a state from the search stack.
   * @return Popped state, or nullptr if empty
   */
  virtual std::unique_ptr<state::SemanticState> popState() = 0;

  /**
   * @brief Check if the search stack is empty.
   * @return true if empty
   */
  virtual bool isEmpty() const = 0;

  /**
   * @brief Get the number of states in the search stack.
   * @return Number of states
   */
  virtual size_t size() const = 0;

  /**
   * @brief Clear the search stack.
   */
  virtual void clear() = 0;
};

/**
 * @brief Default implementation of SearchEngine.
 */
class DefaultSearchEngine : public SearchEngine {
private:
  std::queue<std::unique_ptr<state::SemanticState>> stateQueue_;
  SearchPolicy currentPolicy_;

public:
  DefaultSearchEngine(SearchPolicy policy = SearchPolicy::DFS);

  SearchResult execute(
      const state::SemanticState& initialState,
      std::function<SearchResult(state::SemanticState&)> strategy,
      SearchPolicy policy = SearchPolicy::DFS) override;

  void pushState(std::unique_ptr<state::SemanticState> state) override;
  std::unique_ptr<state::SemanticState> popState() override;
  bool isEmpty() const override;
  size_t size() const override;
  void clear() override;
};

} // namespace search
} // namespace semcal
