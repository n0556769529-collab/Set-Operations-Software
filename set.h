/*
 *set.h-Header Description:
 * This header file defines the 'set' data structure and the prototypes for the set operations.
 * It includes the necessary libraries and defines constants used throughout the program,
 * such as the set size (128 bits) and return codes.
 * This file serves as the interface between the main program (myset.c) and the set logic (set.c).
 */
#ifndef SET_H
#define SET_H
#include <stdio.h>
#include <stdlib.h>
/*Constants definition */
#define SET_ARRAY_SIZE 16 /* Size of the array in bytes (128 bits total) */
#define BITS_PER_BYTE 8   
#define SET_MIN_VALUE 0   
#define SET_MAX_VALUE 127 
#define LIST_TERMINATOR -1 /* The number that ends the list in read_set */
/*General Constants */
#define MAX_LINE_LEN 1000        
#define MAX_CMD_LEN 20           
#define MAX_SET_NAME_LEN 10      
#define NUM_OF_SETS 6            
#define NUMS_PER_LINE 16 /* For print_set formatting */
/*Return codes and boolean values */
#define OK 0
#define TRUE 1
#define FALSE 0
/*Operation codes for handle_three_sets function */
#define OP_UNION 1
#define OP_INTERSECT 2
#define OP_SUB 3
#define OP_SYMDIFF 4

/*Structure definition */
typedef struct {
    unsigned char data[SET_ARRAY_SIZE]; /* Array of bits to hold the set data */
}set;
/*Helper struct to map set names (strings) to set variables */
typedef struct {
    char *name;
    set *ptr;
} set_entry;
/*Function Prototypes*/
/*
 * Resets a set to be empty.
 * Input: pointer to the set to clear.
 * Output: None.
 */
void clear_set(set *s);
/*
 * Calculates the union of two sets (elements in s1 OR s2).
 * Input: pointers to two source sets (s1, s2) and a pointer to the destination set.
 * Output: None (result is stored in dest).
 */
void union_set(const set *s1, const set *s2, set *dest);
/*
 * Calculates the intersection of two sets (elements in s1 AND s2).
 * Input: pointers to two source sets (s1, s2) and a pointer to the destination set.
 * Output: None (result is stored in dest).
 */
void intersect_set(const set *s1, const set *s2, set *dest);
/*
 * Calculates the difference between sets (elements in s1 BUT NOT in s2).
 * Input: pointers to two source sets (s1, s2) and a pointer to the destination set.
 * Output: None (result is stored in dest).
 */
void sub_set(const set *s1, const set *s2, set *dest);
/*
 * Calculates the symmetric difference (elements in s1 OR s2, BUT NOT both).
 * Input: pointers to two source sets (s1, s2) and a pointer to the destination set.
 * Output: None (result is stored in dest).
 */
void symdiff_set(const set *s1, const set *s2, set *dest);
/*
 * Terminates the program execution.
 * Input:None.
 * Output:None(Exits the program).
 */
void stop(void);
#endif
