/*
 *myset.c- This program acts as an interactive "pocket calculator" for handling set operations.
 * It manages 6 predefined sets (SETA, SETB, SETC, SETD, SETE, SETF), each capable of storing
 * integers in the range [0..127].
 * The program reads commands from standard input (stdin), allowing both interactive keyboard input
 * and file redirection. It parses each command line, validates the input format, detects errors
 * (such as missing commas, invalid set names, extraneous text, etc.), and invokes the appropriate
 * logical operations.
 * Responsibilities:
 * - Managing the main loop and user interaction.
 * - Parsing commands and arguments.
 * - Handling all Input/Output operations (read_set, print_set, and error messages).
 * - calling the mathematical functions located in set.c.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "set.h"

/*Helper Function Prototypes-
Skips spaces and tabs in a string */
void skip_spaces(char **str);
/* Finds a set pointer by its name */
set* get_set_by_name(const char *name, set_entry sets_map[]);
/*Analyzes the command line and calls relevant functions */
void handle_command(char *line, set_entry sets_map[]);
/*Handles the 'read_set' command parsing */
void handle_read_set(char *ptr, set_entry sets_map[]);
/*Handles the 'print_set' command parsing */
void handle_print_set(char *ptr, set_entry sets_map[]);
/* Handles commands that involve 3 sets (union, intersect, etc.) */
void handle_three_sets(char *ptr, int op, set_entry sets_map[]);
/* Prints the content of a set */
void print_set(const set *s);
/*Main Function*/
int main(void) {
    char line[MAX_LINE_LEN];
    int i;
    /*Define the sets locally in main*/
    set SETA, SETB, SETC, SETD, SETE, SETF;
    /*Map strings to variables for easy lookup*/
    set_entry sets_map[NUM_OF_SETS]; 
    sets_map[0].name = "SETA"; sets_map[0].ptr = &SETA;
    sets_map[1].name = "SETB"; sets_map[1].ptr = &SETB;
    sets_map[2].name = "SETC"; sets_map[2].ptr = &SETC;
    sets_map[3].name = "SETD"; sets_map[3].ptr = &SETD;
    sets_map[4].name = "SETE"; sets_map[4].ptr = &SETE;
    sets_map[5].name = "SETF"; sets_map[5].ptr = &SETF;
    /*Initialize all sets to be empty*/
    for (i = 0; i < NUM_OF_SETS; i++) {
        clear_set(sets_map[i].ptr);
    }
    /*Main loop: read commands until EOF or stop*/
    while (1){
        printf("\nPlease enter a command\n");
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) {
            printf("The code should end with a stop command\n");
            break;
        }   
        /* Echo the command to output as required */
        printf("%s", line); 
        handle_command(line, sets_map);
    }  
    return OK;
}
/*Function Implementations-
 * Updates the set with new values.
 * Logic: Calculates the correct byte and bit index for each number
 * and turns on the bit using bitwise OR.
 * Input: Pointer to set, array of values, count of values.
 * Output: None.
 */
void read_set(set *s, const int values[], int count) {
    int i;
    int byte_index; 
    int bit_index;
    clear_set(s); /* Always start with a clean set */   
    for (i = 0; i < count; i++) {
        byte_index = values[i] / BITS_PER_BYTE;
        bit_index = values[i] % BITS_PER_BYTE;
        s->data[byte_index] |= (1 << bit_index);
    }
}
/*
 * Prints the numbers in the set.
 * Logic: Iterates over all bits. If a bit is 1, calculates the number and prints it.
 * formatted to print 16 numbers per line.
 * Input: Pointer to set.
 * Output: None (prints to stdout).
 */
void print_set(const set *s){
    int i, j, num;
    int count = 0;      
    int is_empty = TRUE;   
    for (i = 0; i < SET_ARRAY_SIZE; i++){
        for (j = 0; j < BITS_PER_BYTE; j++){
            if (s->data[i] & (1 << j)){ 
                num = i * BITS_PER_BYTE + j;
                /*Handle line break every 16 numbers */
                if (count > 0 && count % NUMS_PER_LINE == 0){
                    printf("\n");
                }
                printf("%d ", num);
                count++;
                is_empty = FALSE;
            }
        }
    }
    if (is_empty) {
        printf("The set is empty\n");
    }else{
        printf("\n");
    }
}
/*
 * Advances the pointer past any spaces or tabs.
 * Input: Pointer to the string pointer.
 * Output: None (modifies the pointer).
 */
void skip_spaces(char **str) {
    while (**str == ' ' || **str == '\t') {
        (*str)++;
    }
}
/*
 * Search for a set by its name string.
 * Input: Name to search, and the map array.
 * Output: Pointer to the set if found, NULL otherwise.
 */
set* get_set_by_name(const char *name, set_entry sets_map[]) {
    int i;
    for (i = 0; i < NUM_OF_SETS; i++){
        if (strcmp(name, sets_map[i].name) == 0) {
            return sets_map[i].ptr; 
        }
    }
    return NULL; 
}
/*
 * Main parser function.
 * Identifies the command name and calls the specific handler.
 * Input: The command line string, and the sets map.
 * Output: None.
 */
void handle_command(char *line, set_entry sets_map[]) {
    char *ptr = line;           
    char cmd[MAX_CMD_LEN];      
    int i = 0;
    skip_spaces(&ptr);
    /* Ignore empty lines */
    if (*ptr == '\0' || *ptr == '\n'){  
        return;
    }
    /* Extract command name */
    while (*ptr && !isspace((unsigned char)*ptr) && *ptr != ',') {
        if (i < MAX_CMD_LEN - 1) cmd[i++] = *ptr;
        ptr++;
    }
    cmd[i] = '\0';
    skip_spaces(&ptr);
    /* Commas are not allowed right after command name */
    if (*ptr == ','){
        printf("Illegal comma\n");
        return;
    }
    /* Dispatch to correct function */
    if(strcmp(cmd, "read_set") == 0) {
        handle_read_set(ptr, sets_map);
    }else if(strcmp(cmd, "print_set") == 0) {
        handle_print_set(ptr, sets_map);
    }else if(strcmp(cmd, "union_set") == 0) {
        handle_three_sets(ptr, OP_UNION, sets_map);
    }else if(strcmp(cmd, "intersect_set") == 0) {
        handle_three_sets(ptr, OP_INTERSECT, sets_map);
    }else if(strcmp(cmd, "sub_set") == 0) {
        handle_three_sets(ptr, OP_SUB, sets_map);
    }else if(strcmp(cmd, "symdiff_set") == 0) {
        handle_three_sets(ptr, OP_SYMDIFF, sets_map);
    }else if(strcmp(cmd, "stop") == 0) {
        /* Ensure no extra text after stop */
        if(*ptr != '\0' && *ptr != '\n') {
            printf("Extraneous text after end of command\n");
            return;
        }
        stop(); /* Calls the function from set.c */
    } else {
        printf("Undefined command name\n");
    }
}
/*
 * Parses arguments for read_set command.
 * Reads the set name, then a list of numbers terminated by -1.
 * Input: Pointer to arguments string, sets map.
 * Output: None.
 */
void handle_read_set(char *ptr, set_entry sets_map[]) {
    char name[MAX_SET_NAME_LEN];
    int i = 0;
   set temp_set;           
    int num, sign;
    int prev_was_comma = FALSE;    
    set *s;   
    skip_spaces(&ptr);
    if (*ptr == '\0' || *ptr =='\n') {
        printf("Missing parameter\n");
        return;
    }
    /*Get set name*/
    while (*ptr && !isspace((unsigned char)*ptr) && *ptr != ',') {
        if (i < MAX_SET_NAME_LEN - 1) name[i++]= *ptr;
        ptr++;
    }
    name[i] = '\0';
    s = get_set_by_name(name, sets_map);
    if (s == NULL) {
        printf("Undefined set name\n");
        return;
    }
    skip_spaces(&ptr);
    /* Check for the initial comma after set name */
    if (*ptr != ','){
        if (*ptr =='\0' || *ptr =='\n') 
             printf("Missing parameter\n"); 
        else printf("Missing comma\n");
        return;
    }
    ptr++; 
    prev_was_comma = TRUE;
    /* Loop to parse numbers */
    while (1){
        skip_spaces(&ptr);
        if (*ptr =='\0' ||*ptr == '\n') {
            printf("List of set members is not terminated correctly\n");
            return;
        }
        /* Handle commas and check for double commas */
        if (*ptr == ','){ 
            if (prev_was_comma){
                printf("Multiple consecutive commas\n");
                return;
            }
            prev_was_comma = TRUE;
            ptr++;
            continue;
        }
        prev_was_comma = FALSE;
        sign = 1;/* Parse number sign */
        if (*ptr == '-') {
            sign = -1;
            ptr++;
        }
        /* Parse integer value */
        if (!isdigit((unsigned char)*ptr)){ 
            printf("Invalid set member - not an integer\n");
            return;
        }
        num = 0;
        while (isdigit((unsigned char)*ptr)) {
            num = num * 10 + (*ptr - '0');
            ptr++;
        }
        num *= sign;
        /*Validate text after number*/
        if (*ptr != '\0' && !isspace((unsigned char)*ptr) && *ptr != ',') {
            printf("Invalid set member - not an integer\n");
            return;
        }
        /* Check for list terminator */
        if (num == LIST_TERMINATOR) {
            skip_spaces(&ptr);
            if (*ptr != '\0' && *ptr != '\n') {
                printf("Extraneous text after end of command\n");
                return;
            }
            break; /*Successfully finished reading */
        }
        /* Validate range*/
        if (num < SET_MIN_VALUE || num > SET_MAX_VALUE) {
            printf("Invalid set member - value out of range\n");
            return;
        }
        /* Turn on the bit corresponding to 'num' */
        temp_set.data[num/BITS_PER_BYTE] |= (1<<(num%BITS_PER_BYTE));
    }
    *s = temp_set;/* Copy temp set to real set */
    clear_set(&temp_set); 
}
/*
 * Parses arguments for print_set command.
 * Gets the set name and calls print_set.
 * Input: Pointer to arguments string, sets map.
 * Output: None.
 */
void handle_print_set(char *ptr, set_entry sets_map[]) {
    char name[MAX_SET_NAME_LEN];
    int i = 0;
    set *s;
    skip_spaces(&ptr);
    if (*ptr == '\0'||*ptr == '\n') {
        printf("Missing parameter\n");
        return;
    }
    /* Extract set name */
    while (*ptr && !isspace((unsigned char)*ptr) && *ptr != ',') {
        if (i < MAX_SET_NAME_LEN - 1) name[i++] = *ptr;
        ptr++;
    }
    name[i] = '\0';
    s = get_set_by_name(name, sets_map);
    if (s == NULL) {
        printf("Undefined set name\n");
        return;
    }
    skip_spaces(&ptr);
    if (*ptr != '\0' && *ptr != '\n') {
        printf("Extraneous text after end of command\n");
        return;
    } 
    print_set(s);
}
/*
 * Parses arguments for 3-set operations (union, intersect, sub, symdiff).
 * Reads destination set, and two source sets.
 * Input: Pointer to arguments, operation code, sets map.
 * Output: None.
 */
void handle_three_sets(char *ptr, int op, set_entry sets_map[]) {
    char name[MAX_SET_NAME_LEN];
    int i;
    set *dest, *s1, *s2;
    skip_spaces(&ptr);
    /* 1. Get Destination Set */
    if (*ptr == '\0' || *ptr == '\n') {
        printf("Missing parameter\n");
        return;
    }
    i = 0;
    while (*ptr && !isspace((unsigned char)*ptr) && *ptr != ',') {
        if (i < MAX_SET_NAME_LEN - 1) name[i++] = *ptr;
        ptr++;
    }
    name[i] = '\0';
    s1 = get_set_by_name(name, sets_map);
    if (s1 == NULL) {
        printf("Undefined set name\n");
        return;
    }
    skip_spaces(&ptr);
    if (*ptr != ',') {
        printf("Missing comma\n");
        return;
    }
    ptr++;
    /*2. Get First Source Set */
    skip_spaces(&ptr);
    if (*ptr == ',') {
        printf("Multiple consecutive commas\n");
        return;
    }
    if (*ptr == '\0' || *ptr == '\n') {
        printf("Missing parameter\n");
        return;
    }
    i = 0;
    while (*ptr && !isspace((unsigned char)*ptr) && *ptr != ',') {
        if (i < MAX_SET_NAME_LEN - 1) name[i++] = *ptr;
        ptr++;
    }
    name[i] = '\0';
    s2 = get_set_by_name(name, sets_map);
    if (s2 == NULL) {
        printf("Undefined set name\n");
        return;
    }
    skip_spaces(&ptr);
    if (*ptr != ',') {
        if (*ptr == '\0' || *ptr == '\n') 
             printf("Missing parameter\n");
        else printf("Missing comma\n");
        return;
    }
    ptr++;
    /*3. Get Second Source Set */
    skip_spaces(&ptr);
    if (*ptr == ',') {
        printf("Multiple consecutive commas\n");
        return;
    }
    if (*ptr == '\0' || *ptr == '\n') {
        printf("Missing parameter\n");
        return;
    }
    i = 0;
    while (*ptr && !isspace((unsigned char)*ptr) && *ptr != ',') {
        if (i < MAX_SET_NAME_LEN - 1) name[i++] = *ptr;
        ptr++;
    }
    name[i] = '\0';
    dest = get_set_by_name(name, sets_map);
    if (dest == NULL) {
        printf("Undefined set name\n");
        return;
    }
    skip_spaces(&ptr);
    if (*ptr != '\0' && *ptr != '\n') {
        printf("Extraneous text after end of command\n");
        return;
    }
    /* Execute the requested operation*/
    switch (op) {
        case OP_UNION: 
            union_set(s1, s2, dest);break;
        case OP_INTERSECT: 
            intersect_set(s1, s2, dest); break;
        case OP_SUB: 
            sub_set(s1, s2, dest);break;
        case OP_SYMDIFF: 
            symdiff_set(s1, s2, dest);break;
    }
}
