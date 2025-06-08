#!/bin/sh

set -e # Exit early if any commands fail
echo "Remote architecture: $(ssh $remote_host uname -m)"

cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory

if [ "$OS" = "Linux" ]; then
  if [ "$(command -v apt-get)" ]; then
    echo "Detected Debian-based system (e.g., Ubuntu). Installing readline..."
    sudo apt-get update
    sudo apt-get install -y libreadline-dev
  elif [ "$(command -v yum)" ]; then
    echo "Detected RHEL/CentOS-based system. Installing readline..."
    sudo yum install -y readline-devel
  elif [ "$(command -v dnf)" ]; then
    echo "Detected Fedora system. Installing readline..."
    sudo dnf install -y readline-devel
  else
    echo "Unsupported Linux package manager. Please install readline manually."
  fi

  # These are usually default paths on Linux, but you can set explicitly if needed:
  export CPPFLAGS="-I/usr/include"
  export LDFLAGS="-L/usr/lib"

  # If the system uses /usr/lib64 for 64-bit
  if [ "$ARCH" = "x86_64" ] && [ -d "/usr/lib64" ]; then
    export LDFLAGS="-L/usr/lib64"
  fi

  echo "Environment configured for $ARCH Linux."
else
  echo "This script is intended for Linux systems only."
fi


gcc $CPPFLAGS $LDFLAGS app/*.c -o /tmp/shell-target

exec /tmp/shell-target "$@"