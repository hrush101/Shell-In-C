#!/bin/bash
set -e

cd "$(dirname "$0")"

if [[ "$(uname -m)" == "x86_64" ]]; then
  export CPPFLAGS="-I/opt/homebrew/Cellar/readline/8.2.13/include"
  export LDFLAGS="-L/opt/homebrew/Cellar/readline/8.2.13/lib"
fi

cmake -B build -S .
cmake --build build

exec ./build/shell "$@"