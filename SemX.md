# SemX
## A Verified Semantic Framework for Solver Construction

**SemX** is a semantic framework for constructing
**search-based constraint solvers**
as **first-class semantic programs**.

SemX separates:

- **semantic truth** from **algorithmic behavior**,
- **soundness** from **search heuristics**,
- **verified reasoning** from **high-performance execution**.

The goal of SemX is not to provide a solver,
but to provide a **correct-by-construction foundation**
for building solvers.

---

## 1. System Overview

SemX is organized into four strictly layered subsystems:

```

SemX
├── SemCal     : Axiomatic Semantic Calculus
├── SemKernel  : Verified Semantic Kernel
├── SemSearch  : Generic Search and Execution Engine
└── SemSolver  : Concrete Solver Instances

````

Each subsystem has a **precise responsibility**
and a **non-negotiable trust boundary**.

---

## 2. Design Principles

### 2.1 Solver as a Semantic Program

In SemX:

> **A solver is a program that explores semantic states
> using sound semantic operators under an explicit search discipline.**

- Semantic meaning is defined by **SemCal**
- Semantic validity is enforced by **SemKernel**
- Search mechanics are provided by **SemSearch**
- Concrete solvers are defined in **SemSolver**

Solvers differ only in *strategy*,
never in *semantic rules*.

---

### 2.2 Hard Separation of Concerns

SemX enforces the following separation:

| Concern | Responsible Subsystem |
|------|----------------------|
| Semantic axioms | SemCal |
| Semantic soundness checking | SemKernel |
| Search control & backtracking | SemSearch |
| Solver strategy & composition | SemSolver |
| Performance & heuristics | Implementations |

Violating this separation is considered a design error.

---

## 3. Semantic Foundations

### 3.1 Models and Constraints

Let:

- $\mathcal{M}$ be the set of models of a background theory
- $F$ be a constraint (formula)
- $[[F]] \subseteq \mathcal{M}$ denote the model set of $F$

---

### 3.2 Abstract Domains

Let:

- $(\mathcal{A}, \sqsubseteq)$ be an abstract domain
- $\gamma : \mathcal{A} \to \mathcal{P}(\mathcal{M})$ be a concretization

$\mathcal{A}$ represents an over-approximation
of possible models.

---

### 3.3 Partial Models

A **partial model** $\mu$ is a partial valuation:

\[
\mu : \text{Var} \rightharpoonup \text{Val}
\]

A total model $M$ **extends** $\mu$ iff:

\[
M \supseteq \mu
\]

Partial models represent **search decisions**.

---

### 3.4 Semantic State

A **semantic state** is a triple:

\[
\sigma = (F,\; a,\; \mu)
\]

where:

- $F$ is a constraint
- $a \in \mathcal{A}$ is an abstract region
- $\mu$ is a partial model consistent with $a$

---

### 3.5 Concrete Meaning

The concrete interpretation of a semantic state is:

\[
\mathsf{Conc}(F,a,\mu)
=
\{\, M \in [[F]] \cap \gamma(a) \mid M \supseteq \mu \,\}
\]

---

### 3.6 Search Interpretation

- **Search progress**: extending $\mu$
- **Branching**: alternative extensions of $\mu$ or $a$
- **Pruning**: proving $\mathsf{Conc}(\sigma) = \emptyset$
- **Success**: extending $\mu$ to a total model

---

## 4. SemCal: Axiomatic Semantic Calculus

SemCal defines the **semantic instruction set**
available to all solvers.

It specifies **what is semantically allowed**,
not **how algorithms are implemented**.

---

### 4.1 Core Axioms

SemCal consists of the following axioms:

| Axiom | Role |
|-----:|------|
| R | Restriction |
| D | Decomposition |
| I | Infeasibility |
| A | Relaxation |
| C | Refinement |
| S | Shadowing |
| L | Lifting |

Each axiom constrains semantic operators
with respect to $\mathsf{Conc}$.

---

### 4.2 Operator Contract

Each semantic operator must declare:

- its axiom class,
- its semantic guarantee:
  - PRESERVING
  - OVER_APPROX
  - UNDER_APPROX
  - COVERING
  - REFUTE_CERTIFIED
- whether it requires evidence.

Operators may be incomplete or heuristic,
but **never unsound**.

---

## 5. SemKernel: Verified Semantic Kernel

### 5.1 Role

**SemKernel** is the only trusted component in SemX.

Its sole responsibility is:

> **to validate semantic claims made by solvers.**

SemKernel is not a solver.
It performs no search, no optimization, and no heuristics.

---

### 5.2 Kernel Judgments

SemKernel checks judgments of the form:

- **Refutation**:
  \[
  \mathsf{Conc}(\sigma) = \emptyset
  \]
- **Containment**:
  \[
  \mathsf{Conc}(\sigma) \subseteq \mathsf{Conc}(\sigma')
  \]
- **Covering**:
  \[
  \mathsf{Conc}(\sigma) \subseteq \bigcup_i \mathsf{Conc}(\sigma_i)
  \]
- **Model validity**:
  \[
  M \in \mathsf{Conc}(\sigma)
  \]

---

### 5.3 Kernel Interface (Abstract)

Conceptually, SemKernel provides:

```text
check_step :
  State → Step → Evidence → option State

run_trace :
  State → list (Step × Evidence) → option State
````

If a trace is accepted,
the resulting semantic claim is correct.

---

### 5.4 Trust Boundary

SemKernel does **not** trust:

* solver strategies,
* backend solvers,
* numeric approximations,
* floating-point arithmetic,
* implementation code.

All UNSAT or refutation claims
must be accompanied by **checkable evidence**.

---

## 6. SemSearch: Generic Search Engine

### 6.1 Role

SemSearch provides **search infrastructure only**:

* state scheduling (DFS, BFS, best-first)
* backtracking and branching
* execution of solver strategies
* resource management

SemSearch is **solver-agnostic**.

---

### 6.2 What SemSearch Must Not Do

SemSearch must **not**:

* decide which operator is applied
* claim SAT or UNSAT
* interpret semantic meaning

SemSearch executes solver programs,
it does not define them.

---

## 7. SemSolver: Concrete Solver Programs

### 7.1 Definition

A **SemSolver** is defined by:

* a solver strategy (program),
* a set of semantic operators,
* backend oracles,
* a search policy.

---

### 7.2 Solver Strategy Language

A solver strategy is a program composed of:

* semantic operators:

  * Restrict, Decompose, Infeasible, Relax, Refine, Shadow, Lift
* search actions:

  * extend partial model
  * branch
  * push / pop
* control flow:

  * conditionals
  * loops
  * nondeterministic choice

---

### 7.3 Solver as Artifact

Each SemSolver is:

* a concrete, reproducible artifact
* executable under SemSearch
* validated by SemKernel
* comparable to other solvers

Examples:

* SAT solvers
* SMT solvers
* ICP-based numeric solvers
* CAD refutation solvers
* Counting and sampling solvers

---

## 8. Soundness Guarantee

> **If SemKernel accepts a solver trace,
> then the solver result is semantically correct.**

This guarantee is independent of:

* search order
* heuristics
* termination
* performance

Completeness and termination are intentionally unspecified.

---

## 9. Summary

SemX provides:

* **SemCal**: a minimal axiomatic semantic calculus
* **SemKernel**: a verified semantic judge
* **SemSearch**: a generic search execution engine
* **SemSolver**: a principled method for solver construction

> **SemX does not build solvers.
> SemX defines what solvers are allowed to do,
> and how they can be built safely.**

```