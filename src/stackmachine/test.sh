#!/bin/bash -e

mkdir -p ../../build/stackmachine

# Ignore warnings unless the build fails
if ! gcc -o ../../build/stackmachine/test test/main.c &> /dev/null; then
  gcc -o ../../build/stackmachine/test test/main.c || true
  echo
  echo "Build failed; no tests were run."
  exit 1
fi

# $1 <- Test name
runTest () {
  printf "$1"
  set +e
  REASON=`../../build/stackmachine/test $1`
  CODE=$?
  set -e
  if [[ $CODE -eq 0 ]]; then
    printf ": \033[0;32mPassed\033[0m\n"
  else
    printf ": \033[0;31mFailed\033[0m\n"
    echo "  Exited with code $CODE."
    if [[ $REASON != '' ]]; then
      for LINE in $REASON; do
        echo "  $LINE"
      done
    else
      echo "  Probably a segmentation fault."
    fi
  fi
}

IFS=`printf '\n'`
for TEST in `grep '  useTest' test/main.c | sed -E 's/ *useTest\((.+)\);/\1/'`; do
  runTest $TEST
done
