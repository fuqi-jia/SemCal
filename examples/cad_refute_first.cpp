/**
 * @file cad_refute_first.cpp
 * @brief Example: Refute-first pipeline using CAD backend
 * 
 * This demonstrates a "refute-first" pipeline that:
 * 1. Uses CAD to decompose into cells
 * 2. Uses CAD to refute infeasible cells
 * 3. Continues with further decomposition/refinement if needed
 * 
 * This pipeline showcases how SemCal operators can be orchestrated
 * without defining a monolithic CAD solver.
 */

#include "semcal.h"
#include "../include/backends/cad_stub.h"
#include "../include/operators/infeasible_cad.h"
#include "../include/operators/decompose_cad.h"
#include <iostream>
#include <memory>

using namespace semcal;
using namespace semcal::operators;
using namespace semcal::backends;

int main() {
  std::cout << "SemCal Refute-First Pipeline Example" << std::endl;
  std::cout << "=====================================" << std::endl;
  std::cout << "\nThis example demonstrates:" << std::endl;
  std::cout << "1. CAD Decompose (cell decomposition)" << std::endl;
  std::cout << "2. CAD Infeasible (refutation)" << std::endl;
  std::cout << "3. Soundness by construction via SemCal operators\n" << std::endl;

  // Create initial state: (F, Top)
  auto formula = std::make_unique<core::ConcreteFormula>("(and (> x 0) (< x 10) (> y 0) (< y 10))");
  auto topElement = std::make_unique<domain::TopElement>();
  auto σ0 = std::make_unique<state::SemanticState>(
    std::move(formula),
    std::move(topElement)
  );

  std::cout << "Initial state: " << σ0->toString() << std::endl;
  std::cout << "\nRunning refute-first pipeline...\n" << std::endl;

  // Create CAD backend (stub)
  // In real implementation: auto cad = std::make_unique<CadLibpolyBackend>();
  CadStubBackend cad;
  
  std::cout << "[Step 1] Checking infeasibility..." << std::endl;
  CadInfeasibleOp infeasible(cad);
  auto r = infeasible.apply(*σ0);
  if (r.status == util::OpStatus::UNSAT) {
    std::cout << "  [CAD] Refuted: " << r.witness.explanation << std::endl;
    std::cout << "Result: UNSAT" << std::endl;
    return 0;
  }
  std::cout << "  [CAD] Not refuted (UNKNOWN)" << std::endl;

  std::cout << "\n[Step 2] Decomposing into cells..." << std::endl;
  CadDecomposeOp decompose(cad);
  auto parts = decompose.apply(*σ0);
  if (parts.status == util::OpStatus::OK && parts.value.has_value()) {
    std::cout << "  [CAD] Decomposed into " << parts.value->size() << " cells" << std::endl;
    for (size_t i = 0; i < parts.value->size(); ++i) {
      auto& σi = parts.value->at(i);
      auto r = infeasible.apply(*σi);
      if (r.status == util::OpStatus::UNSAT) {
        std::cout << "    [CAD] Cell " << i << " refuted: " << r.witness.explanation << std::endl;
      } else {
        std::cout << "    [CAD] Cell " << i << " not refuted" << std::endl;
      }
    }
  }

  std::cout << "\n=== Results ===" << std::endl;
  std::cout << "Note: This is a stub implementation." << std::endl;
  std::cout << "Replace CadStubBackend with actual CAD backend to get real results." << std::endl;

  return 0;
}
