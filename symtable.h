/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "murmurhash.h"
#include <stdbool.h>
#include <stdint.h>

#define SYMTAB_MAX_SIZE 256

typedef enum
{
    INT_T,
    FLOAT_T,
    STRING_T,
    BOOL_T
} Data_type;

typedef struct
{
    Data_type type; // type of var/func
    bool def;       // var/func has been defined
    bool global;    // global variable
} tab_item_data;

typedef struct tab_item
{
    char *identifier;   // identifier of var/func
    tab_item_data data; // data struct
    // struct tab_item *next_item; // pointer to next item in the hash table, probably useless
} * Symtab_item;

typedef Symtab_item Symtab[SYMTAB_MAX_SIZE];

void symt_init(Symtab);

void symt_add(Symtab, char *, tab_item_data);

tab_item_data symt_find(Symtab, char *);

// bool symt_remove(Symtab, char *);

void symt_free(Symtab);

#endif // HASHTABLE_H
