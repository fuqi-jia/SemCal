#ifndef SEMCAL_DOMAIN_ABSTRACT_DOMAIN_H
#define SEMCAL_DOMAIN_ABSTRACT_DOMAIN_H

#include <memory>
#include <string>

namespace semcal {
namespace domain {

/**
 * @brief Abstract domain element.
 * 
 * Represents an element a ∈ A in the abstract domain (A, ⊑).
 * The partial order ⊑ represents the precision ordering.
 */
class AbstractElement {
public:
    virtual ~AbstractElement() = default;

    /**
     * @brief Check if this element is less precise than or equal to another.
     * 
     * a ⊑ b means a is less precise (more abstract) than b.
     * 
     * @param other The other abstract element
     * @return true if this ⊑ other
     */
    virtual bool isLessPreciseThan(const AbstractElement& other) const = 0;

    /**
     * @brief Check if this element is more precise than or equal to another.
     * 
     * a ⊒ b means a is more precise (less abstract) than b.
     * 
     * @param other The other abstract element
     * @return true if this ⊒ other
     */
    virtual bool isMorePreciseThan(const AbstractElement& other) const;

    /**
     * @brief Check equality with another abstract element.
     * @param other The other abstract element
     * @return true if elements are equal
     */
    virtual bool equals(const AbstractElement& other) const = 0;

    /**
     * @brief Get a string representation.
     * @return String representation
     */
    virtual std::string toString() const = 0;

    /**
     * @brief Clone the abstract element.
     * @return A new copy
     */
    virtual std::unique_ptr<AbstractElement> clone() const = 0;
};

/**
 * @brief Abstract domain interface.
 * 
 * Defines the structure (A, ⊑) of an abstract domain.
 */
class AbstractDomain {
public:
    virtual ~AbstractDomain() = default;

    /**
     * @brief Get the bottom element (most abstract, least precise).
     * @return The bottom element
     */
    virtual std::unique_ptr<AbstractElement> bottom() const = 0;

    /**
     * @brief Get the top element (most concrete, most precise).
     * @return The top element
     */
    virtual std::unique_ptr<AbstractElement> top() const = 0;

    /**
     * @brief Compute the least upper bound (join).
     * 
     * a ⊔ b is the least precise element that is more precise than both a and b.
     * 
     * @param a First element
     * @param b Second element
     * @return The join a ⊔ b
     */
    virtual std::unique_ptr<AbstractElement> join(
        const AbstractElement& a,
        const AbstractElement& b) const = 0;

    /**
     * @brief Compute the greatest lower bound (meet).
     * 
     * a ⊓ b is the most precise element that is less precise than both a and b.
     * 
     * @param a First element
     * @param b Second element
     * @return The meet a ⊓ b
     */
    virtual std::unique_ptr<AbstractElement> meet(
        const AbstractElement& a,
        const AbstractElement& b) const = 0;
};

} // namespace domain
} // namespace semcal

#endif // SEMCAL_DOMAIN_ABSTRACT_DOMAIN_H
