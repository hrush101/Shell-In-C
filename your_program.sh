#!/bin/sh

set -e # Exit early if any commands fail

cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory

gcc app/*.c -o /H_shell-target

exec /H_shell-target "$@"