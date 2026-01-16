# SemX / SemCal
## A Verified Semantic Framework for Solver Construction

**SemX** is a semantic framework for constructing **search-based constraint solvers**
as **first-class semantic programs**.

SemX separates:
- **semantic truth** from **algorithmic behavior**,
- **soundness** from **search heuristics**,
- **verified reasoning** from **high-performance execution**.

The goal of SemX is not to provide a solver,
but to provide a **correct-by-construction foundation** for building solvers.

---

## System Architecture

SemX is organized into four strictly layered subsystems:

```
SemX
├── SemCal     : Axiomatic Semantic Calculus
├── SemKernel  : Verified Semantic Kernel
├── SemSearch  : Generic Search and Execution Engine
└── SemSolver  : Concrete Solver Instances
```

### SemCal: Axiomatic Semantic Calculus

**SemCal** is a solver-agnostic *semantic calculus* for logical constraint reasoning.
It provides a **minimal axiomatization of semantic operators over model sets**
that underlie SMT-style satisfiability, optimization, counting, and sampling.

SemCal is **not a solver** and **does not prescribe algorithms**.
Instead, it formalizes *what it means* for reasoning components to be **sound at the semantic level**,
independently of how they are implemented (e.g., CAD, ICP, SAT, bit-blasting).

Crucially, **SemCal is designed to make it easy to construct new solvers**:
any reasoning system built by orchestrating SemCal operators
inherits semantic soundness by construction.

### SemKernel: Verified Semantic Kernel

**SemKernel** is the only trusted component in SemX.
Its sole responsibility is to **validate semantic claims made by solvers**.

SemKernel is not a solver.
It performs no search, no optimization, and no heuristics.

All UNSAT or refutation claims must be accompanied by **checkable evidence**.

### SemSearch: Generic Search Engine

**SemSearch** provides **search infrastructure only**:
- state scheduling (DFS, BFS, best-first)
- backtracking and branching
- execution of solver strategies
- resource management

SemSearch is **solver-agnostic**.
It executes solver programs, it does not define them.

### SemSolver: Concrete Solver Programs

A **SemSolver** is defined by:
- a solver strategy (program),
- a set of semantic operators,
- backend oracles,
- a search policy.

Each SemSolver is:
- a concrete, reproducible artifact
- executable under SemSearch
- validated by SemKernel
- comparable to other solvers

---

## 1. Design Philosophy

SemCal is built on the following principles:

- **Semantics-first**: reasoning is defined over *model sets*, not syntax.
- **Operator-based**: algorithms are instances of semantic operators.
- **Solver-agnostic**: no commitment to DPLL(T), CDCL, CAD, or any backend.
- **Minimal axioms**: only soundness requirements are specified.
- **Composable**: operators can be orchestrated to realize different tasks.
- **Solver-construction oriented**: new solvers are assembled from certified components.

The goal of SemCal is to serve as a **semantic foundation** for:

- SMT solving
- Optimization Modulo Theories (OMT)
- Model counting and volume computation
- Symbolic sampling and refutation
- Experimental and domain-specific solver design

---

## 2. Semantic Core

### 2.1 Models and Satisfaction

Fix a background theory (or a combination of theories) with a set of models:

$$
\mathcal{M}.
$$

For a constraint (formula) $F$, its **model set** is defined as:

$$
[[F]] = \{ M \in \mathcal{M} \mid M \models F \}.
$$

### 2.2 Logical Equivalence

Two constraints $F$ and $G$ are **logically equivalent** iff:

$$
F \equiv G \quad\Longleftrightarrow\quad [[F]] = [[G]].
$$

The semantic core defines *meaning only*.
No reasoning, search, or approximation is performed at this level.

---

## 3. Representation Layer

The representation layer consists of transformations on constraints that
preserve (or explicitly relate) semantic meaning.

### 3.1 Constraint-Preserving Transformations

A transformation

$$
T : F \mapsto F'
$$

is **semantics-preserving** if:

$$
[[F]] = [[F']].
$$

Typical instances include:

- normalization
- purification (theory separation)
- introduction of auxiliary variables
- syntactic rewriting

### 3.2 Semantic Encoding

An encoding

$$
\mathsf{Enc} : F \mapsto F'
$$

must explicitly specify its semantic contract, e.g.:

- equisatisfiability, or
- model preservation with a decoding function.

Encodings are **representation-level operators**, not reasoning operators.

---

## 4. Abstract Domains and Concretization

### 4.1 Abstract Domain

SemCal reasons about approximations of model sets via an abstract domain:

$$
(\mathcal{A}, \sqsubseteq).
$$

Examples include:

- boxes or regions
- symbolic cells or covers
- partial assignments
- clause sets

### 4.2 Concretization

A **concretization function** maps abstract elements to model sets:

$$
\gamma : \mathcal{A} \to \mathcal{P}(\mathcal{M}).
$$

### 4.3 Galois Connection (Optional)

When available, an abstraction function

$$
\alpha : \mathcal{P}(\mathcal{M}) \to \mathcal{A}
$$

forms a Galois connection with $\gamma$ if:

$$
\alpha(S) \sqsubseteq a
\;\Longleftrightarrow\;
S \subseteq \gamma(a).
$$

A Galois connection is **not required** for soundness,
but enables optimal abstraction and refinement.

---

## 5. Semantic States

A **semantic state** is a triple:

$$
\sigma = (F, a, \mu)
$$

where:
- $F$ is a constraint (formula)
- $a \in \mathcal{A}$ is an abstract element
- $\mu$ is a partial model (partial valuation)

Its concrete meaning is:

$$
\mathsf{Conc}(\sigma) = \{ M \in [[F]] \cap \gamma(a) \mid M \supseteq \mu \}
$$

Partial models represent **search decisions**.
All semantic operators are required to be sound with respect to this meaning.

---

## 6. Semantic Space Operators (Core of SemCal)

Semantic operators transform semantic states or their components.
They are defined solely by **soundness axioms**.

The core operators are:
- **R (Restrict)**: Sound contraction
- **D (Decompose)**: Sound space covering
- **I (Infeasible)**: Sound refutation
- **A (Relax)**: Sound over-approximation
- **C (Refine)**: Sound refinement
- **S (Shadow)**: Sound semantic shadowing
- **L (Lift)**: Sound lifting

See `AXIOMS.md` for the formal axioms and semantic guarantees.

---

## 7. Formal Scope and Coverage

SemCal provides **semantic coverage** for all *formalizable* reasoning tasks
in the SMT reasoning taxonomy, including:

- satisfiability and refutation
- abstraction and refinement
- optimization and relaxed feasibility
- model enumeration, counting, and integration
- symbolic sampling
- quantifier elimination and shadowing
- invariant and lemma synthesis

Tasks that depend on **search heuristics, optimization strategies,
or termination guarantees** are intentionally treated as
*applications outside the calculus*.

SemCal separates **semantic soundness** from **algorithmic strategy**.

---

## 8. Solver Construction via SemX

A central goal of SemX is to **enable the construction of new solvers**
as first-class semantic programs.

A **SemSolver** is defined by:
- a solver strategy (program),
- a set of semantic operators,
- backend oracles,
- a search policy.

The solver strategy is a program composed of:
- semantic operators (Restrict, Decompose, Infeasible, Relax, Refine, Shadow, Lift)
- search actions (extend partial model, branch, push/pop)
- control flow (conditionals, loops, nondeterministic choice)

SemX deliberately exposes **operator-level interfaces** rather than
monolithic solver APIs. This allows researchers to:

- prototype new solver architectures
- mix symbolic, numeric, and bit-level reasoning
- experiment with non-standard search or decomposition strategies
- build theory-specific or domain-specific solvers

### Soundness by Construction

Any solver built using SemX components is **semantically sound by construction**:
- SemCal operators satisfy soundness axioms
- SemKernel validates all semantic claims
- SemSearch provides solver-agnostic execution

SemX therefore provides a **certified substrate**
for future SMT solver research.

---

## 9. Applications (Outside SemCal)

SemCal does not define applications, but supports them via solver construction:

- **SAT**: is $[[F]] \neq \emptyset$? (pure Boolean satisfiability)
- **SMT**: satisfiability modulo theories (with theory constraints)
- **OPT**: extremal values over $[[F]]$
- **COUNT / VOLUME**: measure of $[[F]]$
- **SAMPLE**: draw elements from $[[F]]$

---

## 10. Soundness Guarantee

> **If SemKernel accepts a solver trace,
> then the solver result is semantically correct.**

This guarantee is independent of:
- search order
- heuristics
- termination
- performance

Completeness and termination are intentionally unspecified.

Any solver built using SemX components is **semantically sound** with respect to the original
constraint $F$, as validated by SemKernel.

---

## 11. Instantiation Policy

Concrete algorithms (e.g., CAD, ICP, bit-level reasoning) may be integrated
*only by demonstrating* that they satisfy the corresponding semantic axioms.

SemCal privileges **no algorithm** and **no solver architecture**.

### 11.1 Backend Adapter Architecture

SemCal provides a **backend adapter architecture** that allows concrete algorithms
to be integrated as operator implementations:

- **Backend Capability Interfaces**: Each backend (CAD, LP, ICP, SAT) declares
  which operators it supports and their approximation directions
- **Unified Result Type**: All operators return `OpResult<T, Witness>` with
  status (OK/UNSAT/UNKNOWN/PARTIAL/ERROR), value, and optional witness
- **Soundness Witnesses**: Backends can provide certificates/proofs explaining
  why their results satisfy the axioms

See `docs/instantiation.md` for a complete cookbook on instantiating backends.

### 11.2 Example Backends

- **CAD Backend**: Implements Shadow, Decompose, Infeasible, Lift operators
- **LP/Simplex Backend**: Implements Relax, Infeasible, Refine operators
- **ICP Backend**: Implements Restrict, Decompose, Infeasible operators

Each backend specifies its semantic guarantee (PRESERVING, OVER_APPROX, UNDER_APPROX,
COVERING, or REFUTE_CERTIFIED) to ensure correct composition.

---

## 12. Repository Structure

The SemX repository is organized according to the four-layer architecture:

```text
SemCal/
├── README.md
├── SemX.md          # SemX architecture documentation
├── AXIOMS.md        # SemCal axioms
├── COVERAGE.md
├── build.sh                 # Build script
├── commit.sh                # Auto commit script
│
├── include/                 # Public C++ interfaces (semantic contracts)
│   ├── semx.h               # ⭐ Umbrella header (one-line include)
│   │
│   ├── semcal/               # SemCal: Axiomatic Semantic Calculus
│   │   ├── core/              # Core semantic definitions
│   │   │   ├── model.h
│   │   │   ├── partial_model.h   # Partial models (μ)
│   │   │   ├── formula.h
│   │   │   └── semantics.h
│   │   ├── domain/            # Abstract domains
│   │   │   ├── abstract_domain.h
│   │   │   ├── concretization.h
│   │   │   ├── galois.h
│   │   │   └── top_element.h
│   │   ├── state/             # Semantic states (F, a, μ)
│   │   │   └── semantic_state.h
│   │   ├── operators/         # Semantic operators
│   │   │   ├── restrict.h
│   │   │   ├── decompose.h
│   │   │   ├── infeasible.h
│   │   │   ├── relax.h
│   │   │   ├── refine.h
│   │   │   ├── shadow.h
│   │   │   └── lift.h
│   │   ├── backends/          # Backend capability interfaces
│   │   │   ├── cad_backend.h
│   │   │   ├── lp_backend.h
│   │   │   └── icp_backend.h
│   │   └── util/              # Utilities
│   │       └── op_result.h     # OpResult<T, Witness> type
│   │
│   ├── semkernel/             # SemKernel: Verified Semantic Kernel
│   │   └── kernel.h
│   │
│   ├── semsearch/             # SemSearch: Generic Search Engine
│   │   └── search_engine.h
│   │
│   ├── semsolver/             # SemSolver: Concrete Solver Instances
│   │   └── solver.h
│   │
│
│
├── src/                      # Reference / default implementations
│   ├── semcal/               # SemCal implementations
│   │   ├── core/
│   │   ├── domain/
│   │   ├── state/
│   │   ├── operators/
│   │   ├── backends/
│   │   └── util/
│   ├── semkernel/            # SemKernel implementations
│   ├── semsearch/            # SemSearch implementations
│   └── semsolver/            # SemSolver implementations
│       └── strategies/       # Legacy strategy implementations (deprecated)
│
├── examples/                # Example solvers built from SemX
│   ├── sat/                 # SAT and SMT solver examples
│   │   ├── sat_solver.cpp  # Pure Boolean SAT solver
│   │   └── smt_solver.cpp  # SMT solver (with theories)
│   ├── omt/                # Optimization Modulo Theories
│   │   └── omt_solver.cpp
│   ├── counting/           # Model counting
│   │   └── counting_solver.cpp
│   └── pipelines/          # Backend orchestration examples
│       ├── cad_refute_first.cpp  # CAD refute-first pipeline
│       └── lp_guided_refine.cpp  # LP-guided refinement pipeline
│
├── external/                # Third-party libraries (submodules or vendored)
│   ├── SMTParser/
│   ├── libpoly/
│   ├── cadical/
│   └── README.md            # Dependency policy & versions
│
├── docs/
│   ├── design.md
│   ├── solver_patterns.md
│   └── instantiation.md     # Backend instantiation cookbook
│
├── CMakeLists.txt
└── LICENSE
```

### Building and Running Examples

To build the project:

```bash
./build.sh
```

This will automatically download SMTParser to `external/SMTParser/` if it doesn't exist.

### Committing Changes

To quickly commit changes with a predefined message:

```bash
./commit.sh
```

Edit `COMMIT_MESSAGE` in `commit.sh` to change the default commit message.

Options:
- `-m, --message MSG`: Override commit message
- `-p, --push`: Automatically push after commit
- `--no-push`: Don't push (default)

This will create:
- Library: `build/libsemx.a`
- Examples:
  - `build/examples/sat/sat_solver` - Pure Boolean SAT solver
  - `build/examples/sat/smt_solver` - SMT solver with theory constraints
  - `build/examples/omt/omt_solver` - Optimization Modulo Theories solver
  - `build/examples/counting/counting_solver` - Model counting solver
  - `build/examples/pipelines/cad_refute_first` - CAD refute-first pipeline
  - `build/examples/pipelines/lp_guided_refine` - LP-guided refinement pipeline

Run examples:
```bash
./build/examples/sat/sat_solver
./build/examples/sat/smt_solver
./build/examples/omt/omt_solver
./build/examples/counting/counting_solver
./build/examples/pipelines/cad_refute_first
./build/examples/pipelines/lp_guided_refine
```
---

## 13. Scope and Non-Goals

SemX intentionally does **not** provide:

- a complete solver implementation
- a fixed search strategy
- a termination guarantee
- a preferred theory

Its role is to provide a **correct-by-construction foundation**
that enables *any* such system to be built safely.

> **SemX does not build solvers.
> SemX defines what solvers are allowed to do,
> and how they can be built safely.**
