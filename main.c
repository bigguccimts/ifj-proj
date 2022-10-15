/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file main.c
 * @brief File containing main function of the compiler
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#include "symtable.h"
#include "returncodes.h"
#include <stdio.h>
#include <stdlib.h>
/*
 * @brief Main function of the compiler
 * @return 0 if no errors occured
 */
int main()
{
    Symtab table;
    symt_init(table);
    if (symt_init(NULL) == INTERNAL_ERR)
        fprintf(stderr, "%d\n", INTERNAL_ERR);
    tab_item_data data;
    tab_item_data ret;
    data.type = BOOL_T;
    symt_add(table, "foo", data);
    symt_find(table, "foo", &ret);
    printf("%d\n", ret.type);
    symt_free(table);

    return ALL_GOOD;
}
