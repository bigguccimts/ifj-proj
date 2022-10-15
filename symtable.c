/*
 * Implementae překladače imperativního jazyka IFJ22
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void symt_init(Symtab table)
{

    if (table != NULL)
    {
        for (int i = 0; i < SYMTAB_MAX_SIZE; i++)
        {
            table[i] = NULL;
        }
    }
    else
    {
        fprintf(stderr, "NULL PASSED TO INIT\n");
        return;
    }
}

void symt_add(Symtab table, char *id, tab_item_data data)
{
    uint32_t index = murmurhash(id, (uint32_t)strlen(id), 0);

    if (table != NULL)
    {
        table[index] = malloc(2 * sizeof(Symtab_item));

        if (table[index] == NULL)
        {
            fprintf(stderr, "ALLOC ERROR\n");
            return;
        }

        table[index]->data = data;
    }
    else
    {
        printf("NULL PASSED TO ADD\n");
        return;
    }
}

tab_item_data symt_find(Symtab table, char *id)
{
    uint32_t index = murmurhash(id, (uint32_t)strlen(id), 0);
    return table[index]->data;
}

void symt_free(Symtab table)
{
    for (int i = 0; i < SYMTAB_MAX_SIZE; i++)
    {
        free(table[i]);
    }
}
