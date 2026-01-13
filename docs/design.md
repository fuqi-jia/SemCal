# SemCal Design Document

## Overview

SemCal is a semantic calculus for constraint reasoning that provides a minimal axiomatization of semantic operators over model sets. This document describes the design decisions and architecture of the SemCal library.

## Core Design Principles

1. **Semantics-First**: All reasoning is defined over model sets, not syntax.
2. **Operator-Based**: Algorithms are instances of semantic operators.
3. **Solver-Agnostic**: No commitment to specific solver architectures.
4. **Minimal Axioms**: Only soundness requirements are specified.
5. **Composable**: Operators can be orchestrated to realize different tasks.

## Architecture

### Core Layer

The core layer defines the fundamental concepts:
- **Model**: Represents a satisfying assignment
- **Formula**: Represents a constraint
- **Semantics**: Defines the interpretation function [[·]]

### Domain Layer

The domain layer defines abstract domains:
- **AbstractElement**: Elements in the abstract domain
- **Concretization**: Maps abstract elements to model sets
- **Galois Connection**: Optional optimal abstraction/refinement

### State Layer

The state layer combines formulas and abstract elements:
- **SemanticState**: Pair (F, a) with meaning [[F]] ∩ γ(a)

### Operator Layer

The operator layer implements the seven semantic operators:
- **Restrict (R)**: Sound narrowing
- **Decompose (D)**: Sound space splitting
- **Infeasible (I)**: Sound refutation
- **Relax (A)**: Sound over-approximation
- **Refine (C)**: Sound refinement
- **Project (P)**: Sound projection
- **Lift (L)**: Sound lifting

### Orchestration Layer

The orchestration layer enables solver construction:
- **Pipeline**: Composes operators
- **Strategy**: Defines search strategies

## Naming Conventions

All code follows camelCase naming convention:
- Methods: `toString()`, `isSatisfiable()`, `getValue()`
- Variables: `modelSet`, `abstractElement`, `semanticState`

## Extension Points

SemCal is designed to be extended:
1. Implement custom `AbstractElement` subclasses
2. Implement custom operator classes
3. Create domain-specific `Semantics` implementations
4. Define custom search strategies

## Soundness Guarantee

Any reasoning pipeline using only operators satisfying the SemCal axioms is semantically sound by construction.
