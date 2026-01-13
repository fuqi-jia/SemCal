# SemCal
## A Semantic Calculus for Constraint Reasoning

**SemCal** is a solver-agnostic semantic calculus for logical constraint reasoning.
It provides a **minimal axiomatization of semantic operators** over model sets,
which underlie SMT-style satisfiability, optimization, counting, and sampling.

SemCal is **not a solver** and **does not prescribe algorithms**.
Instead, it formalizes *what it means* for reasoning components to be sound,
independently of how they are implemented (e.g., CAD, ICP, SAT, bit-blasting).

---

## 1. Design Philosophy

SemCal is built on the following principles:

- **Semantics-first**: reasoning is defined over model sets, not syntax.
- **Operator-based**: algorithms are instances of semantic operators.
- **Solver-agnostic**: no commitment to DPLL(T), CDCL, CAD, or any backend.
- **Minimal axioms**: only soundness requirements are specified.
- **Composable**: operators can be orchestrated to realize different tasks.

The goal of SemCal is to serve as a **semantic foundation** for:
- SMT solving
- Optimization Modulo Theories (OMT)
- Model counting and volume computation
- Symbolic sampling and refutation

---

## 2. Semantic Core

### 2.1 Models and Satisfaction

Fix a background theory (or a combination of theories) with a set of models:
\[
\mathcal{M}.
\]

For a constraint (formula) \(F\), its **model set** is defined as:
\[
\llbracket F \rrbracket \;=\; \{\, M \in \mathcal{M} \mid M \models F \,\}.
\]

### 2.2 Logical Equivalence

Two constraints \(F\) and \(G\) are **logically equivalent** iff:
\[
F \equiv G \quad\Longleftrightarrow\quad \llbracket F \rrbracket = \llbracket G \rrbracket.
\]

The semantic core defines *meaning only*.
No reasoning, search, or approximation is performed at this level.

---

## 3. Representation Layer

The representation layer consists of transformations on constraints that
preserve (or explicitly relate) semantic meaning.

### 3.1 Constraint-Preserving Transformations

A transformation
\[
T : F \mapsto F'
\]
is **semantics-preserving** if:
\[
\llbracket F \rrbracket = \llbracket F' \rrbracket.
\]

Typical instances include:
- normalization
- purification (theory separation)
- introduction of auxiliary variables
- syntactic rewriting

### 3.2 Semantic Encoding

An encoding
\[
\mathsf{Enc} : F \mapsto F'
\]
must explicitly specify its semantic contract, e.g.:

- equisatisfiability, or
- model preservation with a decoding function.

Encodings are **representation-level operators**, not reasoning operators.

---

## 4. Abstract Domains and Concretization

### 4.1 Abstract Domain

SemCal reasons about approximations of model sets via an abstract domain:
\[
(\mathcal{A}, \sqsubseteq).
\]

Examples include:
- boxes or regions
- symbolic cells or covers
- partial assignments
- clause sets

### 4.2 Concretization

A **concretization function** maps abstract elements to model sets:
\[
\gamma : \mathcal{A} \to \wp(\mathcal{M}).
\]

### 4.3 Galois Connection (Optional)

When available, an abstraction function
\[
\alpha : \wp(\mathcal{M}) \to \mathcal{A}
\]
forms a Galois connection with \(\gamma\) if:
\[
\alpha(S) \sqsubseteq a
\;\Longleftrightarrow\;
S \subseteq \gamma(a).
\]

A Galois connection is **not required** for soundness, but enables optimal
abstraction and refinement.

---

## 5. Semantic States

A **semantic state** is a pair:
\[
\sigma = (F, a), \quad a \in \mathcal{A}.
\]

Its concrete meaning is:
\[
\mathsf{Conc}(\sigma)
\;=\;
\llbracket F \rrbracket \cap \gamma(a).
\]

All semantic operators are required to be sound with respect to this meaning.

---

## 6. Semantic Space Operators (Core of SemCal)

Semantic operators transform semantic states or their components.
They are defined solely by **soundness axioms**.

---

### 6.1 Restriction (Narrowing / Propagation)

A restriction operator:
\[
\mathsf{Restrict}_F : \mathcal{A} \to \mathcal{A}.
\]

**Soundness (R):**
\[
\gamma(\mathsf{Restrict}_F(a))
\;\supseteq\;
\gamma(a) \cap \llbracket F \rrbracket.
\]

Restriction may optionally satisfy progress:
\[
\mathsf{Restrict}_F(a) \sqsubseteq a.
\]

---

### 6.2 Decomposition (Splitting / Partition)

A decomposition operator:
\[
\mathsf{Decomp} : \mathcal{A} \to \mathcal{P}_{\mathrm{fin}}(\mathcal{A}).
\]

**Covering (D):**
\[
\gamma(a)
\;\subseteq\;
\bigcup_{a_i \in \mathsf{Decomp}(a)} \gamma(a_i).
\]

---

### 6.3 Local Infeasibility

A predicate:
\[
\mathsf{Infeasible}(F,a) \in \{\textsf{true},\textsf{false},\textsf{unknown}\}.
\]

**Soundness (I):**
\[
\mathsf{Infeasible}(F,a)=\textsf{true}
\;\Rightarrow\;
\llbracket F \rrbracket \cap \gamma(a) = \emptyset.
\]

---

### 6.4 Relaxation (Semantic Over-Approximation)

A relaxation operator:
\[
\mathsf{Relax} : F \mapsto F^\alpha.
\]

**Over-Approximation (A):**
\[
\llbracket F \rrbracket \subseteq \llbracket F^\alpha \rrbracket.
\]

---

### 6.5 Restoration (Refinement)

Given a spurious model \(M \models F^\alpha\) with \(M \not\models F\),
a restoration operator produces a constraint \(R\).

**Refinement (C):**
\[
\llbracket F \rrbracket
\subseteq
\llbracket F^\alpha \land R \rrbracket
\quad\text{and}\quad
M \not\models R.
\]

---

### 6.6 Projection and Lifting

Let \(\pi : \mathcal{M} \to \mathcal{M}_k\) be a forgetful mapping.

**Projection (P):**
\[
\exists_\pi(\gamma(a))
\subseteq
\gamma_k(\mathsf{Proj}_\pi(a)).
\]

**Lifting (L):**
\[
\gamma(\mathsf{Lift}_\pi(b))
\subseteq
\pi^{-1}(\gamma_k(b)).
\]

---

## 7. Applications (Outside SemCal)

SemCal does not define applications, but supports them via orchestration:

- **SAT**: is \(\llbracket F \rrbracket \neq \emptyset\)?
- **OPT**: extremal values over \(\llbracket F \rrbracket\)
- **COUNT / VOLUME**: measure of \(\llbracket F \rrbracket\)
- **SAMPLE**: draw elements from \(\llbracket F \rrbracket\)

---

## 8. Soundness Guarantee

Any reasoning pipeline that uses only operators satisfying the SemCal axioms
(R, D, I, A, C, P, L) is **semantically sound** with respect to the original
constraint \(F\).

Completeness and termination are deliberately left unspecified.

---

## 9. Instantiation Policy

Concrete algorithms (e.g., CAD, ICP, bit-level reasoning) may be integrated
*only by demonstrating* that they satisfy the corresponding semantic axioms.

SemCal privileges **no algorithm**.

---

## 10. Scope and Non-Goals

SemCal intentionally does **not** provide:

- a solver
- a search strategy
- a termination guarantee
- a preferred theory

Its role is to define the **semantic contract** that all such systems rely on.

---

## 11. Citation

If you use SemCal in academic work, please cite it as:

> *SemCal: A Semantic Calculus for Constraint Reasoning.*

(Full citation to appear.)

---
