#include "semcal/backends/cad_stub.h"

namespace semcal {
namespace backends {

util::OpResult<void, CadRefuteWitness>
CadStubBackend::refute(const state::SemanticState&) {
  // TODO: Implement CAD cell refutation
  return util::OpResult<void, CadRefuteWitness>::unknown();
}

util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>
CadStubBackend::decompose(const state::SemanticState& σ) {
  // TODO: Implement CAD cell decomposition
  // Stub: return single state (no decomposition)
  std::vector<std::unique_ptr<state::SemanticState>> result;
  result.push_back(σ.clone());
  return util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>::ok(
    std::move(result)
  );
}

} // namespace backends
} // namespace semcal
