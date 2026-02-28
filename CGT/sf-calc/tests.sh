#!/usr/bin/env bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

SRC_DIR="src"
INCLUDE_DIR="include"
TEST_DIR="tests"
BUILD_DIR="build"

mkdir -p "$BUILD_DIR"

declare -A tests
tests["frac"]="$TEST_DIR/test_frac.cpp $SRC_DIR/UFrac.cpp"
tests["editor"]="$TEST_DIR/test_editor.cpp $SRC_DIR/UFrac.cpp $SRC_DIR/UEditor.cpp"
tests["memory"]="$TEST_DIR/test_memory.cpp $SRC_DIR/UFrac.cpp"
tests["proc"]="$TEST_DIR/test_proc.cpp $SRC_DIR/TProc.cpp $SRC_DIR/UFrac.cpp"
tests["ctrl"]="$TEST_DIR/test_ctrl.cpp $SRC_DIR/UCtrl.cpp $SRC_DIR/TProc.cpp $SRC_DIR/UEditor.cpp $SRC_DIR/UFrac.cpp"

headers=("UFrac.h" "UEditor.h" "UMemory.h" "TProc.h" "CalcEnums.h")

run_test() {
    local name=$1
    local sources=${tests[$name]}
    local executable="$BUILD_DIR/test_${name}"

    echo -e "${YELLOW}Компиляция test_${name}...${NC}"
    g++ -std=c++11 -I"$INCLUDE_DIR" -o "$executable" $sources
    if [ $? -ne 0 ]; then
        echo -e "${RED}Ошибка компиляции test_${name}${NC}"
        return 1
    fi

    echo -e "${YELLOW}Запуск test_${name}...${NC}"
    output=$("$executable" 2>&1)
    ret=$?
    echo "$output"

    failed_line=$(echo "$output" | grep -i "failed checks" | tail -n1)
    if [[ -z "$failed_line" ]]; then
        if [ $ret -eq 0 ]; then
            echo -e "${GREEN}test_${name}: ALL PASS${NC}"
            return 0
        else
            echo -e "${RED}test_${name}: FAILED (unknown)${NC}"
            return 1
        fi
    else
        failed=$(echo "$failed_line" | awk -F':' '{print $NF}' | tr -d ' ')
        total=$(echo "$output" | grep -i "total checks" | tail -n1 | awk -F':' '{print $NF}' | tr -d ' ')
        if [ "$failed" -eq 0 ]; then
            echo -e "${GREEN}test_${name}: ALL PASS (total $total checks)${NC}"
            return 0
        else
            echo -e "${RED}test_${name}: $failed FAILED (total $total checks)${NC}"
            return 1
        fi
    fi
}

if [ $# -eq 0 ]; then
    all_passed=0
    for test in "${!tests[@]}"; do
        run_test "$test"
        if [ $? -ne 0 ]; then
            all_passed=1
        fi
        echo
    done
    if [ $all_passed -eq 0 ]; then
        echo -e "${GREEN}Все тесты пройдены успешно.${NC}"
    else
        echo -e "${RED}Некоторые тесты не пройдены.${NC}"
    fi
else
    for arg in "$@"; do
        if [[ -v tests[$arg] ]]; then
            run_test "$arg"
            echo
        else
            echo -e "${RED}Неизвестный тест: $arg${NC}"
            echo "Доступные тесты: ${!tests[@]}"
            exit 1
        fi
    done
fi
