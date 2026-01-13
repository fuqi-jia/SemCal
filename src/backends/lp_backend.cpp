#include "../../include/backends/lp_backend.h"
#include <sstream>

namespace semcal {
namespace backends {

// LpRelaxWitness implementation
std::string LpRelaxWitness::toString() const {
    std::ostringstream oss;
    oss << "LP Relaxation: type=" << relaxationType 
        << ", relaxed " << relaxedConstraints.size() << " constraints";
    return oss.str();
}

// LpUnsatWitness implementation
std::string LpUnsatWitness::toString() const {
    std::ostringstream oss;
    oss << "LP Infeasibility (Farkas): " << farkasCoefficients.size() << " coefficients";
    return oss.str();
}

// LpCutWitness implementation
std::string LpCutWitness::toString() const {
    std::ostringstream oss;
    oss << "LP Cut: type=" << cutType << ", constraint=" << cutConstraint;
    return oss.str();
}

// LpBackend implementation
bool LpBackend::supportsOperator(const std::string& opName) const {
    return opName == "Relax" || opName == "Infeasible" || 
           opName == "Refine" || opName == "Optimize";
}

ApproxDirection LpBackend::getApproxDirection(const std::string& opName) const {
    if (opName == "Relax") return ApproxDirection::OVER_APPROX; // default
    if (opName == "Infeasible") return ApproxDirection::REFUTE_CERTIFIED;
    if (opName == "Refine") return ApproxDirection::PRESERVING;
    return ApproxDirection::PRESERVING;
}

} // namespace backends
} // namespace semcal
