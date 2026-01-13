#ifndef SEMCAL_UTIL_RESULT_H
#define SEMCAL_UTIL_RESULT_H

#include <variant>
#include <string>
#include <memory>

namespace semcal {
namespace util {

/**
 * @brief Result type for operations that can succeed or fail.
 * 
 * Similar to Rust's Result<T, E> or std::expected.
 */
template<typename T, typename E = std::string>
class Result {
private:
    std::variant<T, E> value_;

public:
    /**
     * @brief Construct a successful result.
     */
    static Result<T, E> success(const T& value) {
        Result<T, E> result;
        result.value_ = value;
        return result;
    }

    /**
     * @brief Construct a successful result (move).
     */
    static Result<T, E> success(T&& value) {
        Result<T, E> result;
        result.value_ = std::move(value);
        return result;
    }

    /**
     * @brief Construct a failed result.
     */
    static Result<T, E> failure(const E& error) {
        Result<T, E> result;
        result.value_ = error;
        return result;
    }

    /**
     * @brief Construct a failed result (move).
     */
    static Result<T, E> failure(E&& error) {
        Result<T, E> result;
        result.value_ = std::move(error);
        return result;
    }

    /**
     * @brief Check if the result is successful.
     */
    bool isSuccess() const {
        return std::holds_alternative<T>(value_);
    }

    /**
     * @brief Check if the result is a failure.
     */
    bool isFailure() const {
        return std::holds_alternative<E>(value_);
    }

    /**
     * @brief Get the value (throws if failure).
     */
    T& getValue() {
        return std::get<T>(value_);
    }

    /**
     * @brief Get the value (const, throws if failure).
     */
    const T& getValue() const {
        return std::get<T>(value_);
    }

    /**
     * @brief Get the error (throws if success).
     */
    E& getError() {
        return std::get<E>(value_);
    }

    /**
     * @brief Get the error (const, throws if success).
     */
    const E& getError() const {
        return std::get<E>(value_);
    }

    /**
     * @brief Get the value or a default.
     */
    T getValueOr(const T& defaultValue) const {
        if (isSuccess()) {
            return std::get<T>(value_);
        }
        return defaultValue;
    }
};

} // namespace util
} // namespace semcal

#endif // SEMCAL_UTIL_RESULT_H
