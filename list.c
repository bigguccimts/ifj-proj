#include "list.h"

#include <stdio.h>
#include <stdlib.h>
/**
 * @brief initialize new list
 * 
 * @param list 
 */
void list_init(List *list)
{
    list->firstElement = NULL;
    list->activeElement = NULL;
}
/**
 * @brief frees space after list
 * 
 * @param list 
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
 * @brief Sets activity on first element in list
 * 
 * @param list 
 */
void list_first(List *list)
{
    list->activeElement = list->firstElement;
}
/**
 * @brief inserts new element in list
 * 
 * @param list 
 * @param data 
 * @param ef 
 */
void list_insert(List *list, Data_type data, int *ef)
{
    struct ListElement *tmpElement = malloc(sizeof(struct ListElement));
    if (tmpElement)
    {
        if (!list->firstElement)
        {
            // fprintf(stderr, "First");
            tmpElement->type = data;
            tmpElement->nextElement = NULL;
            list->firstElement = tmpElement;
        }
        else
        {
            // fprintf(stderr, "Sec");
            list_first(list);
            while (list->activeElement->nextElement != NULL)
            {
                list_next(list);
            }
            tmpElement->type = data;
            tmpElement->nextElement = NULL;
            list->activeElement->nextElement = tmpElement;
            list_first(list);
        }
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}
/**
 * @brief sets activity on next element in a row.
 * 
 * @param list 
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
 * @brief Checks if active elemnt is set.
 * 
 * @param list 
 * @return int 
 */
int list_is_active(List *list)
{
    return list->activeElement != NULL;
}
/**
 * @brief Returns data from list element.
 * 
 * @param list 
 * @return Data_type 
 */
Data_type list_ret_type(List *list)
{
    return list->activeElement->type;
}
