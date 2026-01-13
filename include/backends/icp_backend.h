#ifndef SEMCAL_BACKENDS_ICP_BACKEND_H
#define SEMCAL_BACKENDS_ICP_BACKEND_H

#include "backend_capability.h"
#include "../core/formula.h"
#include "../domain/abstract_domain.h"
#include "../state/semantic_state.h"
#include "../util/result.h"
#include <vector>
#include <memory>
#include <string>

namespace semcal {
namespace backends {

/**
 * @brief Witness for ICP contraction operation.
 * 
 * Contains information about how the interval was contracted.
 */
struct IcpContractWitness {
    std::vector<std::string> contractedVars;
    std::string contractionMethod; // e.g., "Newton", "HC4", "Box"
    std::string roundingMode; // e.g., "directed-rounding", "high-precision"
    std::string toString() const;
};

/**
 * @brief Witness for ICP decomposition (box splitting).
 */
struct IcpDecompWitness {
    size_t numBoxes;
    std::string splitVar;
    double splitPoint;
    std::string toString() const;
};

/**
 * @brief ICP (Interval Constraint Propagation) backend capability interface.
 * 
 * ICP provides:
 * - Restrict: Interval contraction (sound narrowing)
 * - Decompose: Box splitting
 * - Infeasible: Empty box detection
 */
class IcpBackend : public BackendCapability {
public:
    virtual ~IcpBackend() = default;

    // BackendCapability interface
    std::string getName() const override { return "ICP"; }
    bool supportsOperator(const std::string& opName) const override;
    ApproxDirection getApproxDirection(const std::string& opName) const override;

    /**
     * @brief Contract intervals (Axiom R / Restrict).
     * 
     * Contract((F, a)) returns (F, a') such that:
     * [[F]] ∩ γ(a') = [[F]] ∩ γ(a)  (sound contraction)
     * and γ(a') ⊆ γ(a)  (narrowing)
     * 
     * Note: Must use directed rounding or high precision to ensure soundness.
     * 
     * @param formula The constraint F
     * @param abstractElement The abstract element a (must be a box/interval)
     * @return Result with contracted abstract element and witness
     */
    virtual util::OpResult<
        std::unique_ptr<domain::AbstractElement>,
        IcpContractWitness
    > contract(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement) = 0;

    /**
     * @brief Decompose by splitting boxes (Axiom D).
     * 
     * Decompose((F, a)) returns {(F, a_i)} such that:
     * [[F]] ∩ γ(a) ⊆ ∪_i ([[F]] ∩ γ(a_i))
     * 
     * @param formula The constraint F
     * @param abstractElement The abstract element a (must be a box)
     * @return Result with vector of decomposed states and witness
     */
    virtual util::OpResult<
        std::vector<std::unique_ptr<state::SemanticState>>,
        IcpDecompWitness
    > decompose(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement) = 0;

    /**
     * @brief Check if box is empty (Axiom I).
     * 
     * If infeasible((F, a)) returns UNSAT, then:
     * [[F]] ∩ γ(a) = ∅
     * 
     * @param formula The constraint F
     * @param abstractElement The abstract element a (must be a box)
     * @return Result with UNSAT status if box is empty
     */
    virtual util::OpResult<void, std::monostate> infeasible(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement) = 0;
};

} // namespace backends
} // namespace semcal

#endif // SEMCAL_BACKENDS_ICP_BACKEND_H
