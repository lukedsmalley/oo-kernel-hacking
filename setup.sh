#!/bin/bash -e
set -o pipefail

# From https://stackoverflow.com/a/4774063
WORKSPACE_PATH="$( cd "$(dirname "$0")" > /dev/null 2>&1; pwd -P )"
if [[ $WORKSPACE_PATH = '' ]]; then
  exit 1
fi

pushd "$WORKSPACE_PATH" &> /dev/null

if [[ -d build ]]; then
  echo 'The "build" directory already exists, so everything should already be configured.'
  echo 'Delete the directory to clone everything again and start over.'
  echo
  popd &> /dev/null # $WORKSPACE_PATH
  exit 1
fi

mkdir -p tree-snapshots super-src build/linux build/busybox
./treetool snapshot build/linux tree-snapshots/linux-unconfigured.tree
./treetool snapshot build/busybox tree-snapshots/busybox-unconfigured.tree

pushd super-src &> /dev/null

git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git linux
pushd linux &> /dev/null
git checkout v5.5.2
make O=../../build/linux allnoconfig
../../treetool snapshot ../../build/linux ../../tree-snapshots/linux-allnoconfig.tree
popd &> /dev/null # linux

git clone git://busybox.net/busybox.git busybox
pushd busybox &> /dev/null
git checkout 1_31_stable
make O=../../build/busybox defconfig
../../treetool snapshot ../../build/busybox ../../tree-snapshots/busybox-defconfig.tree
popd &> /dev/null # busybox

popd &> /dev/null # super-src

#sed -i 's///' /build/linux/.config
#sed -i 's///' /build/busybox/.config
#./treetool snapshot build/linux tree-snapshots/linux-configured.tree
#./treetool snapshot build/busybox tree-snapshots/busybox-configured.tree

popd &> /dev/null # $WORKSPACE_PATH
