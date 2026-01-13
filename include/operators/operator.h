#pragma once
#include "../state/semantic_state.h"
#include "../util/op_result.h"

namespace semcal {
namespace operators {

/**
 * @brief Base class for all semantic operators.
 * 
 * All SemCal operators inherit from this base class.
 * Operators transform semantic states according to semantic axioms.
 */
class SemanticOperator {
public:
  virtual ~SemanticOperator() = default;
};

} // namespace operators
} // namespace semcal
