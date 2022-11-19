/**
 * Implemenace překladače imperativního jazyka IFJ22
 *
 * @file stack.h
 * @brief File contains definitions of structs and functions for stack of symbol tables
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#ifndef STACK_H
#define STACK_H

#include "symtable.h"
#include <stdbool.h>

/**
 * @brief Struct containing pointer to the hash table of the specific scope
 *  and pointer to the next item in the stack
 *
 */
typedef struct stack_item
{
    Symtab *tab;
    struct stack_item *next;
} * stack_item_ptr;

/**
 * @brief Struct containing pointers to the top and bottom items of the stack
 *
 */
typedef struct
{
    stack_item_ptr top;
    stack_item_ptr bot;

} Symstack;

void stack_init(Symstack *, int *);

void stack_push(Symstack *, Symtab *, int *);

void stack_pop(Symstack *, Symstack *, bool, int *);

void stack_peek(Symstack *, stack_item_ptr *, int *);

void stack_free(Symstack *, int *);

#endif // STACK_H
