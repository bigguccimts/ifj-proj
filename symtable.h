/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file symtable.h
 * @brief File containing definitions of structs and functions for the symbol hash table
 *
 * @author Matúš Ďurica (xduric06)
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "murmurhash.h"
#include "returncodes.h"
#include "list.h"
#include "datatypes.h"
#include <stdbool.h>
#include <stdint.h>

#define SYMTAB_MAX_SIZE 256 // Max size of the symbol table - keep it in 2^n format!

#define INIT_SYMT_DATA(data) \
    data.params.firstElement = NULL;

/**
 * @brief Struct containg data of 1 item in the symbol table
 *
 */
typedef struct
{
    Data_type type; // Data type of variable/function
    float num_val;  // Numerical value of variable
    char *str_val;  // String value of variable
    bool def;       // Variable/function has been defined
    List params;    // List containing params of functions
} tab_item_data;

/**
 * @brief Item of hash table
 *
 */
typedef struct tab_item
{
    char *identifier;           // Identifier of variable/function
    tab_item_data data;         // Data struct containing data of the item
    struct tab_item *next_item; // Pointer to collision
} *Symtab_item;

typedef Symtab_item Symtab[SYMTAB_MAX_SIZE];

void symt_init(Symtab, int *);

void symt_add(Symtab, char *, tab_item_data, int *);

bool symt_find(Symtab, char *, tab_item_data *, int *);

void symt_free(Symtab, int *);

void symt_add_list(tab_item_data *, List, int *);

#endif // HASHTABLE_H