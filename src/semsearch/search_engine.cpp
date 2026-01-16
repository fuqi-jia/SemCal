#include "semsearch/search_engine.h"

namespace semcal {
namespace search {

DefaultSearchEngine::DefaultSearchEngine(SearchPolicy policy)
  : currentPolicy_(policy) {
}

SearchResult DefaultSearchEngine::execute(
    const state::SemanticState& initialState,
    std::function<SearchResult(state::SemanticState&)> strategy,
    SearchPolicy policy) {
  currentPolicy_ = policy;
  clear();
  
  auto state = initialState.clone();
  pushState(std::move(state));
  
  while (!isEmpty()) {
    auto currentState = popState();
    if (!currentState) {
      return SearchResult::UNKNOWN;
    }
    
    SearchResult result = strategy(*currentState);
    
    if (result == SearchResult::SAT || result == SearchResult::UNSAT) {
      return result;
    }
    
    // Continue search if UNKNOWN
    // Strategy function should push new states if needed
  }
  
  return SearchResult::UNKNOWN;
}

void DefaultSearchEngine::pushState(std::unique_ptr<state::SemanticState> state) {
  stateQueue_.push(std::move(state));
}

std::unique_ptr<state::SemanticState> DefaultSearchEngine::popState() {
  if (stateQueue_.empty()) {
    return nullptr;
  }
  
  auto state = std::move(stateQueue_.front());
  stateQueue_.pop();
  return state;
}

bool DefaultSearchEngine::isEmpty() const {
  return stateQueue_.empty();
}

size_t DefaultSearchEngine::size() const {
  return stateQueue_.size();
}

void DefaultSearchEngine::clear() {
  while (!stateQueue_.empty()) {
    stateQueue_.pop();
  }
}

} // namespace search
} // namespace semcal
