# SemCal Instantiation Cookbook

This document provides a practical guide for instantiating concrete algorithms
(CAD, Simplex, ICP, SAT, etc.) as SemCal operator implementations.

The key principle is:

> **Do not implement algorithms as monolithic solvers. Instead, implement them
> as SemCal operator instances (adapters) that satisfy the semantic axioms.**

---

## Overview

Each backend algorithm must:

1. **Implement operator interfaces**: Declare which SemCal operators it supports
2. **Specify approximation directions**: Over-approx, under-approx, preserving, or refute-certified
3. **Provide witnesses/certificates**: Optional proof objects explaining soundness
4. **Handle failure modes**: Return UNKNOWN/PARTIAL when appropriate
5. **Document dependencies**: Required libraries, rounding modes, precision

---

## Backend Template

Every backend should follow this structure:

```cpp
class MyBackend : public BackendCapability {
public:
    // 1. Declare supported operators
    bool supportsOperator(const std::string& opName) const override;
    
    // 2. Declare approximation directions
    ApproxDirection getApproxDirection(const std::string& opName) const override;
    
    // 3. Implement operator methods
    // Each returns OpResult<T, Witness> with status, value, and witness
};
```

---

## CAD Backend

### Supported Operators

- **Project** (Axiom P): Variable elimination via projection polynomials
- **Decompose** (Axiom D): Cell decomposition (sign-invariant, cylindrical)
- **Infeasible** (Axiom I): Cell refutation via sample point evaluation
- **Lift** (Axiom L): Lifting from lower to higher dimensions

### Approximation Directions

| Operator | Direction | Rationale |
|----------|-----------|----------|
| Project | OVER_APPROX | Projection may introduce spurious cells: ∃_π(γ(a)) ⊆ γ_k(Proj_π(a)) |
| Decompose | PRESERVING | Must cover all cells: γ(a) ⊆ ∪_i γ(a_i) |
| Infeasible | REFUTE_CERTIFIED | UNSAT claims must be globally correct |
| Lift | UNDER_APPROX | Only valid candidates: γ(Lift_π(b)) ⊆ π^{-1}(γ_k(b)) |

### Witness Types

- **CadProjectWitness**: Eliminated variables, projection polynomials
- **CadDecompWitness**: Number of cells, decomposition type
- **CadUnsatWitness**: Refutation method, conflicting constraints

### Implementation Notes

- **Dependencies**: libpoly, rational arithmetic
- **Failure modes**: 
  - UNKNOWN if projection is too expensive
  - PARTIAL if decomposition is incomplete
- **Soundness**: Requires exact arithmetic for projection polynomials

### Example Usage

```cpp
auto cadBackend = std::make_unique<CadBackendImpl>();

// Project out variables
auto projResult = cadBackend->project(formula, abstractElement, {"x", "y"});
if (projResult.status == OpStatus::OK) {
    auto [projFormula, projElement] = std::move(projResult.value);
    // Use projected result
}

// Decompose into cells
auto decompResult = cadBackend->decompose(formula, abstractElement);
if (decompResult.status == OpStatus::OK) {
    for (auto& cellState : decompResult.value) {
        // Process each cell
    }
}

// Refute a cell
auto refuteResult = cadBackend->refute(formula, abstractElement);
if (refuteResult.status == OpStatus::UNSAT) {
    // Cell is empty (certified by witness)
}
```

---

## LP/Simplex Backend

### Supported Operators

- **Relax** (Axiom A): Linear relaxation (over-approx or under-approx)
- **Infeasible** (Axiom I): LP infeasibility with Farkas certificate
- **Refine** (Axiom C): Cut generation (Gomory, Chvatal-Gomory, Farkas-based)
- **Optimize**: LP optimization oracle (for OMT)

### Approximation Directions

| Operator | Direction | Rationale |
|----------|-----------|----------|
| Relax | OVER_APPROX (default) | Linear relaxation admits extra models: [[F]] ⊆ [[Relax(F)]] |
| Relax | UNDER_APPROX (optional) | Tightening/strengthening: [[Relax(F)]] ⊆ [[F]] |
| Infeasible | REFUTE_CERTIFIED | Farkas certificate proves infeasibility |
| Refine | PRESERVING | Cuts preserve real solutions: [[F]] ⊆ [[F ∧ R]] |

### Witness Types

- **LpRelaxWitness**: Relaxed constraints, relaxation type
- **LpUnsatWitness**: Farkas coefficients (certificate)
- **LpCutWitness**: Cut type, cut constraint

### Implementation Notes

- **Dependencies**: GLPK, CPLEX, Gurobi, or custom simplex
- **Failure modes**:
  - UNKNOWN if relaxation is not applicable
  - UNKNOWN if optimization is unbounded
- **Soundness**: 
  - Over-approx relax: LP feasible ⇒ original may be feasible
  - Under-approx relax: LP infeasible ⇒ original infeasible
  - Infeasible: Farkas certificate guarantees correctness

### Example Usage

```cpp
auto lpBackend = std::make_unique<LpBackendImpl>();

// Relax to linear (over-approx)
auto relaxResult = lpBackend->relax(formula, ApproxDirection::OVER_APPROX);
if (relaxResult.status == OpStatus::OK) {
    auto relaxedFormula = std::move(relaxResult.value);
    // Use relaxed formula for optimization
}

// Check infeasibility
auto infeasResult = lpBackend->infeasible(formula, abstractElement);
if (infeasResult.status == OpStatus::UNSAT) {
    // Proven infeasible (Farkas certificate in witness)
    auto farkasCert = infeasResult.witness;
}

// Generate cuts
auto cutResult = lpBackend->refine(formula, spuriousModel);
if (cutResult.status == OpStatus::OK) {
    auto refinedFormula = std::move(cutResult.value);
    // Add cut to formula
}

// Optimize
auto optResult = lpBackend->optimize(formula, objectiveCoeffs, true);
if (optResult.status == OpStatus::OK) {
    auto opt = optResult.value;
    // Use optimal value and solution
}
```

---

## ICP Backend

### Supported Operators

- **Restrict** (Axiom R): Interval contraction (sound narrowing)
- **Decompose** (Axiom D): Box splitting
- **Infeasible** (Axiom I): Empty box detection

### Approximation Directions

| Operator | Direction | Rationale |
|----------|-----------|----------|
| Restrict | PRESERVING | Sound contraction: [[F]] ∩ γ(a') = [[F]] ∩ γ(a), γ(a') ⊆ γ(a) |
| Decompose | PRESERVING | Box splitting covers all space: γ(a) ⊆ ∪_i γ(a_i) |
| Infeasible | REFUTE_CERTIFIED | Empty box detection must be correct |

### Witness Types

- **IcpContractWitness**: Contracted variables, contraction method, rounding mode
- **IcpDecompWitness**: Number of boxes, split variable, split point

### Implementation Notes

- **Dependencies**: Interval arithmetic library, directed rounding support
- **Failure modes**:
  - UNKNOWN if contraction fails (e.g., due to rounding errors)
  - PARTIAL if contraction is incomplete
- **Soundness**: 
  - **Critical**: Must use directed rounding or high precision
  - Unsound contraction can incorrectly remove feasible points
  - If rounding is not sound, must return UNKNOWN

### Example Usage

```cpp
auto icpBackend = std::make_unique<IcpBackendImpl>();

// Contract intervals
auto contractResult = icpBackend->contract(formula, abstractElement);
if (contractResult.status == OpStatus::OK) {
    auto contractedElement = std::move(contractResult.value);
    // Use contracted element (sound narrowing)
    // Check witness for rounding mode
}

// Decompose by splitting
auto decompResult = icpBackend->decompose(formula, abstractElement);
if (decompResult.status == OpStatus::OK) {
    for (auto& boxState : decompResult.value) {
        // Process each box
    }
}

// Check if box is empty
auto infeasResult = icpBackend->infeasible(formula, abstractElement);
if (infeasResult.status == OpStatus::UNSAT) {
    // Box is empty
}
```

---

## SAT Backend

### Supported Operators

- **Decompose** (Axiom D): Case splitting (branching)
- **Infeasible** (Axiom I): Conflict detection (UNSAT proof)
- **Refine** (Axiom C): Clause learning (lemma generation)

### Approximation Directions

| Operator | Direction | Rationale |
|----------|-----------|----------|
| Decompose | PRESERVING | Case splitting covers all assignments |
| Infeasible | REFUTE_CERTIFIED | UNSAT proof must be correct |
| Refine | PRESERVING | Learned clauses preserve satisfiability |

### Witness Types

- **SatDecompWitness**: Split variable, assignment
- **SatUnsatWitness**: UNSAT proof (resolution proof, DRAT certificate)
- **SatRefineWitness**: Learned clause

### Implementation Notes

- **Dependencies**: CaDiCaL, MiniSat, or custom SAT solver
- **Failure modes**:
  - UNKNOWN if timeout
  - PARTIAL if incomplete search
- **Soundness**: 
  - UNSAT requires proof certificate
  - Learned clauses must be implied

---

## Composition Guidelines

### Combining Operators

When composing operators, ensure approximation directions are compatible:

1. **OVER_APPROX → UNDER_APPROX**: May not preserve soundness
2. **OVER_APPROX → OVER_APPROX**: Sound (monotonic)
3. **UNDER_APPROX → UNDER_APPROX**: Sound (monotonic)
4. **PRESERVING → any**: Sound

### Example: LP + CAD Pipeline

```cpp
// 1. Relax to linear (OVER_APPROX)
auto relaxResult = lpBackend->relax(formula, OVER_APPROX);

// 2. Optimize LP (gives bounds)
auto optResult = lpBackend->optimize(relaxedFormula, objCoeffs, true);

// 3. If LP infeasible, original is infeasible (only if under-approx!)
// But with over-approx, we can't conclude UNSAT

// 4. For nonlinear parts, use CAD (PRESERVING)
auto decompResult = cadBackend->decompose(nonlinearFormula, abstractElement);

// 5. Refute cells (REFUTE_CERTIFIED)
for (auto& cell : cells) {
    if (cadBackend->refute(cell) == UNSAT) {
        // Prune (sound)
    }
}
```

### Soundness by Construction

The pipeline is sound if:

- Each operator satisfies its axiom
- Approximation directions are compatible
- UNSAT claims are refute-certified
- Witnesses can be verified (optional)

---

## Testing and Verification

### Runtime Checks

Implement runtime checks to verify axioms:

```cpp
// Example: Check Restrict axiom
void verifyRestrict(const AbstractElement& a, const AbstractElement& restricted) {
    // Check: γ(restricted) ⊇ γ(a) ∩ [[F]]
    // In practice: check that all models in γ(a) ∩ [[F]] are in γ(restricted)
}
```

### Witness Verification

Verify witnesses when available:

```cpp
// Example: Verify Farkas certificate
bool verifyFarkas(const LpUnsatWitness& witness, const Formula& formula) {
    // Check that Farkas coefficients prove infeasibility
    // This is a mathematical proof object
}
```

---

## Summary Table

| Backend | Operators | Key Direction | Dependencies | Failure Modes |
|---------|-----------|---------------|--------------|---------------|
| CAD | P, D, I, L | Project: OVER_APPROX | libpoly, rational | UNKNOWN, PARTIAL |
| LP | A, I, C, OPT | Relax: OVER_APPROX | GLPK/CPLEX | UNKNOWN |
| ICP | R, D, I | Restrict: PRESERVING | Interval, rounding | UNKNOWN, PARTIAL |
| SAT | D, I, C | All: PRESERVING/REFUTE | CaDiCaL/MiniSat | UNKNOWN, PARTIAL |

---

## Next Steps

1. **Choose a backend**: Select CAD, LP, ICP, or SAT based on your problem
2. **Implement operator methods**: Follow the interface and return `OpResult`
3. **Specify directions**: Declare approximation directions for each operator
4. **Add witnesses**: Provide certificates when possible
5. **Test soundness**: Verify axioms hold for your implementation
6. **Compose pipelines**: Combine multiple backends for hybrid solving

For complete examples, see:
- `examples/pipelines/cad_refute_first.cpp`
- `examples/pipelines/lp_guided_refine.cpp`
