#include "../../include/backends/cad_backend.h"
#include <sstream>

namespace semcal {
namespace backends {

// CadProjectWitness implementation
std::string CadProjectWitness::toString() const {
    std::ostringstream oss;
    oss << "CAD Projection: eliminated " << eliminatedVars.size() 
        << " variables, " << projectionPolynomials.size() << " projection polynomials";
    return oss.str();
}

// CadDecompWitness implementation
std::string CadDecompWitness::toString() const {
    std::ostringstream oss;
    oss << "CAD Decomposition: " << numCells << " cells, type: " << decompositionType;
    return oss.str();
}

// CadUnsatWitness implementation
std::string CadUnsatWitness::toString() const {
    std::ostringstream oss;
    oss << "CAD Refutation: method=" << refutationMethod 
        << ", conflicting constraints=" << conflictingConstraints.size();
    return oss.str();
}

// CadBackend implementation
bool CadBackend::supportsOperator(const std::string& opName) const {
    return opName == "Project" || opName == "Decompose" || 
           opName == "Infeasible" || opName == "Lift";
}

ApproxDirection CadBackend::getApproxDirection(const std::string& opName) const {
    if (opName == "Project") return ApproxDirection::OVER_APPROX;
    if (opName == "Decompose") return ApproxDirection::PRESERVING;
    if (opName == "Infeasible") return ApproxDirection::REFUTE_CERTIFIED;
    if (opName == "Lift") return ApproxDirection::UNDER_APPROX;
    return ApproxDirection::PRESERVING;
}

} // namespace backends
} // namespace semcal
