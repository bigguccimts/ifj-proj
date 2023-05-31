/**
 * Implemenace překladače imperativního jazyka IFJ22
 *
 * @file stack.h
 * @brief File contains definitions of structs and functions for stack of symbol tables
 *
 * @author Gabriela Paganíková (xpagan00)
 */

#ifndef STACK_H
#define STACK_H

#include "symtable.h"
#include "precedence.h"
#include "scanner.h"
#include <stdbool.h>

/**
 * @brief Struct containing end states of the specific scope
 *  and pointer to the next item in the stack
 *
 */
typedef struct stack_item
{
    TOKEN token;
    int prec_val;
    struct stack_item *next;
} *stack_item_ptr;

/**
 * @brief Struct containing pointers to the top and bottom items of the stack
 *
 */
typedef struct
{
    stack_item_ptr top;
    // stack_item_bottom_ptr bot;
} Precstack;

void stack_init(Precstack *, int *);

void stack_push(Precstack *, TOKEN, int, int *);

void stack_push_dollar(Precstack *);

bool stack_pop_out(Precstack *);

stack_item_ptr stack_peek(Precstack *);

void stack_free(Precstack *);

void print_stack(Precstack *stack);
#endif // STACK_H