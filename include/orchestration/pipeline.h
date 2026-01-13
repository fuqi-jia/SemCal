#ifndef SEMCAL_ORCHESTRATION_PIPELINE_H
#define SEMCAL_ORCHESTRATION_PIPELINE_H

#include "../state/semantic_state.h"
#include "../operators/restrict.h"
#include "../operators/decompose.h"
#include "../operators/infeasible.h"
#include "../operators/relax.h"
#include "../operators/refine.h"
#include "../operators/project.h"
#include "../operators/lift.h"
#include "../core/semantics.h"
#include "../domain/concretization.h"
#include <vector>
#include <memory>
#include <functional>

namespace semcal {
namespace orchestration {

/**
 * @brief Result of a pipeline execution.
 */
enum class PipelineResult {
    Success,
    Infeasible,
    Timeout,
    Error
};

/**
 * @brief Pipeline for orchestrating semantic operators.
 * 
 * A pipeline is a sequence of semantic operators applied to semantic states.
 * This enables solver construction by composing certified components.
 */
class Pipeline {
private:
    std::unique_ptr<operators::Restrict> restrict_;
    std::unique_ptr<operators::DecomposeOp> decompose_;
    std::unique_ptr<operators::InfeasibleOp> infeasible_;
    std::unique_ptr<operators::Relax> relax_;
    std::unique_ptr<operators::Refine> refine_;
    std::unique_ptr<operators::ProjectOp> project_;
    std::unique_ptr<operators::LiftOp> lift_;

    std::shared_ptr<core::Semantics> semantics_;
    std::shared_ptr<domain::Concretization> concretization_;

public:
    /**
     * @brief Construct a pipeline with operator instances.
     */
    Pipeline(
        std::unique_ptr<operators::Restrict> restrict,
        std::unique_ptr<operators::DecomposeOp> decompose,
        std::unique_ptr<operators::InfeasibleOp> infeasible,
        std::unique_ptr<operators::Relax> relax,
        std::unique_ptr<operators::Refine> refine,
        std::unique_ptr<operators::ProjectOp> project,
        std::unique_ptr<operators::LiftOp> lift,
        std::shared_ptr<core::Semantics> semantics,
        std::shared_ptr<domain::Concretization> concretization);

    /**
     * @brief Execute the pipeline on a semantic state.
     * 
     * @param initialState The initial semantic state
     * @param stepFunction A function that defines the pipeline steps
     * @return The result of execution
     */
    PipelineResult execute(
        const state::SemanticState& initialState,
        std::function<PipelineResult(state::SemanticState&)> stepFunction) const;

    /**
     * @brief Get the restrict operator.
     */
    operators::Restrict& getRestrict() const { return *restrict_; }

    /**
     * @brief Get the decompose operator.
     */
    operators::DecomposeOp& getDecompose() const { return *decompose_; }

    /**
     * @brief Get the infeasible operator.
     */
    operators::InfeasibleOp& getInfeasible() const { return *infeasible_; }

    /**
     * @brief Get the relax operator.
     */
    operators::Relax& getRelax() const { return *relax_; }

    /**
     * @brief Get the refine operator.
     */
    operators::Refine& getRefine() const { return *refine_; }

    /**
     * @brief Get the project operator.
     */
    operators::ProjectOp& getProject() const { return *project_; }

    /**
     * @brief Get the lift operator.
     */
    operators::LiftOp& getLift() const { return *lift_; }

    /**
     * @brief Get the semantics.
     */
    core::Semantics& getSemantics() const { return *semantics_; }

    /**
     * @brief Get the concretization.
     */
    domain::Concretization& getConcretization() const { return *concretization_; }
};

/**
 * @brief Factory for creating default pipelines.
 */
class PipelineFactory {
public:
    /**
     * @brief Create a default pipeline with default operator implementations.
     */
    static std::unique_ptr<Pipeline> createDefault();
};

} // namespace orchestration
} // namespace semcal

#endif // SEMCAL_ORCHESTRATION_PIPELINE_H
