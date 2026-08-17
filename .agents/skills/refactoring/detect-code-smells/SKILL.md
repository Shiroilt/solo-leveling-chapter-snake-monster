---
name: detect-code-smells
description: Use when reviewing code for quality issues, when code feels hard to change or understand, when preparing for refactoring, or when a module has grown unwieldy
---

# Detect Code Smells

## Overview

Code smells are surface indicators of deeper structural problems. They don't cause bugs directly but signal design weaknesses that slow development and increase bug risk. Detecting smells is the first step before applying targeted refactoring.

## When to Use

- Code review reveals hard-to-read or hard-to-change sections
- A class or method has grown significantly over time
- Adding a feature requires touching many unrelated files
- Copy-pasted logic across the codebase
- Tests are brittle or hard to write for a module

## Quick Reference

| Category | Smell | Key Symptom | Primary Fix (Skill) |
|----------|-------|-------------|---------------------|
| **Bloaters** | Long Method | Method > 20 lines doing multiple things | `refactor-composing-methods` |
| **Bloaters** | Large Class | Too many fields/methods/lines | `refactor-moving-features` |
| **Bloaters** | Primitive Obsession | Primitives instead of small objects | `refactor-organizing-data` |
| **Bloaters** | Long Parameter List | 4+ parameters | `refactor-simplifying-method-calls` |
| **Bloaters** | Data Clumps | Same variables appear together repeatedly | `refactor-organizing-data` |
| **OO Abusers** | Switch Statements | Complex switch/if-else on type codes | `refactor-simplifying-conditionals` |
| **OO Abusers** | Temporary Field | Fields only set in certain circumstances | `refactor-organizing-data` |
| **OO Abusers** | Refused Bequest | Subclass uses little of parent's interface | `refactor-generalization` |
| **OO Abusers** | Alternative Classes w/ Different Interfaces | Same thing, different method names | `refactor-generalization` |
| **Change Preventers** | Divergent Change | One class changed for many different reasons | `refactor-moving-features` |
| **Change Preventers** | Shotgun Surgery | One change requires edits across many classes | `refactor-moving-features` |
| **Change Preventers** | Parallel Inheritance Hierarchies | Adding subclass in one hierarchy requires another | `refactor-generalization` |
| **Dispensables** | Comments (excessive) | Code needs extensive comments to be understood | `refactor-composing-methods` |
| **Dispensables** | Duplicate Code | Identical or similar code in multiple places | `refactor-composing-methods` |
| **Dispensables** | Lazy Class | Class does too little to justify existence | `refactor-moving-features` |
| **Dispensables** | Data Class | Only fields and getters/setters, no behavior | `refactor-organizing-data` |
| **Dispensables** | Dead Code | Unreachable or unused code | `refactor-composing-methods` |
| **Dispensables** | Speculative Generality | Unused abstractions "just in case" | `refactor-generalization` |
| **Couplers** | Feature Envy | Method uses another class's data more than its own | `refactor-moving-features` |
| **Couplers** | Inappropriate Intimacy | Classes access each other's internals excessively | `refactor-moving-features` |
| **Couplers** | Message Chains | `a.getB().getC().getD()` chains | `refactor-simplifying-method-calls` |
| **Couplers** | Middle Man | Class delegates most work to another | `refactor-simplifying-method-calls` |
| **Couplers** | Incomplete Library Class | Library missing needed functionality | `refactor-simplifying-method-calls` |
