# SemCal
## A Semantic Calculus for Constraint Reasoning

**SemCal** is a solver-agnostic *semantic calculus* for logical constraint reasoning.
It provides a **minimal axiomatization of semantic operators over model sets**
that underlie SMT-style satisfiability, optimization, counting, and sampling.

SemCal is **not a solver** and **does not prescribe algorithms**.
Instead, it formalizes *what it means* for reasoning components to be **sound at the semantic level**,
independently of how they are implemented (e.g., CAD, ICP, SAT, bit-blasting).

Crucially, **SemCal is designed to make it easy to construct new solvers**:
any reasoning system built by orchestrating SemCal operators
inherits semantic soundness by construction.

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
\llbracket F \rrbracket \;=\; \{\, M \in \mathcal{M} \mid M \models F \,\}.
$$

### 2.2 Logical Equivalence

Two constraints $F$ and $G$ are **logically equivalent** iff:

$$
F \equiv G \quad\Longleftrightarrow\quad \llbracket F \rrbracket = \llbracket G \rrbracket.
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
\llbracket F \rrbracket = \llbracket F' \rrbracket.
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
\gamma : \mathcal{A} \to \wp(\mathcal{M}).
$$

### 4.3 Galois Connection (Optional)

When available, an abstraction function

$$
\alpha : \wp(\mathcal{M}) \to \mathcal{A}
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

A **semantic state** is a pair:

$$
\sigma = (F, a), \quad a \in \mathcal{A}.
$$

Its concrete meaning is:

$$
\mathsf{Conc}(\sigma)
\;=\;
\llbracket F \rrbracket \cap \gamma(a).
$$

All semantic operators are required to be sound with respect to this meaning.

---

## 6. Semantic Space Operators (Core of SemCal)

Semantic operators transform semantic states or their components.
They are defined solely by **soundness axioms**.

(Restriction, Decomposition, Infeasibility, Relaxation, Restoration,
Projection, and Lifting — see `AXIOMS.md` for the formal axioms.)

---

## 7. Formal Scope and Coverage

SemCal provides **semantic coverage** for all *formalizable* reasoning tasks
in the SMT reasoning taxonomy, including:

- satisfiability and refutation
- abstraction and refinement
- optimization and relaxed feasibility
- model enumeration, counting, and integration
- symbolic sampling
- quantifier elimination and projection
- invariant and lemma synthesis

Tasks that depend on **search heuristics, optimization strategies,
or termination guarantees** are intentionally treated as
*applications outside the calculus*.

SemCal separates **semantic soundness** from **algorithmic strategy**.

---

## 8. Solver Construction via SemCal

A central goal of SemCal is to **enable the construction of new solvers**
by assembling semantic operators.

A solver in the SemCal sense is:

> an orchestration of semantic operators
> over semantic states $(F, a)$.

SemCal deliberately exposes **operator-level interfaces** rather than
monolithic solver APIs. This allows researchers to:

- prototype new solver architectures
- mix symbolic, numeric, and bit-level reasoning
- experiment with non-standard search or decomposition strategies
- build theory-specific or domain-specific solvers

### Soundness by Construction

Any solver pipeline that uses only operators satisfying
the SemCal axioms (R, D, I, A, C, P, L)
is **semantically sound by construction**.

SemCal therefore provides a **certified substrate**
for future SMT solver research.

---

## 9. Applications (Outside SemCal)

SemCal does not define applications, but supports them via orchestration:

- **SAT**: is $\llbracket F \rrbracket \neq \emptyset$? (pure Boolean satisfiability)
- **SMT**: satisfiability modulo theories (with theory constraints)
- **OPT**: extremal values over $\llbracket F \rrbracket$
- **COUNT / VOLUME**: measure of $\llbracket F \rrbracket$
- **SAMPLE**: draw elements from $\llbracket F \rrbracket$

---

## 10. Soundness Guarantee

Any reasoning pipeline that uses only operators satisfying the SemCal axioms
(R, D, I, A, C, P, L) is **semantically sound** with respect to the original
constraint $F$.

Completeness and termination are deliberately left unspecified.

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

- **CAD Backend**: Implements Project, Decompose, Infeasible, Lift operators
- **LP/Simplex Backend**: Implements Relax, Infeasible, Refine, Optimize operators
- **ICP Backend**: Implements Restrict, Decompose, Infeasible operators

Each backend specifies its approximation direction (over-approx, under-approx,
preserving, or refute-certified) to ensure correct composition.

---

## 12. Repository Structure

The SemCal repository is organized as follows:

```text
SemCal/
├── README.md
├── AXIOMS.md
├── COVERAGE.md
├── build.sh                 # Build script
│
├── include/                 # Public C++ interfaces (semantic contracts)
│   ├── semcal.h             # ⭐ Umbrella header (one-line include)
│   │
│   ├── core/
│   │   ├── model.h
│   │   ├── formula.h
│   │   └── semantics.h
│   │
│   ├── domain/
│   │   ├── abstract_domain.h
│   │   ├── concretization.h
│   │   ├── galois.h
│   │   └── top_element.h
│   │
│   ├── state/
│   │   └── semantic_state.h
│   │
│   ├── operators/
│   │   ├── restrict.h
│   │   ├── decompose.h
│   │   ├── infeasible.h
│   │   ├── relax.h
│   │   ├── refine.h
│   │   ├── project.h
│   │   └── lift.h
│   │
│   ├── orchestration/
│   │   ├── pipeline.h
│   │   └── strategy.h
│   │
│   ├── backends/              # Backend capability interfaces
│   │   ├── backend_capability.h
│   │   ├── cad_backend.h      # CAD backend interface
│   │   ├── lp_backend.h       # LP/Simplex backend interface
│   │   └── icp_backend.h      # ICP backend interface
│   │
│   └── util/
│       └── result.h           # OpResult<T, Witness> type
│
├── src/                     # Reference / default implementations
│   ├── core/
│   ├── domain/
│   ├── operators/
│   ├── orchestration/
│   ├── backends/             # Backend implementations
│   │   ├── cad_backend.cpp
│   │   ├── lp_backend.cpp
│   │   └── icp_backend.cpp
│   └── util/
│
├── examples/                # Example solvers built from SemCal
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

This will create:
- Library: `build/libsemcal.a`
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

SemCal intentionally does **not** provide:

- a solver
- a fixed search strategy
- a termination guarantee
- a preferred theory

Its role is to define the **semantic contract**
that enables *any* such system to be built correctly.
