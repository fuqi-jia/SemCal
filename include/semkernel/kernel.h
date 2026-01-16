#pragma once
#include "semcal/state/semantic_state.h"
#include "semcal/util/op_result.h"
#include "semcal/core/model.h"
#include <vector>
#include <memory>
#include <string>

namespace semcal {
namespace kernel {

/**
 * @brief Evidence for semantic claims.
 * 
 * Evidence is checkable proof/justification for semantic claims.
 * SemKernel does not trust solver strategies, backend solvers,
 * or numeric approximations. All UNSAT or refutation claims
 * must be accompanied by checkable evidence.
 */
struct Evidence {
  std::string type;  // Type of evidence (e.g., "refutation", "containment", "covering")
  std::string data;  // Evidence data (format depends on type)
  
  /**
   * @brief Check if this evidence is valid.
   * @return true if evidence is well-formed
   */
  bool isValid() const;
};

/**
 * @brief A step in a solver trace.
 * 
 * Represents a single semantic operation with its evidence.
 */
struct Step {
  std::string operator_name;  // Name of the operator (e.g., "Restrict", "Decompose")
  std::unique_ptr<state::SemanticState> input_state;
  std::unique_ptr<state::SemanticState> output_state;  // May be null for void operations
  Evidence evidence;
  
  std::string toString() const;
};

/**
 * @brief SemKernel: Verified Semantic Kernel
 * 
 * SemKernel is the only trusted component in SemX.
 * Its sole responsibility is to validate semantic claims made by solvers.
 * 
 * SemKernel is not a solver.
 * It performs no search, no optimization, and no heuristics.
 */
class SemKernel {
public:
  virtual ~SemKernel() = default;

  /**
   * @brief Check a single step.
   * 
   * Validates that the step's evidence supports the semantic claim.
   * 
   * @param state Current semantic state
   * @param step The step to check
   * @return OpResult with updated state if step is valid, error otherwise
   */
  virtual util::OpResult<std::unique_ptr<state::SemanticState>>
  checkStep(const state::SemanticState& state, const Step& step) = 0;

  /**
   * @brief Run a trace (sequence of steps).
   * 
   * Validates an entire solver trace.
   * If a trace is accepted, the resulting semantic claim is correct.
   * 
   * @param initialState Initial semantic state
   * @param steps Sequence of (step, evidence) pairs
   * @return OpResult with final state if trace is valid, error otherwise
   */
  virtual util::OpResult<std::unique_ptr<state::SemanticState>>
  runTrace(const state::SemanticState& initialState,
           const std::vector<Step>& steps) = 0;

  /**
   * @brief Check a refutation claim.
   * 
   * Validates: Conc(σ) = ∅
   * 
   * @param state The semantic state to check
   * @param evidence Evidence for the refutation
   * @return true if refutation is valid
   */
  virtual bool checkRefutation(const state::SemanticState& state,
                               const Evidence& evidence) = 0;

  /**
   * @brief Check a containment claim.
   * 
   * Validates: Conc(σ) ⊆ Conc(σ')
   * 
   * @param state1 First semantic state
   * @param state2 Second semantic state
   * @param evidence Evidence for the containment
   * @return true if containment is valid
   */
  virtual bool checkContainment(const state::SemanticState& state1,
                                const state::SemanticState& state2,
                                const Evidence& evidence) = 0;

  /**
   * @brief Check a covering claim.
   * 
   * Validates: Conc(σ) ⊆ ∪ᵢ Conc(σᵢ)
   * 
   * @param state The original semantic state
   * @param decomposedStates The decomposed states
   * @param evidence Evidence for the covering
   * @return true if covering is valid
   */
  virtual bool checkCovering(const state::SemanticState& state,
                             const std::vector<std::unique_ptr<state::SemanticState>>& decomposedStates,
                             const Evidence& evidence) = 0;

  /**
   * @brief Check a model validity claim.
   * 
   * Validates: M ∈ Conc(σ)
   * 
   * @param state The semantic state
   * @param model The model to check
   * @return true if model is valid
   */
  virtual bool checkModelValidity(const state::SemanticState& state,
                                  const core::Model& model) = 0;
};

/**
 * @brief Default implementation of SemKernel.
 * 
 * Provides basic validation. Real implementations should use
 * verified checking mechanisms.
 */
class DefaultSemKernel : public SemKernel {
public:
  util::OpResult<std::unique_ptr<state::SemanticState>>
  checkStep(const state::SemanticState& state, const Step& step) override;

  util::OpResult<std::unique_ptr<state::SemanticState>>
  runTrace(const state::SemanticState& initialState,
           const std::vector<Step>& steps) override;

  bool checkRefutation(const state::SemanticState& state,
                       const Evidence& evidence) override;

  bool checkContainment(const state::SemanticState& state1,
                        const state::SemanticState& state2,
                        const Evidence& evidence) override;

  bool checkCovering(const state::SemanticState& state,
                     const std::vector<std::unique_ptr<state::SemanticState>>& decomposedStates,
                     const Evidence& evidence) override;

  bool checkModelValidity(const state::SemanticState& state,
                          const core::Model& model) override;
};

} // namespace kernel
} // namespace semcal
