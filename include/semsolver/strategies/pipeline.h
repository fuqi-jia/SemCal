#ifndef SEMCAL_SOLVER_STRATEGIES_PIPELINE_H
#define SEMCAL_SOLVER_STRATEGIES_PIPELINE_H

#include "../../semcal/state/semantic_state.h"
#include "../../semcal/operators/restrict.h"
#include "../../semcal/operators/decompose.h"
#include "../../semcal/operators/infeasible.h"
#include "../../semcal/operators/relax.h"
#include "../../semcal/operators/refine.h"
#include "../../semcal/operators/shadow.h"
#include "../../semcal/operators/lift.h"
#include "../../semcal/core/semantics.h"
#include "../../semcal/domain/concretization.h"
#include <vector>
#include <memory>
#include <functional>

namespace semcal {
namespace solver {
namespace strategies {

/**
 * @brief Result of a pipeline execution.
 */
enum class LegacyPipelineResult {
    Success,
    Infeasible,
    Timeout,
    Error
};

/**
 * @brief Legacy operator pipeline (deprecated).
 * 
 * @deprecated This class is deprecated. Use SemSolver with SolverLegacySearchStrategy instead.
 * 
 * A pipeline is a sequence of semantic operators applied to semantic states.
 * This was the old way of orchestrating operators before SemX architecture.
 */
class LegacyOperatorPipeline {
private:
    std::unique_ptr<semcal::operators::RestrictOp> restrict_;
    std::unique_ptr<semcal::operators::DecomposeOp> decompose_;
    std::unique_ptr<semcal::operators::InfeasibleOp> infeasible_;
    std::unique_ptr<semcal::operators::RelaxOp> relax_;
    std::unique_ptr<semcal::operators::RefineOp> refine_;
    std::unique_ptr<semcal::operators::ShadowOp> shadow_;
    std::unique_ptr<semcal::operators::LiftOp> lift_;

    std::shared_ptr<semcal::core::Semantics> semantics_;
    std::shared_ptr<semcal::domain::Concretization> concretization_;

public:
    /**
     * @brief Construct a pipeline with operator instances.
     */
    LegacyOperatorPipeline(
        std::unique_ptr<semcal::operators::RestrictOp> restrict,
        std::unique_ptr<semcal::operators::DecomposeOp> decompose,
        std::unique_ptr<semcal::operators::InfeasibleOp> infeasible,
        std::unique_ptr<semcal::operators::RelaxOp> relax,
        std::unique_ptr<semcal::operators::RefineOp> refine,
        std::unique_ptr<semcal::operators::ShadowOp> shadow,
        std::unique_ptr<semcal::operators::LiftOp> lift,
        std::shared_ptr<semcal::core::Semantics> semantics,
        std::shared_ptr<semcal::domain::Concretization> concretization);

    /**
     * @brief Execute the pipeline on a semantic state.
     * 
     * @param initialState The initial semantic state
     * @param stepFunction A function that defines the pipeline steps
     * @return The result of execution
     */
    LegacyPipelineResult execute(
        const semcal::state::SemanticState& initialState,
        std::function<LegacyPipelineResult(semcal::state::SemanticState&)> stepFunction) const;

    /**
     * @brief Get the restrict operator.
     */
    semcal::operators::RestrictOp& getRestrict() const { return *restrict_; }

    /**
     * @brief Get the decompose operator.
     */
    semcal::operators::DecomposeOp& getDecompose() const { return *decompose_; }

    /**
     * @brief Get the infeasible operator.
     */
    semcal::operators::InfeasibleOp& getInfeasible() const { return *infeasible_; }

    /**
     * @brief Get the relax operator.
     */
    semcal::operators::RelaxOp& getRelax() const { return *relax_; }

    /**
     * @brief Get the refine operator.
     */
    semcal::operators::RefineOp& getRefine() const { return *refine_; }

    /**
     * @brief Get the shadow operator.
     */
    semcal::operators::ShadowOp& getShadow() const { return *shadow_; }

    /**
     * @brief Get the lift operator.
     */
    semcal::operators::LiftOp& getLift() const { return *lift_; }

    /**
     * @brief Get the semantics.
     */
    semcal::core::Semantics& getSemantics() const { return *semantics_; }

    /**
     * @brief Get the concretization.
     */
    semcal::domain::Concretization& getConcretization() const { return *concretization_; }
};

/**
 * @brief Factory for creating default pipelines.
 */
class LegacyPipelineFactory {
public:
    /**
     * @brief Create a default pipeline with default operator implementations.
     */
    static std::unique_ptr<LegacyOperatorPipeline> createDefault();
};

} // namespace strategies
} // namespace solver
} // namespace semcal

#endif // SEMCAL_SOLVER_STRATEGIES_PIPELINE_H
