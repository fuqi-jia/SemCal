#include "orchestration/pipeline.h"
#include "operators/restrict.h"
#include "operators/decompose.h"
#include "operators/infeasible.h"
#include "operators/relax.h"
#include "operators/refine.h"
#include "operators/project.h"
#include "operators/lift.h"
#include "core/semantics.h"
#include "domain/concretization.h"

namespace semcal {
namespace orchestration {

Pipeline::Pipeline(
    std::unique_ptr<operators::Restrict> restrict,
    std::unique_ptr<operators::DecomposeOp> decompose,
    std::unique_ptr<operators::InfeasibleOp> infeasible,
    std::unique_ptr<operators::Relax> relax,
    std::unique_ptr<operators::Refine> refine,
    std::unique_ptr<operators::ProjectOp> project,
    std::unique_ptr<operators::LiftOp> lift,
    std::shared_ptr<core::Semantics> semantics,
    std::shared_ptr<domain::Concretization> concretization)
    : restrict_(std::move(restrict)),
      decompose_(std::move(decompose)),
      infeasible_(std::move(infeasible)),
      relax_(std::move(relax)),
      refine_(std::move(refine)),
      project_(std::move(project)),
      lift_(std::move(lift)),
      semantics_(std::move(semantics)),
      concretization_(std::move(concretization)) {
}

PipelineResult Pipeline::execute(
    const state::SemanticState& initialState,
    std::function<PipelineResult(state::SemanticState&)> stepFunction) const {
    auto state = initialState.clone();
    return stepFunction(*state);
}

std::unique_ptr<Pipeline> PipelineFactory::createDefault() {
    // Create default operator implementations
    auto restrict = std::make_unique<operators::DefaultRestrict>();
    auto decompose = std::make_unique<operators::DefaultDecomposeOp>();
    auto infeasible = std::make_unique<operators::DefaultInfeasibleOp>();
    auto relax = std::make_unique<operators::DefaultRelax>();
    auto refine = std::make_unique<operators::DefaultRefine>();
    auto project = std::make_unique<operators::DefaultProjectOp>();
    auto lift = std::make_unique<operators::DefaultLiftOp>();
    
    // Create default semantics and concretization
    auto semantics = std::make_shared<core::DefaultSemantics>();
    auto concretization = std::make_shared<domain::DefaultConcretization>();
    
    return std::make_unique<Pipeline>(
        std::move(restrict),
        std::move(decompose),
        std::move(infeasible),
        std::move(relax),
        std::move(refine),
        std::move(project),
        std::move(lift),
        semantics,
        concretization
    );
}

} // namespace orchestration
} // namespace semcal
