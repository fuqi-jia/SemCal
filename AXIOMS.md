# SemCal Axioms
## Axiomatization of Semantic Space Operators

This document presents the **axiomatic core** of SemCal.
Each axiom specifies the *minimal semantic soundness requirement*
for a class of semantic space operators.

The axioms are **theory-agnostic** and **algorithm-independent**.

---

## Preliminaries

Let:

- $\mathcal{M}$ be the set of all models of a background theory.
- $F$ be a constraint (formula).
- $[\![F]\!] \subseteq \mathcal{M}$ denote its model set.
- $(\mathcal{A}, \sqsubseteq)$ be an abstract domain.
- $\gamma : \mathcal{A} \to \mathcal{P}(\mathcal{M})$ be a concretization function.

A semantic state is a pair $(F, a)$ with concrete meaning:
\[
\mathsf{Conc}(F,a) = [\![F]\!] \cap \gamma(a).
\]

All axioms below are stated with respect to $\mathsf{Conc}$.

---

## Axiom (R): Restriction Soundness

### Statement

A restriction operator
\[
\mathsf{Restrict} : (F,a) \mapsto (F,a')
\]
is **sound** if:
\[
[\![F]\!] \cap \gamma(a) = [\![F]\!] \cap \gamma(a').
\]

Equivalently:
\[
\gamma(a') \subseteq \gamma(a)
\quad\text{and}\quad
\gamma(a) \cap [\![F]\!] \subseteq \gamma(a').
\]

### Intuition

Restriction may *remove spurious candidates*,
but must never remove a genuine satisfying model.

### Typical Instances

- Interval narrowing / bound tightening (sound ICP)
- Constraint propagation
- Word-level range propagation

---

## Axiom (D): Decomposition Soundness

### Statement

For a decomposition operator
\[
\mathsf{Decomp} : a \mapsto \{a_1,\dots,a_n\},
\]
the following must hold:
\[
\gamma(a)
\;\subseteq\;
\bigcup_{i} \gamma(a_i).
\]

### Intuition

Decomposition splits the search space,
but must **cover all possibilities**.
Overlap and redundancy are permitted.

### Typical Instances

- Case splitting
- Branching on variables
- Cell decomposition (e.g., CAD, CAC)

---

## Axiom (I): Local Infeasibility Soundness

### Statement

If
\[
\mathsf{Infeasible}(F,a)=\textsf{true},
\]
then:
\[
[\![F]\!] \cap \gamma(a) = \emptyset.
\]

### Intuition

Declaring a region infeasible must be *globally correct*.

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
[\![F]\!] \subseteq [\![F^\alpha]\!].
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
[\![F]\!] \subseteq [\![F^\alpha \land R]\!]
\quad\text{and}\quad
M \not\models R.
\]

### Intuition

Refinement removes spurious behavior
without excluding genuine solutions.

### Typical Instances

- Lemma generation
- Counterexample-guided refinement
- Cutting-plane style strengthening

---

## Axiom (P): Projection Soundness

### Statement

Let $\pi$ be a variable projection.
A projection operator
\[
\mathsf{Proj}_\pi : a \mapsto b
\]
is **sound** if:
\[
\exists_\pi\!\left(\gamma(a)\right)
\;\subseteq\;
\gamma(b).
\]

### Intuition

Projection must safely **over-approximate**
existential elimination.

### Typical Instances

- Quantifier elimination (over-approximate)
- Variable elimination
- CAD projection

---

## Axiom (L): Lifting Soundness

### Statement

Let $\pi$ be a projection.
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
without introducing invalid ones.

### Typical Instances

- CAD lifting
- Model reconstruction
- Back-substitution

---

## Summary

| Axiom | Semantic Role |
|------:|---------------|
| R | Sound contraction (no solution loss) |
| D | Sound space covering |
| I | Sound refutation |
| A | Sound over-approximation |
| C | Sound refinement |
| P | Sound projection |
| L | Sound lifting |

These axioms constitute the **entire semantic contract** of SemCal.
Any algorithm satisfying the relevant axioms is a valid SemCal instantiation.

---

## Approximation Directions

When implementing operators, it is crucial to specify the
**approximation direction** to ensure correct composition.

Each operator must declare how it approximates
the concrete meaning $\mathsf{Conc}(\sigma)$.

### Direction Types

- **PRESERVING**  
  Exact semantic preservation:
  \[
  \mathsf{Conc}(\sigma') = \mathsf{Conc}(\sigma)
  \]

- **OVER\_APPROX**  
  May introduce spurious models:
  \[
  \mathsf{Conc}(\sigma) \subseteq \mathsf{Conc}(\sigma')
  \]

- **UNDER\_APPROX**  
  May miss real models:
  \[
  \mathsf{Conc}(\sigma') \subseteq \mathsf{Conc}(\sigma)
  \]

- **REFUTE\_CERTIFIED**  
  Only for refutation:
  \[
  \text{If } \mathsf{Infeasible}(\sigma)=\textsf{UNSAT},
  \text{ then } \mathsf{Conc}(\sigma)=\emptyset
  \]

---

### Operator Directions

| Axiom | Default Direction | Notes |
|------:|------------------:|-------|
| R (Restrict) | PRESERVING | Sound contraction: no satisfying model removed |
| D (Decompose) | PRESERVING | Coverage: $\mathsf{Conc}(\sigma) \subseteq \bigcup_i \mathsf{Conc}(\sigma_i)$ |
| I (Infeasible) | REFUTE\_CERTIFIED | UNSAT claims must be globally correct |
| A (Relax) | OVER\_APPROX | By definition |
| C (Refine) | PRESERVING | Real models must be preserved |
| P (Project) | OVER\_APPROX | Existential elimination |
| L (Lift) | UNDER\_APPROX | Candidate reconstruction |

---

### Backend-Specific Directions

Different backends may instantiate operators with different directions:

- **CAD**
  - Project: OVER\_APPROX
  - Decompose: PRESERVING
  - Infeasible: REFUTE\_CERTIFIED
  - Lift: UNDER\_APPROX

- **LP / Simplex**
  - Relax: OVER\_APPROX (standard LP relaxation)
  - Infeasible: REFUTE\_CERTIFIED (Farkas certificate)
  - Refine: PRESERVING (cuts preserve real solutions)

- **ICP**
  - Restrict: PRESERVING (sound interval contraction)
  - Decompose: PRESERVING (box splitting)
  - Infeasible: REFUTE\_CERTIFIED (empty box detection)

**Critical.**
When composing operators, approximation directions must be compatible.
For example, composing an OVER\_APPROX operator with an UNDER\_APPROX
operator requires explicit semantic justification.

---
