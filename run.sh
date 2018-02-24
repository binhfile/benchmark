#!/bin/bash
#
# @author binhfile
# @brief Command for benchmark
# 
# $1 build dir
# $2 report dir
# ${@:3} options...
#
CUR_DIR="$(dirname "$(readlink -e "$0")")"
BUILD_DIR="$CUR_DIR"/_build
REPORT_DIR="$CUR_DIR"/_report

CMAKE=""
MAKE=""

function find_cmake() {
    CMAKE="`which cmake 2>/dev/null`"
    if [[ -z "$CMAKE" ]]; then
        CMAKE="`which cmake3 2>/dev/null`"
    fi
    return
}
function find_make() {
    MAKE="`which make 2>/dev/null`"
    if [[ -z "$MAKE" ]]; then
        MAKE="`which mingw32-make 2>/dev/null`"
    fi
    return
}
function find_python() {
    PYTHON="`which python 2>/dev/null`"
    if [[ -z "$MAKE" ]]; then
        PYTHON="`which python3 2>/dev/null`"
    fi
    return
}
function python_check_module() {
    if ! "$PYTHON" -c "import matplotlib" &> /dev/null; then
        echo 'matplotlib is not found'
        return 1
    fi
    if ! "$PYTHON" -c "import pandas" &> /dev/null; then
        echo 'pandas is not found'
        return 1
    fi
    return 0
}

function do_initialize() {
    mkdir -p "$BUILD_DIR"
    mkdir -p "$REPORT_DIR"
    return
}

# @param $1 mode
# @param ${@:2} plot option
function do_run() {
    if [[ ! -d "$BUILD_DIR" ]]; then
        echo "$BUILD_DIR is not existed" 1>&2
        exit 1
    fi 
    find_cmake
    find_make
    find_python
    if [[ -z "$CMAKE" ]];then
        echo "cmake not found"
        exit 1
    fi 
    if [[ -z "$MAKE" ]];then
        echo "make not found"
        exit 1
    fi 
    if [[ -z "$PYTHON" ]];then
        echo "python not found"
        exit 1
    fi 
    if [[ "`python_check_module`" == "0" ]];then
        exit 1
    fi

    cd "$BUILD_DIR"
    "$CMAKE" \
        -DCMAKE_BUILD_TYPE=Release \
        -DMODE="$1" \
        "$CUR_DIR" || exit 1
    "$MAKE" -j4 || exit 1
    if [[ "$2" == "--show" ]]; then 
        ./app.benchmark --benchmark_format=csv > benchmark.csv
        "$PYTHON" "$CUR_DIR"/plot.py -f "$BUILD_DIR"/benchmark.csv "${@:2}"
        if [[ -f "$BUILD_DIR"/output.png ]];then
            cp "$BUILD_DIR"/output.png "$REPORT_DIR"/"$1".png
        fi
    else
        ./app.benchmark
    fi 

    return
}
function do_clean() {
    if [[ ! -d "$BUILD_DIR" ]]; then
        echo "$BUILD_DIR is not existed" 1>&2
        exit 1
    fi 
    rm -rf "$BUILD_DIR"
    return
}

do_initialize

MODE="$1"
if [[ "$MODE" == "clean" ]];then
    do_clean
    exit 0
fi

do_run "${@:1}"
