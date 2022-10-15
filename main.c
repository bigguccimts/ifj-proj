/*
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

#include "hashtable.h"
#include "returncodes.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * @brief Main function of the compiler
 * @return 0 if no errors occured
 */
int main()
{
    Symtab tab;
    symt_init(tab);
    tab_item_data data, ret;
    data.type = BOOL_T;
    symt_add(tab, "Peter", data);
    printf("%d\n", murmurhash("Peter", (uint32_t)strlen("Peter"), 0));
    ret = symt_find(tab, "Peter");
    printf("%d\n", ret.type);

    return ALL_GOOD;
}
