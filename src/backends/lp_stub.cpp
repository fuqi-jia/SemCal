#include "../../include/backends/lp_stub.h"

namespace semcal {
namespace backends {

util::OpResult<void, LpWitness>
LpStubBackend::refute(const state::SemanticState&) {
  // TODO: Implement LP infeasibility check with Farkas certificate
  return util::OpResult<void, LpWitness>::unknown();
}

} // namespace backends
} // namespace semcal
