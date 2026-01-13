#include "../../include/backends/icp_backend.h"
#include <sstream>

namespace semcal {
namespace backends {

// IcpContractWitness implementation
std::string IcpContractWitness::toString() const {
    std::ostringstream oss;
    oss << "ICP Contraction: method=" << contractionMethod 
        << ", rounding=" << roundingMode 
        << ", contracted " << contractedVars.size() << " variables";
    return oss.str();
}

// IcpDecompWitness implementation
std::string IcpDecompWitness::toString() const {
    std::ostringstream oss;
    oss << "ICP Decomposition: " << numBoxes << " boxes, split on " 
        << splitVar << " at " << splitPoint;
    return oss.str();
}

// IcpBackend implementation
bool IcpBackend::supportsOperator(const std::string& opName) const {
    return opName == "Restrict" || opName == "Decompose" || opName == "Infeasible";
}

ApproxDirection IcpBackend::getApproxDirection(const std::string& opName) const {
    if (opName == "Restrict") return ApproxDirection::PRESERVING;
    if (opName == "Decompose") return ApproxDirection::PRESERVING;
    if (opName == "Infeasible") return ApproxDirection::REFUTE_CERTIFIED;
    return ApproxDirection::PRESERVING;
}

} // namespace backends
} // namespace semcal
