#ifndef SEMCAL_BACKENDS_BACKEND_CAPABILITY_H
#define SEMCAL_BACKENDS_BACKEND_CAPABILITY_H

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
 * @brief Approximation direction for operators.
 * 
 * Specifies how an operator approximates the concrete meaning:
 * - PRESERVING: Exact (no approximation)
 * - OVER_APPROX: Over-approximation (may introduce spurious models)
 * - UNDER_APPROX: Under-approximation (may miss real models)
 * - REFUTE_CERTIFIED: Only used for refutation (UNSAT claims)
 */
enum class ApproxDirection {
    PRESERVING,      // Exact: Conc(σ) = Conc(σ')
    OVER_APPROX,     // Over-approx: Conc(σ) ⊆ Conc(σ')
    UNDER_APPROX,    // Under-approx: Conc(σ') ⊆ Conc(σ)
    REFUTE_CERTIFIED // Only for refutation: if UNSAT, then Conc(σ) = ∅
};

/**
 * @brief Base interface for backend capabilities.
 * 
 * All backends must declare which operators they support
 * and their approximation directions.
 */
class BackendCapability {
public:
    virtual ~BackendCapability() = default;

    /**
     * @brief Get the name of this backend.
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Check if this backend supports a specific operator.
     * 
     * @param opName Name of the operator (e.g., "Restrict", "Decompose", "Infeasible")
     * @return true if supported
     */
    virtual bool supportsOperator(const std::string& opName) const = 0;

    /**
     * @brief Get the approximation direction for an operator.
     * 
     * @param opName Name of the operator
     * @return The approximation direction, or PRESERVING if unknown
     */
    virtual ApproxDirection getApproxDirection(const std::string& opName) const = 0;
};

} // namespace backends
} // namespace semcal

#endif // SEMCAL_BACKENDS_BACKEND_CAPABILITY_H
