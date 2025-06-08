#!/bin/sh

set -e # Exit early if any commands fail
echo "Remote architecture: $(ssh $remote_host uname -m)"

cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory

if [ "$(uname -m)" = "arm64" ]; then
  # Apple Silicon macOS
  export CPPFLAGS="-I/opt/homebrew/Cellar/readline/8.2.13/include"
  export LDFLAGS="-L/opt/homebrew/Cellar/readline/8.2.13/lib"
elif [ "$(uname -m)" = "x86_64" ]; then
  # Intel macOS (Homebrew default prefix)
  export CPPFLAGS="-I/usr/local/opt/readline/include"
  export LDFLAGS="-L/usr/local/opt/readline/lib"
fi

gcc $CPPFLAGS $LDFLAGS app/*.c -o /tmp/shell-target


exec /tmp/shell-target "$@"