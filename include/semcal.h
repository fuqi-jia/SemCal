#ifndef SEMCAL_H
#define SEMCAL_H

/**
 * @file semcal.h
 * @brief Umbrella header for SemCal - A Semantic Calculus for Constraint Reasoning
 * 
 * Include this header to access all SemCal functionality.
 */

// Core modules
#include "core/model.h"
#include "core/formula.h"
#include "core/semantics.h"

// Domain modules
#include "domain/abstract_domain.h"
#include "domain/concretization.h"
#include "domain/galois.h"
#include "domain/top_element.h"

// State module
#include "state/semantic_state.h"

// Operator modules
#include "operators/restrict.h"
#include "operators/decompose.h"
#include "operators/infeasible.h"
#include "operators/relax.h"
#include "operators/refine.h"
#include "operators/project.h"
#include "operators/lift.h"

// Orchestration modules
#include "orchestration/pipeline.h"
#include "orchestration/strategy.h"

// Utility modules
#include "util/result.h"

/**
 * @namespace semcal
 * @brief Main namespace for SemCal library
 */
namespace semcal {
    // All SemCal functionality is in sub-namespaces:
    // - semcal::core
    // - semcal::domain
    // - semcal::state
    // - semcal::operators
    // - semcal::orchestration
    // - semcal::util
}

#endif // SEMCAL_H
