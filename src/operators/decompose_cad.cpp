#include "../../include/operators/decompose_cad.h"

namespace semcal {
namespace operators {

util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>
CadDecomposeOp::apply(const state::SemanticState& σ) {
  return backend.decompose(σ);
}

} // namespace operators
} // namespace semcal
