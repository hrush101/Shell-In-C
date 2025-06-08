#!/bin/sh

set -e # Exit early if any commands fail

cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory
sudo dnf install readline-devel
export CPPFLAGS="-I/usr/include"
export LDFLAGS="-L/usr/lib64"

gcc $CPPFLAGS $LDFLAGS -lreadline app/*.c -o /tmp/shell-target

exec /tmp/shell-target "$@"