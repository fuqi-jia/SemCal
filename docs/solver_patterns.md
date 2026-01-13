# Solver Construction Patterns

This document describes common patterns for constructing solvers using SemCal operators.

## SAT Solving Pattern

```cpp
// 1. Create initial state with formula and top element
auto formula = std::make_unique<ConcreteFormula>("...");
auto topElement = std::make_unique<TopElement>();
auto state = std::make_unique<SemanticState>(
    std::move(formula),
    std::move(topElement)
);

// 2. Use decomposition to split search space
auto decomposed = decompose.applyToState(*state);

// 3. Check each state for infeasibility
for (auto& s : decomposed) {
    if (!infeasible.check(*s, semantics, concretization)) {
        // Found a satisfying model
        return true;
    }
}

// 4. If all states are infeasible, formula is UNSAT
return false;
```

## Optimization Pattern (OMT)

```cpp
// 1. Start with relaxed formula
auto relaxed = relax.apply(originalFormula);

// 2. Find a model in the relaxed space
auto model = findModel(relaxed);

// 3. If model satisfies original, we're done
if (semantics.satisfies(*model, originalFormula)) {
    return model;
}

// 4. Otherwise, refine and repeat
auto refinement = refine.generate(originalFormula, relaxed, *model, semantics);
auto refined = refine.apply(relaxed, refinement);
// Repeat from step 2
```

## Model Counting Pattern

```cpp
// 1. Decompose the search space
auto states = decompose.applyToState(initialState);

// 2. For each state, count models
size_t totalCount = 0;
for (auto& state : states) {
    if (!infeasible.check(*state, semantics, concretization)) {
        auto models = state->concretize(semantics, concretization);
        totalCount += models.size();
    }
}

return totalCount;
```

## Abstraction-Refinement Pattern

```cpp
// 1. Abstract the formula
auto abstracted = abstraction.abstract(modelSet);

// 2. Check satisfiability in abstract domain
if (infeasible.check(formula, *abstracted, semantics, concretization)) {
    return UNSAT;
}

// 3. If satisfiable, check if model is spurious
auto model = findModel(abstracted);
if (!semantics.satisfies(*model, originalFormula)) {
    // 4. Refine and repeat
    auto refinement = refine.generate(originalFormula, abstractedFormula, *model, semantics);
    // Repeat from step 2
}
```

## Best Practices

1. **Always check infeasibility** before exploring a state
2. **Use appropriate strategies** for different tasks:
   - DFS for SAT
   - BFS for counting
   - Best-first for optimization
3. **Compose operators** to build complex solvers
4. **Verify soundness** of custom operator implementations
