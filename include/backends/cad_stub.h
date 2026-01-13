#pragma once
#include "cad_backend.h"

namespace semcal {
namespace backends {

/**
 * @brief Stub CAD backend implementation.
 * 
 * This is a placeholder that always returns UNKNOWN.
 * Replace with actual CAD implementation (e.g., using libpoly).
 */
class CadStubBackend : public CadBackend {
public:
  util::OpResult<void, CadRefuteWitness>
  refute(const state::SemanticState& σ) override;

  util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>
  decompose(const state::SemanticState& σ) override;
};

} // namespace backends
} // namespace semcal
