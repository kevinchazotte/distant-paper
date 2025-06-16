#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
for p in distant-paper; do
  for c in Release RelWithDebInfo Debug; do
    echo
    echo "Configuring project \"$p\" with config \"$c\" to: \"$SCRIPT_DIR/../build/bld/x64/$c/$p/\""
    echo
    cmake -DCMAKE_BUILD_TYPE=$c -DCMAKE_PREFIX_PATH="$SCRIPT_DIR/../build/x64/$c" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -S "$SCRIPT_DIR/../../$p/" -B "$SCRIPT_DIR/../build/bld/x64/$c/$p/"
    cmake --build "$SCRIPT_DIR/../build/bld/x64/$c/$p/"
  done
done

