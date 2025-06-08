#!/bin/sh

set -e # Exit early if any commands fail

cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory
if [[ "$(uname -m)" == "arm64"  ]];then
  export CPPFLAGS="-I/usr/local/opt/readline/include"
  export LDFLAGS="-L/usr/local/opt/readline/lib"
fi

gcc app/*.c -o /tmp/shell-target


exec /tmp/shell-target "$@"