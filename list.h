/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file list.h
 * @brief File containing definitions of structs and functions for the list of parameters/arguments
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#ifndef LIST_H
#define LIST_H

#include "datatypes.h"
#define INIT_PARAM_DATA(data) data.identifier = NULL;

/**
 * @brief Struct containg data of 1 item in the list of parameters/arguments
 *
 */
typedef struct
{
    Data_type type;   // Data type of variable/function
    float num_val;    // Numerical value of variable
    char *str_val;    // String value of variable
    char *identifier; // Identifier of variable/function
} params_data;

/**
 * @brief Item of list
 *
 */
typedef struct ListElement
{
    params_data data;
    struct ListElement *nextElement;
} * ListElementPtr;

typedef struct
{
    ListElementPtr firstElement;
    ListElementPtr activeElement;
} List;

void list_init(List *);

void list_dispose(List *);

void list_first(List *);

void list_insert(List *, params_data, int *);

void list_next(List *);

int list_is_active(List *);

params_data list_ret_data(List *);

#endif // LIST_H