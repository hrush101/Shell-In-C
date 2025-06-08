#!/bin/sh

set -e # Exit early if any commands fail
echo "Remote architecture: $(ssh $remote_host uname -m)"

cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory



if [ "$(uname -m)" = "x86_64" ]; then
   
  if ! brew list readline >/dev/null 2>&1; then
    brew install readline
  fi

  # Intel macOS (Homebrew default prefix)
  export CPPFLAGS="-I/usr/local/opt/readline/include"
  export LDFLAGS="-L/usr/local/opt/readline/lib"
fi

gcc $CPPFLAGS $LDFLAGS app/*.c -o /tmp/shell-target

exec /tmp/shell-target "$@"