#!/usr/bin/env bash
#
# check-lab2_3.sh — run this from the root of your fork, on branch lab2_3/design-first,
# before you open the pull request.
#
#   bash lab2_3/check-lab2_3.sh
#
# It checks that your submission is complete and prints the numbers and the commit
# table you need for sections 5 and 6 of lab2_3/REPORT.md. Everything it checks is
# structure, not content — passing it does not mean the work is good, only that a
# marker will be able to find all of it.

set -u

FAILED=0
fail() { printf '[FAIL] %s\n' "$1"; FAILED=1; }
warn() { printf '[warn] %s\n' "$1"; }
ok()   { printf '[ ok ] %s\n' "$1"; }

git rev-parse --git-dir >/dev/null 2>&1 || {
  echo "[FAIL] not inside a git repository — run this from the root of your fork"
  exit 1
}
cd "$(git rev-parse --show-toplevel)" || exit 1

echo "=== 1. branch ==================================================="

BRANCH=$(git rev-parse --abbrev-ref HEAD)
if [ "$BRANCH" = "lab2_3/design-first" ]; then
  ok "on lab2_3/design-first"
else
  warn "you are on '$BRANCH', not 'lab2_3/design-first' — check you meant to be"
fi

BASE=""
for b in main master; do
  if git show-ref --verify --quiet "refs/heads/$b"; then BASE="$b"; break; fi
done
[ -n "$BASE" ] || fail "no local 'main' or 'master' branch to compare against"

echo
echo "=== 2. files ===================================================="

for f in lab2_3/REPORT.md \
         lab2_3/UBIQUITOUS_LANGUAGE.md \
         lab2_3/audits/main.md \
         lab2_3/audits/lab1-head.md; do
  if [ -f "$f" ]; then ok "$f"; else fail "missing: $f"; fi
done

for f in lab2_3/REPORT.md lab2_3/UBIQUITOUS_LANGUAGE.md; do
  [ -f "$f" ] || continue
  git ls-files --error-unmatch "$f" >/dev/null 2>&1 || fail "$f exists but is not committed"
done

echo
echo "=== 3. report structure ========================================="

R=lab2_3/REPORT.md
if [ -f "$R" ]; then
  for n in 1 2 3 4 5 6 7 8; do
    grep -q "^## $n\." "$R" || fail "REPORT.md has no '## $n.' heading — keep all eight, in order"
  done

  BLANKS=$(grep -c '___' "$R" || true)
  if [ "${BLANKS:-0}" -gt 0 ]; then
    fail "REPORT.md still has $BLANKS unfilled '___' placeholders"
  else
    ok "no unfilled placeholders"
  fi

  # section 4 — count table rows that are neither header, separator, nor blank
  ROWS=$(awk '/^## 4\./{s=1;next} /^## 5\./{s=0} s && /^\|/ && !/^\|[- |:]*\|$/ && !/smell reported/ && !/___/ {n++} END{print n+0}' "$R")
  if [ "${ROWS:-0}" -ge 3 ]; then
    ok "section 4 has $ROWS rejected-candidate rows (>= 3)"
  else
    fail "section 4 has only ${ROWS:-0} rejected-candidate rows — need at least 3"
  fi
fi

echo
echo "=== 4. commits =================================================="

BASE_REF=$(git rev-parse "$BASE")
COMMITS=$(git log --oneline "$BASE_REF"..HEAD -- lab2_3/ snake0/ 2>/dev/null | wc -l | tr -d ' ')
ok "commits on this branch ahead of $BASE: $COMMITS"
if [ "${COMMITS:-0}" -lt 4 ]; then
  fail "need at least 4 commits (glossary, smells, refactor, feature) — found $COMMITS"
fi

echo
echo "=== 5. commit table ============================================="
echo
echo "Paste this into section 5 of lab2_3/REPORT.md:"
echo
printf "| # | SHA | Message | Files changed | Lines added | Lines removed |\n"
printf "|---|-----|---------|---------------|-------------|---------------|\n"
N=0
git log --reverse "$BASE_REF"..HEAD --format="%H|%s" -- lab2_3/ snake0/ | while IFS='|' read -r sha msg; do
  N=$((N+1))
  STAT=$(git diff --shortstat "${sha}^" "$sha" 2>/dev/null || echo "0 files, 0 ins, 0 del")
  FILES=$(echo "$STAT" | grep -oP '\d+ file' | grep -oP '\d+' || echo 0)
  INS=$(echo "$STAT"   | grep -oP '\d+ ins'  | grep -oP '\d+' || echo 0)
  DEL=$(echo "$STAT"   | grep -oP '\d+ del'  | grep -oP '\d+' || echo 0)
  printf "| %d | \`%s\` | %s | %s | +%s | -%s |\n" "$N" "${sha:0:7}" "$msg" "$FILES" "$INS" "$DEL"
done

echo
echo "=== 6. line counts (run 2) ======================================"
echo
echo "Paste the feature row into section 6 of lab2_3/REPORT.md:"
echo "(Compare commit 4 alone to your Lab-1 diff for the same feature)"
echo
# Commit 4 is the 4th commit from base; count lines changed in snake0/ only
COMMIT4=$(git log --reverse "$BASE_REF"..HEAD --format="%H" -- lab2_3/ snake0/ | sed -n '4p')
if [ -n "$COMMIT4" ]; then
  STAT4=$(git diff --shortstat "${COMMIT4}^" "$COMMIT4" -- snake0/ 2>/dev/null || echo "")
  echo "Commit 4 (feature): $STAT4"
  COMMIT3=$(git log --reverse "$BASE_REF"..HEAD --format="%H" -- lab2_3/ snake0/ | sed -n '3p')
  STAT3=$(git diff --shortstat "${COMMIT3}^" "$COMMIT3" -- snake0/ 2>/dev/null || echo "")
  echo "Commit 3 (refactor): $STAT3"
else
  warn "fewer than 4 commits — line count for commit 4 unavailable"
fi

echo
echo "=== result ======================================================"
if [ "$FAILED" -eq 0 ]; then
  echo "All checks passed. Open your PR when ready."
else
  echo "Fix the FAIL lines above before opening the PR."
fi
exit "$FAILED"
