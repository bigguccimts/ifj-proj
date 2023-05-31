/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file stack.c
 * @brief File containing the implementation of defined functions in the stack.h file
 *
 * @author Gabriela Paganíková (xpagan00)
 */

#include "returncodes.h"
#include "precedence.h"
#include "stack.h"
#include "symtable.h"
#include "scanner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

/**
 * @brief Initializes the stack of hash tables
 *
 * @param stack Pointer to the stack
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void stack_init(Precstack *stack, int *ef)
{
    if (stack != NULL)
    {
        stack->top = NULL;
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}

void stack_push_dollar(Precstack *stack)
{
    stack_item_ptr new = malloc(sizeof(struct stack_item));
    if (new)
    {
        new->prec_val = _DOLLAR;
        new->next = NULL;
        stack->top = new;
    }
}
/**
 * @brief pushes new items on the top of the stack
 *
 * @param stack pointer to the stack
 * @param token item from end states
 * @param ef pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void stack_push(Precstack *stack, TOKEN token, int val, int *ef)
{
    stack_item_ptr new = (stack_item_ptr)malloc(sizeof(struct stack_item));
    if (new != NULL)
    {
        new->token = token;
        new->prec_val = val;
        new->next = stack->top;
        stack->top = new;
        // printf("NEW TOP %d NEXT %d\n", stack->top->prec_val, stack->top->next->prec_val);
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}
/**
 * @brief peeks into the top item and returns its pointer
 *
 * @param stack pointer to the stack
 * @param ret_token pointer to the variable of returned stored token
 * @param ef pointer to the error flag variable
 */
stack_item_ptr stack_peek(Precstack *stack)
{
    return stack->top;
}

/**
 * @brief Pops top item from the stack and frees its memory
 *
 * @param stack Pointer to the stack
 * @return true When the top item was popped
 * @return false If stack is empty
 */
bool stack_pop_out(Precstack *stack)
{
    struct stack_item *tmp;
    if (stack->top != NULL)
    {
        tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
        return true;
    }
    return false;
}

/**
 * @brief Frees the stack
 *
 * @param stack pointer to the stack
 *
 */
void stack_free(Precstack *stack)
{
    while (stack_pop_out(stack))
        ;
}

// For debugging purposes only
void print_stack(Precstack *stack)
{
    stack_item_ptr tmp = stack->top;
    if (tmp)
    {
        printf("ITEMS ON STACK: ");
        while (tmp->next != NULL)
        {
            printf("%d ", tmp->prec_val);
            tmp = tmp->next;
        }
        printf("%d\n", tmp->prec_val);
    }
}
