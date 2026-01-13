#include "domain/abstract_domain.h"

namespace semcal {
namespace domain {

bool AbstractElement::isMorePreciseThan(const AbstractElement& other) const {
    return other.isLessPreciseThan(*this);
}

} // namespace domain
} // namespace semcal
