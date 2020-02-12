#!/bin/bash -e
set -o pipefail

# "$@" IS USED LATER TO ACCEPT ARGUMENTS FOR MAKE

# Silent pushd
qpushd () {
  pushd "$1" &> /dev/null
}

# Silent popd
qpopd () {
  popd &> /dev/null
}

# From https://stackoverflow.com/a/4774063
WORKSPACE_PATH="$( cd "$(dirname "$0")" > /dev/null 2>&1; pwd -P )"
if [[ $WORKSPACE_PATH = '' ]]; then
  exit 1
fi

# Indentation has been applied relative to working directory depth

qpushd "$WORKSPACE_PATH"

  qpushd build

    qpushd linux
      make "$@"
      ../../treetool snapshot . ../../tree-snapshots/linux-built.tree
    qpopd linux

    qpushd busybox
      make "$@"
      ../../treetool snapshot . ../../tree-snapshots/busybox-built.tree
      make install
      ../../treetool snapshot . ../../tree-snapshots/busybox-installed.tree
    qpopd busybox

    mkdir -p initramfs/root
    qpushd initramfs/root
      mkdir -p bin sbin etc proc sys usr/bin usr/sbin
      cp -a ../../busybox/_install/* .
      cp -r ../../../src/initramfs/* .
      find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
    qpopd initramfs/root
    
  qpopd build

qpopd "$WORKSPACE_PATH"
