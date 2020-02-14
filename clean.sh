#!/bin/bash -e
set -o pipefail

# From https://stackoverflow.com/a/4774063
WORKSPACE_PATH="$( cd "$(dirname "$0")" > /dev/null 2>&1; pwd -P )"
if [[ $WORKSPACE_PATH = '' ]]; then
  exit 1
fi

# Indentation has been applied relative to working directory depth

pushd "$WORKSPACE_PATH" &> /dev/null
  rm -rf build
popd &> /dev/null # $WORKSPACE_PATH
