#include <stdio.h>

int main() {
  printf("0x%x\n", (unsigned char)(-4));
}


// Skipped arglist, gt/lt and equal ops, break, callvirt, calli, castclass,
// push if ops, ckfinite, local memory ops, exception ops, jmp, no { checks },
// switch, throw, rethrow
// #define I_ADD_UNCHECKED 0
// #define I_ADD_SIGNED 1
// #define I_ADD_UNSIGNED 2
// #define I_AND 3
// #define I_BRANCH_IF_EQUAL 4
// #define I_BRANCH_IF_GREATER 5
// #define I_BRANCH_IF_LESSER 6
// #define I_BRANCH_IF_UNSIGNED_GREATER 7
// #define I_BRANCH_IF_UNSIGNED_LESSER 8
// #define I_BOX
// #define I_BRANCH
// #define I_CALL
// Cast semantics indicate that stack items have their data type included
// Constrain and castclass ops also indicate this
// #define I_CAST
// #define I_DIVIDE
// #define I_DIVIDE_UNSIGNED
// #define I_LOAD_DUPLICATE
// #define I_INSTANTIATE
// #define I_LOAD_ARGUMENT
// There are so many loads that I have to come back to these
// #define I_LOAD_VALUE
// #define I_LOAD_FIELD
// #define I_LOAD_FIELD_REFERENCE
// #define I_LOAD_ELEMENT
// #define I_MULTIPLY_UNCHECKED
// #define I_MULTIPLY_SIGNED
// #define I_MULTIPLY_SIGNED
// #define I_NEGATE
// #define I_ALLOCATE_ARRAY
// #define I_NO_OPERATION 0
// #define I_OR
// #define I_POP
// #define I_RETURN
// #define I_SHIFT_LEFT
// #define I_SHIFT_RIGHT_SIGNED
// #define I_SHIFT_RIGHT_UNSIGNED
// #define I_LOAD_SIZE_OF
// // Again, so many stores that it needs revisited
// #define I_STORE_ARG
// #define I_STORE_ELEMENT
// #define I_STORE_FIELD
// #define I_SUBTRACT_UNCHECKED
// #define I_SUBTRACT_SIGNED
// #define I_SUBTRACT_UNSIGNED
// #define I_TAIL
// #define I_UNBOX
// #define I_EXCLUSIVE_OR

