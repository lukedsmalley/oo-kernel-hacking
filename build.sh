#!/bin/bash -e
set -o pipefail

# "$@" IS USED LATER TO ACCEPT ARGUMENTS FOR MAKE

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

  pushd_silently build

    pushd_silently linux
      make "$@"
      ../../treetool snapshot . ../../tree-snapshots/linux-built.tree
    popd_silently linux

    pushd_silently busybox
      make "$@"
      ../../treetool snapshot . ../../tree-snapshots/busybox-built.tree
      make install
      ../../treetool snapshot . ../../tree-snapshots/busybox-installed.tree
    popd_silently busybox

    mkdir -p initramfs/root
    pushd_silently initramfs/root
      mkdir -p bin sbin etc proc sys usr/bin usr/sbin
      cp -a ../../busybox/_install/* .
      cp -r ../../../src/initramfs/* .
      find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
    popd_silently initramfs/root
    
  popd_silently build

popd_silently "$WORKSPACE_PATH"
