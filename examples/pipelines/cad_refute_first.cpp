/**
 * @file cad_refute_first.cpp
 * @brief Example: Refute-first pipeline using CAD backend
 * 
 * This demonstrates a "refute-first" pipeline that:
 * 1. Uses ICP to optionally restrict/narrow the search space
 * 2. Uses CAD to decompose into cells
 * 3. Uses CAD to refute infeasible cells
 * 4. Continues with further decomposition/refinement if needed
 * 
 * This pipeline showcases how SemCal operators can be orchestrated
 * without defining a monolithic CAD solver.
 */

#include "semx.h"
#include "../../include/backends/cad_backend.h"
#include "../../include/backends/icp_backend.h"
#include "../../include/operators/infeasible.h"
#include "../../include/operators/decompose.h"
#include "../../include/operators/restrict.h"
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using namespace semcal;

/**
 * @brief Refute-first solver pipeline using CAD and ICP backends.
 */
class RefuteFirstSolver {
private:
    std::unique_ptr<backends::CadBackend> cadBackend_;
    std::unique_ptr<backends::IcpBackend> icpBackend_;

public:
    RefuteFirstSolver(
        std::unique_ptr<backends::CadBackend> cad,
        std::unique_ptr<backends::IcpBackend> icp)
        : cadBackend_(std::move(cad)), icpBackend_(std::move(icp)) {}

    /**
     * @brief Solve using refute-first strategy.
     * 
     * Pipeline:
     * 1. σ0 = (F, Top)
     * 2. σ1 = Restrict_ICP(σ0) [optional: narrow box]
     * 3. S = Decompose_CAD(σ1) [cell decomposition]
     * 4. For each σi in S:
     *    - If Infeasible_CAD(σi) = UNSAT: prune
     *    - Otherwise: continue with further split/refine
     * 
     * @param initialState Initial semantic state (F, a)
     * @return Vector of feasible states (empty if UNSAT)
     */
    std::vector<std::unique_ptr<state::SemanticState>> solve(
        std::unique_ptr<state::SemanticState> initialState) {
        
        std::vector<std::unique_ptr<state::SemanticState>> feasibleStates;
        std::queue<std::unique_ptr<state::SemanticState>> workQueue;
        
        // Step 1: Initial state
        workQueue.push(std::move(initialState));
        
        // Step 2: Optional ICP restriction (narrow the box)
        if (icpBackend_) {
            std::queue<std::unique_ptr<state::SemanticState>> restrictedQueue;
            while (!workQueue.empty()) {
                auto state = std::move(workQueue.front());
                workQueue.pop();
                
                // Apply ICP contraction
                auto contractResult = icpBackend_->contract(
                    state->getFormula(),
                    state->getAbstractElement()
                );
                
                if (contractResult.status == util::OpStatus::OK) {
                    // Create new state with contracted abstract element
                    auto newState = std::make_unique<state::SemanticState>(
                        state->getFormula().clone(),
                        std::move(contractResult.value)
                    );
                    restrictedQueue.push(std::move(newState));
                    
                    std::cout << "  [ICP] Contracted: " 
                              << contractResult.witness.toString() << std::endl;
                } else {
                    // If contraction fails, keep original
                    restrictedQueue.push(std::move(state));
                }
            }
            workQueue = std::move(restrictedQueue);
        }
        
        // Step 3: CAD decomposition and refutation loop
        int iteration = 0;
        const int maxIterations = 100;
        
        while (!workQueue.empty() && iteration < maxIterations) {
            auto state = std::move(workQueue.front());
            workQueue.pop();
            iteration++;
            
            std::cout << "\n[Iteration " << iteration << "] Processing state: "
                      << state->toString() << std::endl;
            
            // Step 3a: Check infeasibility first (refute-first strategy)
            auto refuteResult = cadBackend_->refute(
                state->getFormula(),
                state->getAbstractElement()
            );
            
            if (refuteResult.status == util::OpStatus::UNSAT) {
                std::cout << "  [CAD] Refuted: " 
                          << refuteResult.witness.toString() << std::endl;
                // Prune this branch
                continue;
            }
            
            // Step 3b: Decompose into cells
            auto decompResult = cadBackend_->decompose(
                state->getFormula(),
                state->getAbstractElement()
            );
            
            if (decompResult.status == util::OpStatus::OK) {
                std::cout << "  [CAD] Decomposed into " 
                          << decompResult.value.size() << " cells: "
                          << decompResult.witness.toString() << std::endl;
                
                // Process each decomposed cell
                for (auto& cellState : decompResult.value) {
                    // Check if this cell is feasible
                    auto cellRefuteResult = cadBackend_->refute(
                        cellState->getFormula(),
                        cellState->getAbstractElement()
                    );
                    
                    if (cellRefuteResult.status == util::OpStatus::UNSAT) {
                        std::cout << "    [CAD] Cell refuted" << std::endl;
                        // Prune
                    } else if (cellRefuteResult.status == util::OpStatus::OK) {
                        // Potentially feasible - add to work queue for further processing
                        // or mark as feasible if sufficiently refined
                        workQueue.push(std::move(cellState));
                    } else {
                        // UNKNOWN - treat as potentially feasible
                        workQueue.push(std::move(cellState));
                    }
                }
            } else {
                // Decomposition failed or returned UNKNOWN
                // Treat as potentially feasible
                feasibleStates.push_back(std::move(state));
            }
        }
        
        // Collect remaining states as feasible
        while (!workQueue.empty()) {
            feasibleStates.push_back(std::move(workQueue.front()));
            workQueue.pop();
        }
        
        return feasibleStates;
    }
};

/**
 * @brief Stub CAD backend implementation for demonstration.
 */
class StubCadBackend : public backends::CadBackend {
public:
    util::OpResult<
        std::pair<std::unique_ptr<core::Formula>, std::unique_ptr<domain::AbstractElement>>,
        backends::CadProjectWitness
    > project(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement,
        const std::vector<std::string>& varsToElim) override {
        // Stub implementation
        backends::CadProjectWitness witness;
        witness.eliminatedVars = varsToElim;
        witness.projectionPolynomials = {"p1", "p2"};
        
        return util::OpResult<
            std::pair<std::unique_ptr<core::Formula>, std::unique_ptr<domain::AbstractElement>>,
            backends::CadProjectWitness
        >(util::OpStatus::UNKNOWN, 
          std::make_pair(formula.clone(), abstractElement.clone()),
          witness);
    }
    
    util::OpResult<
        std::vector<std::unique_ptr<state::SemanticState>>,
        backends::CadDecompWitness
    > decompose(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement) override {
        // Stub: return single cell
        backends::CadDecompWitness witness;
        witness.numCells = 1;
        witness.decompositionType = "sign-invariant";
        
        auto state = std::make_unique<state::SemanticState>(
            formula.clone(),
            abstractElement.clone()
        );
        
        std::vector<std::unique_ptr<state::SemanticState>> cells;
        cells.push_back(std::move(state));
        
        return util::OpResult<
            std::vector<std::unique_ptr<state::SemanticState>>,
            backends::CadDecompWitness
        >(util::OpStatus::OK, std::move(cells), witness);
    }
    
    util::OpResult<void, backends::CadUnsatWitness> refute(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement) override {
        // Stub: always return UNKNOWN (not refuted)
        backends::CadUnsatWitness witness;
        witness.refutationMethod = "sample-point-evaluation";
        
        return util::OpResult<void, backends::CadUnsatWitness>(
            util::OpStatus::UNKNOWN, void{}, witness);
    }
    
    util::OpResult<
        std::unique_ptr<state::SemanticState>,
        std::monostate
    > lift(
        const core::Formula& formula,
        const domain::AbstractElement& projectedElement,
        const std::vector<std::string>& vars) override {
        // Stub implementation
        auto state = std::make_unique<state::SemanticState>(
            formula.clone(),
            projectedElement.clone()
        );
        
        return util::OpResult<
            std::unique_ptr<state::SemanticState>,
            std::monostate
        >(util::OpStatus::OK, std::move(state), std::monostate{});
    }
};

/**
 * @brief Stub ICP backend implementation for demonstration.
 */
class StubIcpBackend : public backends::IcpBackend {
public:
    util::OpResult<
        std::unique_ptr<domain::AbstractElement>,
        backends::IcpContractWitness
    > contract(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement) override {
        // Stub: return same element
        backends::IcpContractWitness witness;
        witness.contractionMethod = "HC4";
        witness.roundingMode = "directed-rounding";
        
        return util::OpResult<
            std::unique_ptr<domain::AbstractElement>,
            backends::IcpContractWitness
        >(util::OpStatus::OK, abstractElement.clone(), witness);
    }
    
    util::OpResult<
        std::vector<std::unique_ptr<state::SemanticState>>,
        backends::IcpDecompWitness
    > decompose(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement) override {
        // Stub implementation
        backends::IcpDecompWitness witness;
        witness.numBoxes = 1;
        
        auto state = std::make_unique<state::SemanticState>(
            formula.clone(),
            abstractElement.clone()
        );
        
        std::vector<std::unique_ptr<state::SemanticState>> boxes;
        boxes.push_back(std::move(state));
        
        return util::OpResult<
            std::vector<std::unique_ptr<state::SemanticState>>,
            backends::IcpDecompWitness
        >(util::OpStatus::OK, std::move(boxes), witness);
    }
    
    util::OpResult<void, std::monostate> infeasible(
        const core::Formula& formula,
        const domain::AbstractElement& abstractElement) override {
        // Stub: always return UNKNOWN
        return util::OpResult<void, std::monostate>(
            util::OpStatus::UNKNOWN, void{}, std::monostate{});
    }
};

int main() {
    std::cout << "SemCal Refute-First Pipeline Example" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "\nThis example demonstrates:" << std::endl;
    std::cout << "1. ICP Restrict (optional narrowing)" << std::endl;
    std::cout << "2. CAD Decompose (cell decomposition)" << std::endl;
    std::cout << "3. CAD Infeasible (refutation)" << std::endl;
    std::cout << "4. Soundness by construction via SemCal operators\n" << std::endl;
    
    // Create backends
    auto cadBackend = std::make_unique<StubCadBackend>();
    auto icpBackend = std::make_unique<StubIcpBackend>();
    
    // Create solver
    RefuteFirstSolver solver(std::move(cadBackend), std::move(icpBackend));
    
    // Create initial state: (F, Top)
    auto formula = std::make_unique<core::ConcreteFormula>("(and (> x 0) (< x 10) (> y 0) (< y 10))");
    auto topElement = std::make_unique<domain::TopElement>();
    auto initialState = std::make_unique<state::SemanticState>(
        std::move(formula),
        std::move(topElement)
    );
    
    std::cout << "Initial state: " << initialState->toString() << std::endl;
    std::cout << "\nRunning refute-first pipeline...\n" << std::endl;
    
    // Solve
    auto feasibleStates = solver.solve(std::move(initialState));
    
    std::cout << "\n=== Results ===" << std::endl;
    std::cout << "Feasible states found: " << feasibleStates.size() << std::endl;
    
    if (feasibleStates.empty()) {
        std::cout << "Result: UNSAT (all cells refuted)" << std::endl;
    } else {
        std::cout << "Result: SAT (found " << feasibleStates.size() << " feasible region(s))" << std::endl;
    }
    
    return 0;
}
