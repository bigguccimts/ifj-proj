/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file symtable.c
 * @brief File containing the implementation of defined functions in the symtable.h file
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

/**
 * @brief Initializes the hash table of symbols
 *
 * @param table Pointer to the hash table
 */
void symt_init(Symtab table)
{
    // Checking if NULL was not passed as a pointer to the hash table
    if (table != NULL)
    {
        for (int i = 0; i < SYMTAB_MAX_SIZE; i++)
        {
            table[i] = NULL;
        }
    }
    else
    {
        fprintf(stderr, "NULL PASSED TO INIT\n"); // just for debug, will be changed later to proper error code
        return;
    }
}

/**
 * @brief Adds symbols to the hash table of symbols
 *
 * @param table Pointer to the hash table
 * @param id Index of the symbol
 * @param data Struct containing data of the symbol
 */
void symt_add(Symtab table, char *id, tab_item_data data)
{
    // Contains the hashed identificator
    uint32_t index = murmurhash(id, (uint32_t)strlen(id), 0);
    // Checking if NULL was not passed as a pointer to the hash table
    if (table != NULL)
    {
        // Allocating memory
        table[index] = (Symtab_item *)malloc(2 * sizeof(Symtab_item));
        // Checking if malloc failed
        if (table[index] == NULL)
        {
            fprintf(stderr, "ALLOC ERROR\n"); // just for debug, will be changed later to proper error code
            return;
        }
        // Writing data
        table[index]->data = data;
    }
    else
    {
        fprintf(stderr, "NULL PASSED TO ADD\n"); // just for debug, will be changed later to proper error code
        return;
    }
}

/**
 * @brief Finds the symbol in the hash table and returns its value
 *
 * @param table Pointer to the hash table
 * @param id Index of the symbol
 * @return tab_item_data Returns the struct cointaing data of the symbol
 */
tab_item_data symt_find(Symtab table, char *id)
{
    // Checking if NULL was not passed as a pointer to the hash table
    if (table != NULL)
    {
        // Contains the hashed identificator
        uint32_t index = murmurhash(id, (uint32_t)strlen(id), 0);
        return table[index]->data;
    }
    else
    {
        fprintf(stderr, "NULL PASSED TO FIND\n"); // just for debug, will be changed later to proper error code
        return;
    }
}
/**
 * @brief Frees the symbol hash table
 *
 * @param table Pointer to the hash table
 */
void symt_free(Symtab table)
{
    // Checking if NULL was not passed as a pointer to the hash table
    if (table != NULL)
    {
        for (int i = 0; i < SYMTAB_MAX_SIZE; i++)
        {
            free(table[i]);
        }
    }
    else
    {
        fprintf(stderr, "NULL PASSED TO FREE\n"); // just for debug, will be changed later to proper error code
        return;
    }
}
