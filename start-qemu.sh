#!/bin/bash -e
set -o pipefail

# From https://stackoverflow.com/a/4774063
WORKSPACE_PATH="$( cd "$(dirname "$0")" > /dev/null 2>&1; pwd -P )"
if [[ $WORKSPACE_PATH = '' ]]; then
  exit 1
fi

# Indentation has been applied relative to working directory depth

pushd "$WORKSPACE_PATH" &> /dev/null

  qemu-system-x86_64 -kernel build/linux/arch/x86_64/boot/bzImage \
    -initrd build/initramfs/initramfs.cpio.gz -nographic \
    -append "console=ttyS0"

popd &> /dev/null # $WORKSPACE_PATH
