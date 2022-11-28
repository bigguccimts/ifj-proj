/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file subor.c
 * @brief Header for data structure list.
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#ifndef LIST_H
#define LIST_H

#include "symtable.h"

typedef struct ListElement
{
    Data_type type;
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
void list_insert(List *, Data_type, int *);
void list_next(List *);
int list_is_active(List *);
Data_type list_ret_type(List *);
#endif // LIST_H
