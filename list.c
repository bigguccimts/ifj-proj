#include "list.h"

#include <stdio.h>
#include <stdlib.h>

void list_init(List *list)
{
    list->firstElement = NULL;
    list->activeElement = NULL;
}

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

void list_first(List *list)
{
    list->activeElement = list->firstElement;
}

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
        ef = INTERNAL_ERR;
    }
}

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

int list_is_active(List *list)
{
    return list->activeElement != NULL;
}

Data_type list_ret_type(List *list)
{
    return list->activeElement->type;
}
