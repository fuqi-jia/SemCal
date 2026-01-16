# SemCal Axioms
## Axiomatization of Semantic Space Operators

This document presents the **axiomatic core** of **SemCal**.
Each axiom specifies a *minimal semantic soundness requirement*
for a class of semantic space operators.

The axioms are **theory-agnostic** and **algorithm-independent**.
They define *what is semantically allowed*,
not *how an algorithm is implemented*.

---

## Preliminaries

Let:

- $\mathcal{M}$ be the set of all models of a background theory.
- $F$ be a constraint (formula).
- $[[F]] \subseteq \mathcal{M}$ denote its model set.
- $(\mathcal{A}, \sqsubseteq)$ be an abstract domain.
- $\gamma : \mathcal{A} \to \mathcal{P}(\mathcal{M})$ be a concretization function.

A **semantic state** is a pair $(F, a)$ with concrete meaning:
\[
\mathsf{Conc}(F,a) = [[F]] \cap \gamma(a).
\]

All axioms below are stated with respect to $\mathsf{Conc}$,
unless explicitly noted otherwise.

---

## Axiom (R): Restriction Soundness

### Statement (Exact Restriction)

A restriction operator
\[
\mathsf{Restrict} : (F,a) \mapsto (F,a')
\]
is **exactly sound** if:
\[
[[F]] \cap \gamma(a) = [[F]] \cap \gamma(a').
\]

Equivalently:
\[
\gamma(a') \subseteq \gamma(a)
\quad\text{and}\quad
[[F]] \cap \gamma(a) \subseteq \gamma(a').
\]

### Statement (Safe Restriction)

A restriction operator is **safely sound** if:
\[
[[F]] \cap \gamma(a)
\;\subseteq\;
[[F]] \cap \gamma(a').
\]

### Intuition

Restriction may remove spurious candidates,
but must never remove genuine satisfying models.

Exact restriction preserves the concrete meaning,
while safe restriction preserves satisfiability only.

### Typical Instances

- Sound interval contraction (ICP)
- Constraint propagation
- Word-level range propagation

---

## Axiom (D): Decomposition Covering Soundness

### Statement

For a decomposition operator
\[
\mathsf{Decomp} : a \mapsto \{a_1,\dots,a_n\},
\]
the following **covering condition** must hold:
\[
\gamma(a)
\;\subseteq\;
\bigcup_{i} \gamma(a_i).
\]

### Optional Strengthenings

- **Subspace soundness**:
  \[
  \forall i,\ \gamma(a_i) \subseteq \gamma(a)
  \]
- **Exact decomposition**:
  \[
  \bigcup_i \gamma(a_i) = \gamma(a)
  \]

### Intuition

Decomposition splits the search space
but must **cover all possibilities**.
Overlap and redundancy are permitted.

### Typical Instances

- Case splitting
- Branching on variables
- Cell decomposition (CAD, CAC)

---

## Axiom (I): Local Infeasibility Soundness

### Statement

If
\[
\mathsf{Infeasible}(F,a)=\textsf{true},
\]
then:
\[
[[F]] \cap \gamma(a) = \emptyset.
\]

### Intuition

Declaring a region infeasible must be *globally correct*.
No false UNSAT is permitted.

### Typical Instances

- Cell emptiness checks
- Box refutation
- Theory-level conflict detection
- LP-based refutation with certificates

---

## Axiom (A): Relaxation Soundness

### Statement

For a relaxation operator
\[
\mathsf{Relax} : F \mapsto F^\alpha,
\]
the following must hold:
\[
[[F]] \subseteq [[F^\alpha]].
\]

### Intuition

Relaxation may admit spurious models,
but must preserve all real ones.

### Typical Instances

- Predicate abstraction
- Bit-level abstraction
- Linearization of nonlinear constraints

---

## Axiom (C): Restoration (Refinement) Soundness

### Statement

Given:

- a relaxation $F^\alpha$,
- a spurious model $M \models F^\alpha$ with $M \not\models F$,

a refinement constraint $R$ is **sound** if:
\[
F \Rightarrow R
\quad\text{and}\quad
M \not\models R.
\]

Equivalently:
\[
[[F]] \subseteq [[F^\alpha \land R]]
\quad\text{and}\quad
M \not\models R.
\]

### Intuition

Refinement removes spurious behavior
without excluding genuine solutions.

### Typical Instances

- Lemma generation
- Counterexample-guided refinement
- Cutting-plane strengthening

---

## Axiom (S): Shadowing Soundness

### Statement

Let $\pi$ be a variable projection.
A **shadowing operator**
\[
\mathsf{Shadow}_\pi : (F,a) \mapsto b
\]
is **sound** if:
\[
\exists_\pi\!\big([[F]] \cap \gamma(a)\big)
\;\subseteq\;
\gamma(b).
\]

### Intuition

Shadowing safely collapses a constrained semantic space
onto a lower-dimensional space,
preserving all feasible *shadows* of satisfying models.

Shadowing is a **semantic operation**.
It is distinct from structural or syntactic projection
(e.g., CAD polynomial projection).

### Typical Instances

- Variable forgetting
- Over-approximate existential abstraction
- Dimension reduction for abstraction

---

## Axiom (L): Lifting Soundness

### Statement

Let $\pi$ be a variable projection.
A lifting operator
\[
\mathsf{Lift}_\pi : b \mapsto a
\]
is **sound** if:
\[
\gamma(a)
\;\subseteq\;
\pi^{-1}\!\left(\gamma(b)\right).
\]

### Intuition

Lifting reconstructs higher-dimensional candidates
without introducing values inconsistent with the shadowed space.

This axiom expresses **safety only**.
Completeness or progress guarantees are orthogonal.

### Typical Instances

- CAD lifting
- Model reconstruction
- Back-substitution

---

## Summary of Axioms

| Axiom | Semantic Role |
|------:|---------------|
| R | Sound contraction (exact or safe) |
| D | Sound space covering |
| I | Sound refutation |
| A | Sound over-approximation |
| C | Sound refinement |
| S | Sound semantic shadowing |
| L | Sound lifting |

These axioms constitute the **entire semantic contract** of SemCal.
Any algorithm satisfying the relevant axioms
is a valid SemCal instantiation.

---

## Semantic Guarantees

Operators must declare the **semantic guarantee**
they provide with respect to $\mathsf{Conc}$.

### Guarantee Types

- **PRESERVING**
  \[
  \mathsf{Conc}(\sigma') = \mathsf{Conc}(\sigma)
  \]

- **OVER\_APPROX**
  \[
  \mathsf{Conc}(\sigma) \subseteq \mathsf{Conc}(\sigma')
  \]

- **UNDER\_APPROX**
  \[
  \mathsf{Conc}(\sigma') \subseteq \mathsf{Conc}(\sigma)
  \]

- **COVERING**
  \[
  \mathsf{Conc}(\sigma)
  \subseteq
  \bigcup_i \mathsf{Conc}(\sigma_i)
  \]

- **REFUTE\_CERTIFIED**
  \[
  \mathsf{Infeasible}(\sigma)=\textsf{true}
  \;\Rightarrow\;
  \mathsf{Conc}(\sigma)=\emptyset
  \]

---

## Default Guarantees by Axiom

| Axiom | Default Guarantee | Notes |
|------:|------------------:|-------|
| R (Restrict) | PRESERVING / SAFE | Exact or safe contraction |
| D (Decompose) | COVERING | Space must be covered |
| I (Infeasible) | REFUTE\_CERTIFIED | UNSAT claims must be globally correct |
| A (Relax) | OVER\_APPROX | By definition |
| C (Refine) | PRESERVING | Real models preserved |
| S (Shadow) | OVER\_APPROX | Feasible shadows preserved |
| L (Lift) | UNDER\_APPROX | Candidate reconstruction |

---

## Backend-Specific Notes

- **CAD**
  - Decompose: COVERING
  - Infeasible: REFUTE\_CERTIFIED
  - Lift: UNDER\_APPROX
  - *Structural CAD projection is not a SemCal operator and does not instantiate Axiom (S).*

- **LP / Simplex**
  - Relax: OVER\_APPROX
  - Infeasible: REFUTE\_CERTIFIED (Farkas certificate)
  - Refine: PRESERVING (cuts preserve real solutions)

- **ICP**
  - Restrict: SAFE / PRESERVING
  - Decompose: COVERING
  - Infeasible: REFUTE\_CERTIFIED

**Critical.**
When composing operators,
their semantic guarantees must be compatible.
In particular, composing OVER\_APPROX and UNDER\_APPROX operators
requires explicit justification.

---
