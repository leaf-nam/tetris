#!/bin/bash
set -e

rm -rf build
cmake -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_COVERAGE=ON

cmake --build build

case "$(uname)" in
  Linux) cmake --build build --target linux-test ;;
  MINGW*|MSYS*|CYGWIN*) cmake --build build --target windows-test ;;
esac
