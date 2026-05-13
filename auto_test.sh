#!/bin/bash

EXEC="./mysh"
PASS=0
FAIL=0

echo "=============================="
echo "  MYSH AUTOMATED TEST SUITE"
echo "=============================="

run_test() {
    NAME="$1"
    CMD="$2"
    EXPECT="$3"

    OUTPUT=$(echo -e "$CMD" | $EXEC 2>&1)

    if [[ "$OUTPUT" == *"$EXPECT"* ]]; then
        echo "[PASS] $NAME"
        ((PASS++))
    else
        echo "[FAIL] $NAME"
        echo "  Expected: $EXPECT"
        echo "  Got: $OUTPUT"
        ((FAIL++))
    fi
}

# ---------------------------
# BASIC COMMANDS
# ---------------------------

run_test "Echo command" "echo hello" "hello"
run_test "Pwd command" "pwd" "CMSC125_LabWork"
run_test "List files" "ls" ""

# ---------------------------
# BUILT-INS
# ---------------------------

run_test "CD command" "cd /\npwd" "/"
run_test "Exit command" "exit" ""

# ---------------------------
# NON-EXISTENT COMMAND
# ---------------------------

run_test "Invalid command" "fakecommand" "not found"

# ---------------------------
# I/O REDIRECTION
# ---------------------------

echo "hello" > input.txt
run_test "Input redirection" "wc -l < input.txt" "1"

run_test "Output redirection" "echo test > out.txt\ncat out.txt" "test"

run_test "Append redirection" "echo a >> out.txt\necho b >> out.txt\ncat out.txt" "a"

# ---------------------------
# BACKGROUND JOBS
# ---------------------------

run_test "Background job" "sleep 1 &" "Started"

# ---------------------------
# EDGE CASES
# ---------------------------

run_test "Empty input" "" ""

run_test "Whitespace input" "     " ""

# ---------------------------
# SUMMARY
# ---------------------------

echo "=============================="
echo "PASS: $PASS"
echo "FAIL: $FAIL"
echo "=============================="

if [ $FAIL -eq 0 ]; then
    echo "ALL TESTS PASSED 🎉"
else
    echo "SOME TESTS FAILED ❌"
fi