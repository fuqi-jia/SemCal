#pragma once
#include <optional>
#include <variant>
#include <string>
#include <type_traits>

namespace semcal {
namespace util {

enum class OpStatus {
  OK,
  UNSAT,
  UNKNOWN,
  PARTIAL,
  ERROR
};

// Forward declaration
template <class T, class Witness = std::monostate>
struct OpResult;

// Specialization for void type (must come before general template)
template <class Witness>
struct OpResult<void, Witness> {
  OpStatus status;
  Witness witness;

  static OpResult<void, Witness> ok() {
    return {OpStatus::OK, {}};
  }

  static OpResult<void, Witness> unsat(Witness w = {}) {
    return {OpStatus::UNSAT, w};
  }

  static OpResult<void, Witness> unknown() {
    return {OpStatus::UNKNOWN, {}};
  }

  static OpResult<void, Witness> error() {
    return {OpStatus::ERROR, {}};
  }

  // For void, we don't have a value, so has_value() checks status
  bool has_value() const {
    return status == OpStatus::OK || status == OpStatus::PARTIAL;
  }
};

// General template for non-void types
template <class T, class Witness>
struct OpResult {
  OpStatus status;
  std::optional<T> value;
  Witness witness;

  static OpResult<T, Witness> ok(T v) {
    return {OpStatus::OK, std::move(v), {}};
  }

  static OpResult<T, Witness> unsat(Witness w = {}) {
    return {OpStatus::UNSAT, std::nullopt, w};
  }

  static OpResult<T, Witness> unknown() {
    return {OpStatus::UNKNOWN, std::nullopt, {}};
  }

  static OpResult<T, Witness> partial(T v) {
    return {OpStatus::PARTIAL, std::move(v), {}};
  }

  static OpResult<T, Witness> error() {
    return {OpStatus::ERROR, std::nullopt, {}};
  }

  bool has_value() const {
    return value.has_value();
  }
};

} // namespace util
} // namespace semcal
