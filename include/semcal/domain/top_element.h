#ifndef SEMCAL_DOMAIN_TOP_ELEMENT_H
#define SEMCAL_DOMAIN_TOP_ELEMENT_H

#include "abstract_domain.h"

namespace semcal {
namespace domain {

/**
 * @brief A simple top element implementation for examples.
 * 
 * This represents the most abstract (least precise) element,
 * which concretizes to all possible models.
 */
class TopElement : public AbstractElement {
public:
    bool isLessPreciseThan(const AbstractElement& other) const override;
    bool equals(const AbstractElement& other) const override;
    std::string toString() const override;
    std::unique_ptr<AbstractElement> clone() const override;
};

} // namespace domain
} // namespace semcal

#endif // SEMCAL_DOMAIN_TOP_ELEMENT_H
