#!/bin/bash
set -e

# 기본값 : 클린 빌드
CLEAN=true

for arg in "$@" ;do
    if [[ $arg == '--no-clear' ]] ;then
        CLEAN=false
    fi
done

if [[ $CLEAN = true ]] ;then 
    echo 'remove original build directory.'
    rm -rf build
fi

cmake -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_COVERAGE=ON

cmake --build build

case "$(uname)" in
  Linux) cmake --build build --target linux-test ;;
  MINGW*|MSYS*|CYGWIN*) cmake --build build --target windows-test ;;
esac
