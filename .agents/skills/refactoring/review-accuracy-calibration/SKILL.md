---
name: review-accuracy-calibration
description: Use when you want to improve the signal-to-noise ratio of any code review. Teaches confidence scoring, false positive reduction, severity calibration, and when to escalate vs. flag. Load this skill alongside review-cheat-sheet to ensure your findings are well-calibrated before posting comments.
---

# Review Accuracy and Calibration

## Overview

The accuracy problem in code review has two faces: over-flagging (false positives that waste reviewer and author time) and under-flagging (missing real defects). AI-assisted review tools generate false positives that waste 2-5 hours per developer per week, and 25% of AI suggestions contain errors. The fix is not reviewing less — it is calibrating more precisely.

## Quick Reference — Confidence Levels

| Level | Label | Post? | Severity floor |
|-------|-------|-------|----------------|
| C4 — Certain | You have evidence: test failure, spec violation, data loss | Yes | HIGH or CRITICAL |
| C3 — High | Strong reasoning: well-known anti-pattern, measurable impact | Yes | MEDIUM or higher |
| C2 — Medium | Plausible concern but depends on context you lack | Conditional | LOW or NIT |
| C1 — Low | Speculative; could be intentional or context-dependent | No (investigate first) | — |

## Confidence Scoring Model

### C4 — Certain
Direct evidence the code is wrong: test fails, spec violation, data loss provable, behavior contradicts PR description.

### C3 — High
Strong reasoning: well-documented anti-pattern (N+1, mutable default), clear performance/reliability impact, inconsistency with codebase pattern.

### C2 — Medium
Plausible concern but lacking full context: pattern looks wrong but could be intentional, performance concern depends on data volume, style inconsistency may follow team convention.

### C1 — Low
Speculative, style-only, or easily explained by context: you don't understand the domain and code might be correct, personal preference not backed by rule or measurable impact, would need 5 more files to know if it's a problem.

Action: Do not post C1. Investigate first.

## False Positive Reduction Heuristics

1. **Check Framework Conventions First** — verify it's not idiomatic for the framework/language.
2. **Read Surrounding Code** — expand context 20 lines before flagging.
3. **Distinguish Language Idiom from Bug** — know what is intentional in the language.
4. **Require Measurable Impact for Performance Findings** — "might be slow" without model = C1, drop it.
5. **Separate Style from Correctness** — style findings never at MEDIUM or higher severity.
