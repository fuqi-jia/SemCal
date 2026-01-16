#pragma once
#include "semcal/state/semantic_state.h"
#include "semcal/util/op_result.h"
#include <vector>
#include <string>

namespace semcal {
namespace backends {

/**
 * @brief Witness for CAD refutation.
 */
struct CadRefuteWitness {
  std::string reason;
};

/**
 * @brief CAD (Cylindrical Algebraic Decomposition) backend capability interface.
 * 
 * CAD provides:
 * - refute: Cell refutation (Axiom I)
 * - decompose: Cell decomposition (Axiom D)
 */
class CadBackend {
public:
  virtual ~CadBackend() = default;

  /**
   * @brief Refute a semantic state using CAD (Axiom I).
   * 
   * Semantic contract:
   * If refute(σ) returns UNSAT, then [[F]] ∩ γ(a) = ∅
   * 
   * Approximation direction: REFUTE_CERTIFIED
   * UNSAT claims must be globally correct.
   * 
   * @param σ The semantic state (F, a) to refute
   * @return OpResult with UNSAT if refuted, UNKNOWN otherwise
   */
  virtual util::OpResult<void, CadRefuteWitness>
  refute(const state::SemanticState& σ) = 0;

  /**
   * @brief Decompose a semantic state into cells (Axiom D).
   * 
   * Semantic contract:
   * For Decomp: a ↦ {a₁, ..., aₙ}, returns states such that:
   * γ(a) ⊆ ∪ᵢ γ(aᵢ)
   * 
   * Equivalently: Conc(σ) ⊆ ∪ᵢ Conc(σᵢ)
   * 
   * Approximation direction: PRESERVING
   * Must cover all possibilities. Overlap is permitted.
   * 
   * @param σ The semantic state (F, a) to decompose
   * @return OpResult with vector of decomposed states
   */
  virtual util::OpResult<std::vector<std::unique_ptr<state::SemanticState>>>
  decompose(const state::SemanticState& σ) = 0;
};

} // namespace backends
} // namespace semcal
