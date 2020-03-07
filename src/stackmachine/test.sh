#!/bin/bash -e

mkdir -p ../../build/stackmachine

# Ignore warnings unless the build fails
sed -E "s/return [a-zA-Z0-9_()]+;/return 1;/" test/main.c > test/main.generated.c
if ! gcc -o ../../build/stackmachine/test test/main.generated.c &> /dev/null; then
  gcc -o ../../build/stackmachine/test test/main.generated.c || true
  echo
  echo "Build failed; no tests were run."
  exit 1
fi

# $1 <- Test name
runTest () {
  printf "$1"
  sed -E "s/return [a-zA-Z0-9_()]+;/return $1();/" test/main.c > test/main.generated.c
  if ! gcc -o ../../build/stackmachine/test test/main.generated.c &> /dev/null; then
    REASON='Invocation via test script failed.'
    CODE=1
  else
    set +e
    REASON=`../../build/stackmachine/test $1`
    CODE=$?
    set -e
  fi
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

IFS=$'\n'
for TEST_FILE in test/*; do
  for TEST in `grep -E '^int [a-zA-Z_]+\(\).*\{.*$' $TEST_FILE | sed -E 's/^int ([a-zA-Z_]+)\(\).*\{.*$/\1/'`; do
    runTest $TEST
  done
done

rm test/main.generated.c
