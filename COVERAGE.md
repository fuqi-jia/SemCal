# SemCal Coverage
## Mapping SMT Reasoning Tasks to Semantic Operators

This document maps common SMT reasoning tasks
to the semantic operators defined in SemCal.

The purpose is to clarify **scope, coverage, and intentional exclusions**.

---

## Legend

- ✅ Direct semantic support
- ◻️ Supported via orchestration
- ❌ Outside semantic scope (non-formalizable or heuristic)

---

## Q0: Formalizability

| Category | Coverage |
|--------|----------|
| Formalizable | ✅ |
| Partially formalizable | ◻️ (formal core only) |
| Non-formalizable | ❌ |

---

## Q1–Q3: Output Shape

| Task | SemCal Mapping |
|-----|----------------|
| Full assignment | $\gamma(a)$ is singleton |
| Partial assignment | $|\gamma(a)| > 1$ |
| Single solution | SAT query |
| Multiple solutions | Decomposition + aggregation |

---

## Q4: Single-Solution Objectives

| Task | Coverage | Operators |
|-----|----------|-----------|
| Feasible | ✅ | R, I |
| Optimal | ◻️ | R, P, A |
| Anytime | ◻️ | R, D |

---

## Q5: Multi-Solution Objectives

| Task | Coverage | Operators |
|-----|----------|-----------|
| Enumeration | ◻️ | D, I |
| Diverse models | ◻️ | A, R |
| Model counting | ◻️ | D + aggregation |
| Volume integration | ◻️ | D + measure |

---

## Q6: Constraint Operations

### Reduction (Equivalence-Preserving)

| Task | Coverage | Operator |
|-----|----------|----------|
| Constraint simplification | ✅ | Representation layer |
| Pre/In-processing | ✅ | Representation layer |
| Variable elimination (exact) | ✅ | P |

---

### Selection / Diagnosis

| Task | Coverage | Operator |
|-----|----------|----------|
| UNSAT core | ◻️ | I + subset selection |
| MUS / MSS / MCS | ◻️ | I + D |
| Assumption diagnosis | ◻️ | I |

---

### Generation / Synthesis

| Task | Coverage | Operator |
|-----|----------|----------|
| Lemma generation | ✅ | C |
| Cut generation | ✅ | C |
| Invariant synthesis | ◻️ | C + D |

---

### Abstraction / Refinement

| Task | Coverage | Operator |
|-----|----------|----------|
| Predicate abstraction | ✅ | A |
| Numeric abstraction | ✅ | A |
| Symbolic abstraction | ✅ | A |
| Refinement | ✅ | C |

---

### Backbone / Backdoor / Autarky

| Task | Coverage | Operator |
|-----|----------|----------|
| Backbone computation | ◻️ | R + D |
| Backdoor detection | ◻️ | D |
| Autarky detection | ◻️ | I |

---

## SMT-Level Reasoning

| Task | Coverage | Operator |
|-----|----------|----------|
| SAT / SMT feasibility | ✅ | I |
| δ-SAT | ✅ | A |
| Incomplete solving | ◻️ | R |
| Local search | ◻️ | R |

---

## Quantification and Enumeration

| Task | Coverage | Operator |
|-----|----------|----------|
| Quantifier elimination | ✅ | P |
| All-SMT | ◻️ | D |
| Projected counting | ◻️ | P + D |

---

## Optimization and Counting

| Task | Coverage | Operator |
|-----|----------|----------|
| OMT / MaxSMT | ◻️ | R + A |
| ε-optimal | ◻️ | A |
| Weighted counting | ◻️ | D + aggregation |

---

## Underlying Theories

| Theory | Coverage |
|------|----------|
| Integer Arithmetic | ✅ |
| Real Arithmetic | ✅ |
| Bit-Vectors | ✅ |
| Floating-Point | ◻️ (encoding-dependent) |
| Arrays / UF / Strings | ✅ |
| Pure SAT | ✅ |
| Theory combination | ✅ |

---

## Summary

SemCal provides **complete semantic coverage** for all
*formalizable* SMT reasoning categories.

Algorithmic strategies, heuristics, and termination guarantees
are intentionally excluded and treated as *applications*.

---
