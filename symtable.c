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
#include "returncodes.h"

/**
 * @brief Initializes the hash table of symbols
 *
 * @param table Pointer to the hash table
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void symt_init(Symtab table, int *ef)
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
        *ef = INTERNAL_ERR;
    }
}

/**
 * @brief Adds symbols to the hash table of symbols
 *
 * @param table Pointer to the hash table
 * @param id Index of the symbol
 * @param data Struct containing data of the symbol
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void symt_add(Symtab table, char *id, tab_item_data data, int *ef)
{
    // Contains the hashed identificator
    uint32_t index = murmurhash(id, (uint32_t)strlen(id), 0);
    // Checking if NULL was not passed as a pointer to the hash table
    if (table != NULL)
    {
        // Allocating memory
        table[index] = malloc(sizeof(struct tab_item));
        // Checking if malloc failed
        if (table[index] == NULL)
        {
            *ef = INTERNAL_ERR;
            return;
        }
        // Writing data
        table[index]->data = data;
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}

/**
 * @brief Finds the symbol in the hash table and returns its value
 *
 * @param table Pointer to the hash table
 * @param id Index of the symbol
 * @param data_ret Pointer to the data struct that will contain data of found symbol
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void symt_find(Symtab table, char *id, tab_item_data *data_ret, int *ef)
{
    // Checking if NULL was not passed as a pointer to the hash table
    if (table != NULL)
    {
        // Contains the hashed identificator
        uint32_t index = murmurhash(id, (uint32_t)strlen(id), 0);
        *data_ret = table[index]->data;
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}
/**
 * @brief Frees the symbol hash table
 *
 * @param table Pointer to the hash table
 * @param ef Pointer to the error flag variable
 *
 * @retval INTERNAL_ERR if error occurred
 */
void symt_free(Symtab table, int *ef)
{
    // Checking if NULL was not passed as a pointer to the hash table
    if (table != NULL)
    {
        for (int i = 0; i < SYMTAB_MAX_SIZE; i++)
        {
            if (table[i] != NULL)
            {
                free(table[i]);
            }
        }
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}