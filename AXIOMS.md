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
- $\llbracket F \rrbracket \subseteq \mathcal{M}$ denote its model set.
- $(\mathcal{A}, \sqsubseteq)$ be an abstract domain.
- $\gamma : \mathcal{A} \to \wp(\mathcal{M})$ be a concretization function.

A semantic state is a pair $(F, a)$ with concrete meaning:
$$
\mathsf{Conc}(F,a) = \llbracket F \rrbracket \cap \gamma(a).
$$

---

## Axiom (R): Restriction Soundness

### Statement

For a restriction operator $\mathsf{Restrict}_F : \mathcal{A} \to \mathcal{A}$:
$$
\gamma(\mathsf{Restrict}_F(a))
\;\supseteq\;
\gamma(a) \cap \llbracket F \rrbracket.
$$

### Intuition

Restriction may *remove spurious candidates*,
but must never remove a genuine satisfying model.

### Typical Instances

- Interval narrowing / bound tightening
- Constraint propagation
- Word-level range propagation

---

## Axiom (D): Decomposition Soundness

### Statement

For a decomposition operator $\mathsf{Decomp} : \mathcal{A} \to \mathcal{P}_{\mathrm{fin}}(\mathcal{A})$:
$$
\gamma(a)
\;\subseteq\;
\bigcup_{a_i \in \mathsf{Decomp}(a)} \gamma(a_i).
$$

### Intuition

Decomposition splits the search space,
but must **cover** all possibilities.

### Typical Instances

- Case splitting
- Branching on variables
- Cell decomposition (e.g., CAD, CAC)

---

## Axiom (I): Local Infeasibility Soundness

### Statement

If $\mathsf{Infeasible}(F,a)=\textsf{true}$, then:
$$
\llbracket F \rrbracket \cap \gamma(a) = \emptyset.
$$

### Intuition

Declaring a region infeasible must be *globally correct*.

### Typical Instances

- Cell emptiness checks
- Box refutation
- Theory-level conflict detection

---

## Axiom (A): Relaxation Soundness

### Statement

For a relaxation operator $\mathsf{Relax}$:
$$
\llbracket F \rrbracket \subseteq \llbracket \mathsf{Relax}(F) \rrbracket.
$$

### Intuition

Relaxation may admit extra models,
but must preserve all real ones.

### Typical Instances

- Predicate abstraction
- Bit-level abstraction
- Linearization of nonlinear constraints

---

## Axiom (C): Restoration (Refinement) Soundness

### Statement

Given a spurious model $M \models F^\alpha$ with $M \not\models F$,
a refinement constraint $R$ must satisfy:
$$
\llbracket F \rrbracket
\subseteq
\llbracket F^\alpha \land R \rrbracket
\quad\text{and}\quad
M \not\models R.
$$

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

For a projection operator $\mathsf{Proj}_\pi$:
$$
\exists_\pi(\gamma(a))
\subseteq
\gamma_k(\mathsf{Proj}_\pi(a)).
$$

### Intuition

Projection must safely over-approximate existential elimination.

### Typical Instances

- Quantifier elimination
- Variable elimination
- CAD projection

---

## Axiom (L): Lifting Soundness

### Statement

For a lifting operator $\mathsf{Lift}_\pi$:
$$
\gamma(\mathsf{Lift}_\pi(b))
\subseteq
\pi^{-1}(\gamma_k(b)).
$$

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
| R | Sound narrowing |
| D | Sound space splitting |
| I | Sound refutation |
| A | Sound over-approximation |
| C | Sound refinement |
| P | Sound projection |
| L | Sound lifting |

These axioms constitute the **entire semantic contract** of SemCal.
Any algorithm satisfying the relevant axioms is a valid SemCal instantiation.

---
