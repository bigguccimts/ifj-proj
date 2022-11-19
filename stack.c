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
 * @retval INTERNAL_ERR if error occurred
 */
void stack_push(Symstack *stack, Symtab *table, int *ef)
{
    if (stack != NULL && table != NULL)
    {
        stack_item_ptr tmp = malloc(sizeof(struct stack_item));
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
    else
    {
        *ef = INTERNAL_ERR;
    }
}
/**
 * @brief Peeks into the top item and returns its hash table
 *
 * @param stack Pointer to the stack
 * @param retptr Pointer to the variable where returned hash table pointer will be stored
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void stack_peek(Symstack *stack, stack_item_ptr *retptr, int *ef)
{
    if (stack != NULL)
    {
        *retptr = stack->top;
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}
/**
 * @brief Pops the top item and moves it to auxiliary stack
 *
 * @param source Pointer to the source stack
 * @param destination Pointer to the destination stack
 * @param retptr Pointer to the variable where returned hash table pointer will be
 * @param aux Specifies, whether the source stack is the main one or the aux one.TRUE == aux FALSE == main
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void stack_pop(Symstack *source, Symstack *destination, bool aux, int *ef)
{
    if (source != NULL)
    {
        if (aux == true)
        {
            stack_push(destination, source->top->tab, ef);
            stack_item_ptr tmp = source->top;
            source->top = source->top->next;
            free(tmp);
        }
        else
        {
            if (source->top != source->bot)
            {
                stack_push(destination, source->top->tab, ef);
                stack_item_ptr tmp = source->top;
                source->top = source->top->next;
                free(tmp);
            }
            else
            {
                // printf("debug\n");
                *ef = INTERNAL_ERR;
            }
        }
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

 * @retval INTERNAL_ERR if error occurred
 */
void stack_free(Symstack *stack, int *ef)
{
    if (stack->top != NULL)
    {
        stack_item_ptr tmp = NULL;
        while (true)
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
