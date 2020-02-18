#!/bin/bash -e
set -o pipefail

pushd_silently () {
  pushd "$1" &> /dev/null
}

popd_silently () {
  popd &> /dev/null
}

# From https://stackoverflow.com/a/4774063
WORKSPACE_PATH="$( cd "$(dirname "$0")" > /dev/null 2>&1; pwd -P )"
if [[ $WORKSPACE_PATH = '' ]]; then
  exit 1
fi

# Indentation has been applied relative to working directory depth

pushd_silently "$WORKSPACE_PATH"

  if [[ -d build ]]; then
    echo 'The "build" directory already exists, so everything should'
    echo 'already be configured. Delete it to reconfigure everything'
    echo 'again and start over.'
    echo
    popd_silently "$WORKSPACE_PATH"
    exit 1
  fi

  mkdir -p tree-snapshots build/linux build/busybox
  ./treetool snapshot build/linux tree-snapshots/linux-unconfigured.tree
  ./treetool snapshot build/busybox tree-snapshots/busybox-unconfigured.tree

  pushd_silently src

    pushd_silently linux
      make O=../../build/linux allnoconfig
      cp .config-projectdefault ../../build/linux/.config
      ../../treetool snapshot ../../build/linux ../../tree-snapshots/linux-configured.tree
    popd_silently linux

    pushd_silently busybox &> /dev/null
      make O=../../build/busybox defconfig
      cp .config-projectdefault ../../build/busybox/.config
      ../../treetool snapshot ../../build/busybox ../../tree-snapshots/busybox-configured.tree
    popd_silently busybox

  popd_silently src

popd_silently "$WORKSPACE_PATH"
