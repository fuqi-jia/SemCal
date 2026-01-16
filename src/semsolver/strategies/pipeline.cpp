#include "semsolver/strategies/pipeline.h"
#include "semcal/operators/restrict.h"
#include "semcal/operators/decompose.h"
#include "semcal/operators/infeasible.h"
#include "semcal/operators/relax.h"
#include "semcal/operators/refine.h"
#include "semcal/operators/shadow.h"
#include "semcal/operators/lift.h"
#include "semcal/core/semantics.h"
#include "semcal/domain/concretization.h"

namespace semcal {
namespace solver {
namespace strategies {

LegacyOperatorPipeline::LegacyOperatorPipeline(
    std::unique_ptr<semcal::operators::RestrictOp> restrict,
    std::unique_ptr<semcal::operators::DecomposeOp> decompose,
    std::unique_ptr<semcal::operators::InfeasibleOp> infeasible,
    std::unique_ptr<semcal::operators::RelaxOp> relax,
    std::unique_ptr<semcal::operators::RefineOp> refine,
    std::unique_ptr<semcal::operators::ShadowOp> shadow,
    std::unique_ptr<semcal::operators::LiftOp> lift,
    std::shared_ptr<semcal::core::Semantics> semantics,
    std::shared_ptr<semcal::domain::Concretization> concretization)
    : restrict_(std::move(restrict)),
      decompose_(std::move(decompose)),
      infeasible_(std::move(infeasible)),
      relax_(std::move(relax)),
      refine_(std::move(refine)),
      shadow_(std::move(shadow)),
      lift_(std::move(lift)),
      semantics_(std::move(semantics)),
      concretization_(std::move(concretization)) {
}

LegacyPipelineResult LegacyOperatorPipeline::execute(
    const semcal::state::SemanticState& initialState,
    std::function<LegacyPipelineResult(semcal::state::SemanticState&)> stepFunction) const {
    auto state = initialState.clone();
    return stepFunction(*state);
}

std::unique_ptr<LegacyOperatorPipeline> LegacyPipelineFactory::createDefault() {
    // Create default operator implementations
    auto restrict = std::make_unique<semcal::operators::DefaultRestrictOp>();
    auto decompose = std::make_unique<semcal::operators::DefaultDecomposeOp>();
    auto infeasible = std::make_unique<semcal::operators::DefaultInfeasibleOp>();
    auto relax = std::make_unique<semcal::operators::DefaultRelaxOp>();
    auto refine = std::make_unique<semcal::operators::DefaultRefineOp>();
    auto shadow = std::make_unique<semcal::operators::DefaultShadowOp>();
    auto lift = std::make_unique<semcal::operators::DefaultLiftOp>();
    
    // Create default semantics and concretization
    auto semantics = std::make_shared<semcal::core::DefaultSemantics>();
    auto concretization = std::make_shared<semcal::domain::DefaultConcretization>();
    
    return std::make_unique<LegacyOperatorPipeline>(
        std::move(restrict),
        std::move(decompose),
        std::move(infeasible),
        std::move(relax),
        std::move(refine),
        std::move(shadow),
        std::move(lift),
        semantics,
        concretization
    );
}

} // namespace strategies
} // namespace solver
} // namespace semcal
