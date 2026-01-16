#ifndef SEMX_H
#define SEMX_H

/**
 * @file semx.h
 * @brief Umbrella header for SemX - A Verified Semantic Framework for Solver Construction
 * 
 * Include this header to access all SemX functionality.
 */

// SemCal: Axiomatic Semantic Calculus
#include "semcal/core/model.h"
#include "semcal/core/partial_model.h"
#include "semcal/core/formula.h"
#include "semcal/core/semantics.h"
#include "semcal/domain/abstract_domain.h"
#include "semcal/domain/concretization.h"
#include "semcal/domain/galois.h"
#include "semcal/domain/top_element.h"
#include "semcal/state/semantic_state.h"
#include "semcal/operators/restrict.h"
#include "semcal/operators/decompose.h"
#include "semcal/operators/infeasible.h"
#include "semcal/operators/relax.h"
#include "semcal/operators/refine.h"
#include "semcal/operators/shadow.h"
#include "semcal/operators/lift.h"
#include "semcal/backends/cad_backend.h"
#include "semcal/backends/lp_backend.h"
#include "semcal/backends/icp_backend.h"
#include "semcal/util/op_result.h"

// SemKernel: Verified Semantic Kernel
#include "semkernel/kernel.h"

// SemSearch: Generic Search and Execution Engine
#include "semsearch/search_engine.h"

// SemSolver: Concrete Solver Instances
#include "semsolver/solver.h"

// Legacy strategies (deprecated - use SemSolver instead)
#include "semsolver/strategies/pipeline.h"
#include "semsolver/strategies/strategy.h"

/**
 * @namespace semcal
 * @brief Main namespace for SemX library
 * 
 * SemX is organized into four strictly layered subsystems:
 * - SemCal: Axiomatic Semantic Calculus (operators, axioms)
 * - SemKernel: Verified Semantic Kernel (validation)
 * - SemSearch: Generic Search and Execution Engine
 * - SemSolver: Concrete Solver Instances
 */
namespace semcal {
    // SemCal sub-namespaces:
    // - semcal::core (models, formulas, semantics)
    // - semcal::domain (abstract domains, concretization)
    // - semcal::state (semantic states)
    // - semcal::operators (semantic operators)
    // - semcal::backends (backend adapters)
    // - semcal::util (utilities)
    
    // SemKernel namespace:
    // - semcal::kernel (verified semantic validation)
    
    // SemSearch namespace:
    // - semcal::search (generic search engine)
    
    // SemSolver namespace:
    // - semcal::solver (concrete solver instances)
}

#endif // SEMX_H
