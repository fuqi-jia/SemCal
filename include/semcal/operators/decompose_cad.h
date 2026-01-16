#pragma once
#include "decompose.h"
#include "semcal/backends/cad_backend.h"

namespace semcal {
namespace operators {

/**
 * @brief CAD-based decomposition operator.
 * 
 * Thin wrapper around CadBackend::decompose.
 */
class CadDecomposeOp : public DecomposeOp {
  backends::CadBackend& backend;

public:
  explicit CadDecomposeOp(backends::CadBackend& b) : backend(b) {}

  util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>
  apply(const state::SemanticState& σ) override;
};

} // namespace operators
} // namespace semcal
