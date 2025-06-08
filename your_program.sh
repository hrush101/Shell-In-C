#!/bin/sh

set -e # Exit early if any commands fail

cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory
sudo apt-get install libreadline-dev
export CPPFLAGS="-I/usr/include/readline"
export LDFLAGS="-L/usr/lib/x86_64-linux-gnu"

gcc $CPPFLAGS $LDFLAGS app/*.c -o /tmp/shell-target

exec /tmp/shell-target "$@"