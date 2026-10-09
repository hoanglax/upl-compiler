#!/usr/bin/env bash
# Chạy: make test   (hoặc bash tests/run_tests.sh từ thư mục gốc)
cd "$(dirname "$0")/.." || exit 2
BIN=build/upl
pass=0; fail=0

# run_dir <thư mục con của tests/> <mã thoát mong đợi>
run_dir() {
    for f in tests/$1/*.upl; do
        [ -e "$f" ] || continue
        "$BIN" "$f" >/dev/null 2>&1
        code=$?
        if [ "$code" -eq "$2" ]; then
            echo "PASS  $f"; pass=$((pass+1))
        else
            echo "FAIL  $f (exit $code, mong đợi $2)"; fail=$((fail+1))
        fi
    done
}

run_dir valid 0
run_dir lexical_errors 1
run_dir syntax_errors 1

echo "----"
echo "Passed: $pass, Failed: $fail"
[ "$fail" -eq 0 ]