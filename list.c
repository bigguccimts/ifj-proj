/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file list.c
 * @brief File containing the implementation of defined functions in the list.h file
 *
 * @author Dušan Slúka (xsluka00)
 */

#include "list.h"
#include "returncodes.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initializes the list of parameters/arguments
 *
 * @param list Pointer to the list
 */
void list_init(List *list)
{
    list->firstElement = NULL;
    list->activeElement = NULL;
}

/**
 * @brief Disposes of the list
 *
 * @param list Pointer to the list
 */
void list_dispose(List *list)
{
    struct ListElement *tmpElement;
    while (list->firstElement != NULL)
    {
        tmpElement = list->firstElement;
        if (list->firstElement->nextElement != NULL)
        {
            list->firstElement = tmpElement->nextElement;
        }
        else
        {
            list->firstElement = NULL;
        }
        free(tmpElement);
    }

    list->activeElement = NULL;
    list->firstElement = NULL;
}

/**
 * @brief Sets activity to the first element of the list
 *
 * @param list Pointer to the list
 */
void list_first(List *list)
{
    list->activeElement = list->firstElement;
}

/**
 * @brief Inserts new element to the list after the last element
 *
 * @param list Pointer to the list
 * @param data Struct containing data of the element
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void list_insert(List *list, params_data data, int *ef)
{
    struct ListElement *tmpElement = malloc(sizeof(struct ListElement));
    if (tmpElement)
    {
        if (!list->firstElement)
        {
            tmpElement->data = data;
            tmpElement->nextElement = NULL;
            list->firstElement = tmpElement;
        }
        else
        {
            tmpElement->data = data;
            tmpElement->nextElement = NULL;
            ListElementPtr tmp_rot = list->firstElement;
            while (tmp_rot->nextElement != NULL)
            {
                tmp_rot = tmp_rot->nextElement;
            }
            tmp_rot->nextElement = tmpElement;
        }
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}

/**
 * @brief Removes the first item
 *
 * @param list Pointer to the list
 */
void list_remove_first(List *list)
{

    if (list->firstElement->nextElement)
    {
        ListElementPtr tmp = list->firstElement;
        list->firstElement = list->firstElement->nextElement;
        free(tmp);
    }
    else
    {
        list_init(list);
    }
}

/**
 * @brief Sets activity to the next element in the list
 *
 * @param list Pointer to the list
 */
void list_next(List *list)
{
    if (list->activeElement != NULL)
    {
        if (list->activeElement->nextElement == NULL)
        {
            list->activeElement = NULL;
        }
        else
        {
            list->activeElement = list->activeElement->nextElement;
        }
    }
}

/**
 * @brief Checks the activity of the list
 *
 * @param list Pointer to the list
 * @return int Returns 1 if the list is active
 */
int list_is_active(List *list)
{
    return list->activeElement != NULL;
}

/**
 * @brief Returns data struct of the active element
 *
 * @param list Pointer to the list
 * @return params_data Data struct of active element
 */
params_data list_ret_data(List *list)
{
    return list->activeElement->data;
}