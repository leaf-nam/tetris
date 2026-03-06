#!/bin/bash
set -e

# 기본값 : 클린 빌드
CLEAN=true
BUILD_DEFAULT='x86'
COMPILER=g++
TEST=ON

for arg in "$@" ;do
    if [[ $arg == '--no-clear' ]] ;then
        CLEAN=false
    fi

    if [[ $arg == '--arm' ]] ;then
        BUILD_DEFAULT='arm'
    fi
done

if [[ $CLEAN = true ]] ;then 
    echo 'remove original build directory.'
    rm -rf build
fi

if [[ $BUILD_DEFAULT == 'arm' ]] ;then
    echo 'compile for arm.'
    COMPILER=arm-linux-gnueabihf-g++
    TEST=OFF
fi

cmake -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_COVERAGE=ON \
  -DCMAKE_CXX_COMPILER=$COMPILER \
  -DENABLE_TEST=$TEST

cmake --build build

case "$(uname)" in
  Linux) cmake --build build --target linux-test ;;
  MINGW*|MSYS*|CYGWIN*) cmake --build build --target windows-test ;;
esac
