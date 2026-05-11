/*
 *set.c-Module Description-
 *This module provides the logical implementation of set operations using a bitwise representation.
 * Sets are implemented as an array of bits (128 bits total), where each bit corresponds to an integer
 * in the range [0..127]. If the k-th bit is 1, the number k is in the set.
 * This file strictly handles the logic and data manipulation. It performs NO Input/Output operations,
 * ensuring a separation of concerns between logic and interface.
 * Implemented Functions:
 * - union_set, intersect_set, sub_set, symdiff_set: Perform bitwise operations to calculate results.
 * - clear_set: Resets a set's data.
 * - stop: Terminates the program.
 */
#include "set.h"
/*
 * Resets the set to be empty.
 * It loops over the array and sets all bytes to 0.
 * Input:Pointer to the set.
 * Output:None.
 */
void clear_set(set *s) {
    int i;
    for (i = 0; i < SET_ARRAY_SIZE; i++) {
        s->data[i] = 0;
    }
}
/*
 * Performs the Union operation.
 * Logic:Uses bitwise OR (|) to combine bits from both sets.
 * Input:Two source sets and one destination set.
 * Output:None.
 */
void union_set(const set *s1, const set *s2, set *dest) {
    int i;
    for (i = 0; i < SET_ARRAY_SIZE; i++) {
        dest->data[i] = s1->data[i] | s2->data[i];
    }
}
/*
 * Performs the Intersection operation.
 * Logic:Uses bitwise AND (&) to keep only bits that are 1 in both sets.
 * Input:-2 source sets and one destination set.
 * Output:None.
 */
void intersect_set(const set *s1, const set *s2, set *dest) {
    int i;
    for (i = 0; i < SET_ARRAY_SIZE; i++) {
        dest->data[i] = s1->data[i] & s2->data[i];
    }
}
/*
 * Performs the Subtraction operation (s1 - s2).
 * Logic-Uses bitwise AND with NOT (& ~) to keep bits from s1 that are 0 in s2.
 * Input:Two source sets and one destination set.
 * Output-None.
 */
void sub_set(const set *s1, const set *s2, set *dest) {
    int i;
    for (i = 0; i < SET_ARRAY_SIZE; i++) {
        dest->data[i] = s1->data[i] & ~(s2->data[i]);
    }
}
/*
 * Performs the Symmetric Difference operation.
 * Logic-Uses bitwise XOR (^) to keep bits that are different between the sets.
 * Input-Two source sets and one destination set.
 * Output-None.
 */
void symdiff_set(const set *s1, const set *s2, set *dest) {
    int i;
    for (i = 0; i < SET_ARRAY_SIZE; i++) {
        dest->data[i] = s1->data[i] ^ s2->data[i];
    }
}
/*
 * Terminates the program execution.
 * Input:None.
 * Output:None.
 */
void stop(void) {
    exit(0);
}
