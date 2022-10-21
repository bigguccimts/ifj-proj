/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file stack.c
 * @brief File containing the implementation of defined functions in the stack.h file
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#include "returncodes.h"
#include "stack.h"
#include "symtable.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/**
 * @brief Initializes the stack of hash tables
 *
 * @param stack Pointer to the stack
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void stack_init(Symstack *stack, int *ef)
{
    if (stack != NULL)
    {
        stack->bot = NULL;
        stack->top = NULL;
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}
/**
 * @brief Pushes hash table of symbols into the stack
 *
 * @param stack Pointer to the stack
 * @param table Pointer to the hash table of symbols
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if no error occurred
 */
void stack_push(Symstack *stack, Symtab *table, int *ef)
{
    if (stack == NULL || table == NULL)
    {
        *ef = INTERNAL_ERR;
    }

    stack_item_ptr tmp;
    tmp = malloc(sizeof(struct stack_item));
    if (tmp == NULL)
    {
        *ef = INTERNAL_ERR;
    }
    if (stack->bot == NULL && stack->top == NULL)
    {
        tmp->tab = table;
        tmp->next = NULL;
        stack->top = tmp;
        stack->bot = stack->top;
    }
    else
    {
        tmp->tab = table;
        tmp->next = stack->top;
        stack->top = tmp;
    }
}

/**
 * @brief Peeks into the top item and returns its hash table
 *
 * @param stack Pointer to the stack
 * @param retptr Pointer to the variable where returned hash table pointer will be stored
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if no error occurred
 */
void stack_peek(Symstack *stack, stack_item_ptr *retptr, int *ef)
{
    if (stack == NULL)
    {
        *ef = INTERNAL_ERR;
    }
    *retptr = stack->top;
}

/**
 * @brief Adds data to the top item of the table
 *
 * @param stack Pointer to the stack
 * @param id Index of the symbol
 * @param data Struct containing data of the symbol
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if no error occurred
 */
void stack_add_data_top(Symstack *stack, char *id, tab_item_data data, int *ef)
{
    stack_item_ptr rettab = NULL;
    stack_peek(stack, &rettab, ef);
    symt_add(*rettab->tab, id, data, ef);
}

/**
 * @brief Pops the top item and returns its hash table
 *
 * @param stack Pointer to the stack
 * @param retptr Pointer to the variable where returned hash table pointer will be
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if no error occurred
 */
void stack_pop(Symstack *stack, stack_item_ptr *retptr, int *ef)
{
    if (stack == NULL)
    {
        *ef = INTERNAL_ERR;
    }

    if (stack->top != stack->bot)
    {
        stack_item_ptr tmp;
        tmp = stack->top;
        stack->top = stack->top->next;
        *retptr = tmp;
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}

/**
 * @brief Frees the last item representing local scope.
 * Only used once in the whole code!
 *
 * @param stack Pointer to the stack
 * @param ef Pointer to the error flag variable
 *

 * @retval INTERNAL_ERR if no error occurred
 */
void stack_free(Symstack *stack, int *ef)
{
    if (stack != NULL)
    {
        stack_item_ptr tmp = NULL;
        while (1)
        {
            tmp = stack->top;
            if (stack->top == stack->bot)
            {
                symt_free(*tmp->tab, ef);
                free(tmp);
                break;
            }
            else
            {
                stack->top = stack->top->next;
                symt_free(*tmp->tab, ef);
                free(tmp);
            }
        }
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}

void stack_free_after_pop(Symtab **tab, int *ef)
{
    if (*tab != NULL)
    {
        free(tab);
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}