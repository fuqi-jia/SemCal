#include "domain/top_element.h"

namespace semcal {
namespace domain {

bool TopElement::isLessPreciseThan(const AbstractElement& other) const {
    // Top is less precise than everything except itself
    return !equals(other);
}

bool TopElement::equals(const AbstractElement& other) const {
    return dynamic_cast<const TopElement*>(&other) != nullptr;
}

std::string TopElement::toString() const {
    return "⊤";
}

std::unique_ptr<AbstractElement> TopElement::clone() const {
    return std::make_unique<TopElement>();
}

} // namespace domain
} // namespace semcal
