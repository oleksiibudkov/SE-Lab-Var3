#!/usr/bin/env bash
# Regression test: runs the program with every scenario *.in and compares the screen output
# (and results.txt, if the scenario saves it) with the reference recorded from the original
# single-file program. Usage:  bash tests/regression/run.sh [path/to/calculator]   (or: make test)
set -u
DIR="$(cd "$(dirname "$0")" && pwd)"
BIN="$(cd "$(dirname "${1:-build/calculator}")" && pwd)/$(basename "${1:-build/calculator}")"
[ -x "$BIN" ] || { echo "Program not found: $BIN (run make first)"; exit 1; }

passed=0; failed=0
for input in "$DIR"/*.in; do
  name="$(basename "$input" .in)"
  work="$(mktemp -d)"
  cp "$DIR"/fixtures/* "$work"/
  (cd "$work" && "$BIN" < "$input" > out.txt 2>&1; echo "exit code: $?" >> out.txt)

  ok=1
  diff -q "$DIR/$name.expected" "$work/out.txt" >/dev/null || ok=0
  if [ -f "$DIR/$name.results.expected" ]; then
    diff -q "$DIR/$name.results.expected" "$work/results.txt" >/dev/null 2>&1 || ok=0
  fi

  if [ $ok = 1 ]; then echo "  PASS  $name"; passed=$((passed + 1))
  else echo "  FAIL  $name"; diff "$DIR/$name.expected" "$work/out.txt" | head -n 10; failed=$((failed + 1)); fi
  rm -rf "$work"
done

echo "$passed passed, $failed failed"
[ $failed = 0 ]
