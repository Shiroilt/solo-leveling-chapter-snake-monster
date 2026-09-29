#!/usr/bin/env bash
# check-lab2_3.sh — Lab 2.3 submission checker
# Validates required files, headings, commits, and measurements.
# Run from the repository root: bash lab2_3/check-lab2_3.sh

set -euo pipefail

PASS=0
FAIL=0
WARN=0

pass() { echo "  PASS  $1"; PASS=$((PASS+1)); }
fail() { echo "  FAIL  $1"; FAIL=$((FAIL+1)); }
warn() { echo "  WARN  $1"; WARN=$((WARN+1)); }

echo ""
echo "=== Lab 2.3 Checker ==="
echo ""

# --- 1. Required files ---
echo "--- Required files ---"
for f in \
    "lab2_3/REPORT.md" \
    "lab2_3/UBIQUITOUS_LANGUAGE.md" \
    "lab2_3/audits/main.md" \
    "lab2_3/audits/lab1-head.md" \
    "lab2_3/check-lab2_3.sh"
do
    if [ -f "$f" ]; then
        pass "File exists: $f"
    else
        fail "Missing file: $f"
    fi
done
echo ""

# --- 2. Required headings in REPORT.md ---
echo "--- REPORT.md headings ---"
REPORT="lab2_3/REPORT.md"
if [ -f "$REPORT" ]; then
    declare -a HEADINGS=(
        "Section 1"
        "Section 2"
        "Section 3"
        "Section 4"
        "Section 5"
        "Section 6"
        "Section 7"
        "Section 8"
    )
    for h in "${HEADINGS[@]}"; do
        if grep -qi "$h" "$REPORT"; then
            pass "Heading present: $h"
        else
            fail "Missing heading: $h"
        fi
    done

    # Check for unfilled placeholders
    if grep -q '___' "$REPORT"; then
        warn "REPORT.md still contains unfilled ___ placeholders"
    else
        pass "No unfilled ___ placeholders"
    fi
else
    fail "REPORT.md not found — skipping heading checks"
fi
echo ""

# --- 3. Rejected candidates (need at least 3) ---
echo "--- Section 4: Rejected candidates ---"
if [ -f "$REPORT" ]; then
    REJECT_COUNT=$(grep -c "^|" "$REPORT" 2>/dev/null | tail -1 || echo 0)
    # Count rows in section 4 table (rough heuristic)
    SEC4_LINES=$(awk '/Section 4/,/Section 5/' "$REPORT" 2>/dev/null | grep "^|" | grep -v "^\| *Smell" | grep -v "^\| *---" | wc -l || echo 0)
    if [ "$SEC4_LINES" -ge 3 ]; then
        pass "At least 3 rejected candidates found in Section 4 ($SEC4_LINES rows)"
    else
        warn "Fewer than 3 rejected candidates visible in Section 4 table (found approx $SEC4_LINES). Verify manually."
    fi
fi
echo ""

# --- 4. Commits in order on current branch ---
echo "--- Required commits ---"
BRANCH=$(git rev-parse --abbrev-ref HEAD)
echo "  Current branch: $BRANCH"

# Get commit messages in reverse order (oldest first among recent commits)
COMMITS=$(git log --oneline --pretty=format:"%s" HEAD~10..HEAD 2>/dev/null || git log --oneline --pretty=format:"%s" HEAD 2>/dev/null | head -10)

C1=0; C2=0; C3=0; C4=0
while IFS= read -r msg; do
    case "$msg" in
        *"ubiquitous language"*|*"ubiquitous-language"*|*"Ubiquitous"*) C1=1 ;;
        *"smell audit"*|*"code smell"*|*"Code smell"*) C2=1 ;;
        *"snake count"*|*"Refactor"*|*"refactor"*) C3=1 ;;
        *"second player"*|*"Add second"*|*"multiplayer"*) C4=1 ;;
    esac
done <<< "$COMMITS"

[ $C1 -eq 1 ] && pass "Commit 1 found: ubiquitous language glossary" || fail "Commit 1 NOT found: 'Add ubiquitous language glossary'"
[ $C2 -eq 1 ] && pass "Commit 2 found: code smell audit" || fail "Commit 2 NOT found: 'Add code smell audit'"
[ $C3 -eq 1 ] && pass "Commit 3 found: refactor snake count" || fail "Commit 3 NOT found: 'Refactor snake count representation'"
[ $C4 -eq 1 ] && pass "Commit 4 found: add second player" || fail "Commit 4 NOT found: 'Add second player'"
echo ""

# --- 5. Commit table printout ---
echo "--- Commit table (for Section 5) ---"
echo ""
echo "Branch: $BRANCH"
echo ""
git log --pretty=format:"| %h | %s | %ad |" --date=short HEAD~20..HEAD 2>/dev/null | grep -E "(ubiquitous|smell|refactor|Refactor|second player|Add second)" || \
git log --pretty=format:"| %h | %s | %ad |" --date=short -10 2>/dev/null
echo ""

# --- 6. Line count measurements (for Section 6) ---
echo "--- Run-2 measurements (Commit 4 vs Commit 3) ---"
echo ""

# Find commit 4 and commit 3 hashes
C4_HASH=$(git log --pretty=format:"%H %s" HEAD~10..HEAD 2>/dev/null | grep -i "second player\|Add second" | head -1 | awk '{print $1}' || true)
C3_HASH=$(git log --pretty=format:"%H %s" HEAD~10..HEAD 2>/dev/null | grep -i "refactor\|snake count" | head -1 | awk '{print $1}' || true)

if [ -n "$C4_HASH" ] && [ -n "$C3_HASH" ]; then
    echo "Commit 4 (feature) diff stat:"
    git diff "$C3_HASH" "$C4_HASH" -- snake0/ --stat 2>/dev/null || echo "(could not diff)"
    echo ""
    echo "Commit 3 (refactor) diff stat:"
    C2_HASH=$(git log --pretty=format:"%H %s" HEAD~10..HEAD 2>/dev/null | grep -i "smell audit\|code smell" | head -1 | awk '{print $1}' || true)
    if [ -n "$C2_HASH" ]; then
        git diff "$C2_HASH" "$C3_HASH" -- snake0/ --stat 2>/dev/null || echo "(could not diff)"
    fi
else
    warn "Could not locate commit 3 and 4 hashes — check manually"
    echo "Recent commits:"
    git log --oneline -8
fi
echo ""

# --- 7. Word count check (Q1-Q4 <= 500 words) ---
echo "--- Analysis word count (Sections 7+8, Q1-Q4) ---"
if [ -f "$REPORT" ]; then
    # Extract sections 7 and 8
    WC=$(awk '/Section 7/,0' "$REPORT" | wc -w || echo 0)
    echo "  Words in Sections 7+8: approx $WC"
    if [ "$WC" -gt 600 ]; then
        warn "Sections 7+8 appear to have >500 words — verify the Q1-Q4 block stays within 500"
    else
        pass "Word count looks within range"
    fi
fi
echo ""

# --- 8. Smell findings have file:line ---
echo "--- Smell findings cite file:line ---"
for audit in lab2_3/audits/main.md lab2_3/audits/lab1-head.md; do
    if [ -f "$audit" ]; then
        LINELESS=$(grep -i "smell\|finding" "$audit" 2>/dev/null | grep -v ":[0-9]" | wc -l || echo 0)
        if [ "$LINELESS" -gt 5 ]; then
            warn "$audit: several smell mentions may lack file:line — verify manually"
        else
            pass "$audit: smell findings appear to cite file:line"
        fi
    fi
done
echo ""

# --- Summary ---
echo "=== Summary ==="
echo "  PASS: $PASS"
echo "  WARN: $WARN"
echo "  FAIL: $FAIL"
echo ""
if [ $FAIL -eq 0 ]; then
    echo "  ✓ Checker passed. Review warnings above before submitting."
    exit 0
else
    echo "  ✗ Checker FAILED. Fix the items marked FAIL before submitting."
    exit 1
fi
