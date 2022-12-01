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
#include "list.h"
#include "returncodes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH(id) (murmurhash(id, (uint32_t)strlen(id), 0))

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
            // list_init(table[i]->data.params);
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
    // Checking if NULL was not passed as a pointer to the hash table
    if (table != NULL)
    {
        // Contains the hashed identificator
        uint32_t index = HASH(id);
        if (table[index] == NULL)
        {
            // Allocating memory
            Symtab_item tmp = malloc(sizeof(struct tab_item));
            // Checking if malloc failed
            if (tmp != NULL)
            {
                tmp->identifier = id;
                tmp->data = data;
                tmp->next_item = table[index];
                table[index] = tmp;
            }
            else
            {
                *ef = INTERNAL_ERR;
            }
        }
        else
        {
            // Rewriting data on the same id
            if (!strcmp(table[index]->identifier, id))
            {
                table[index]->data = data;
            }
        }
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
        Symtab_item tmp = table[HASH(id)];
        while (tmp)
        {
            if (!strcmp(id, tmp->identifier))
            {
                *data_ret = tmp->data;
                break;
            }
            tmp = tmp->next_item;
        }
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
        Symtab_item tmp, del;
        for (int i = 0; i < SYMTAB_MAX_SIZE; i++)
        {
            tmp = table[i];
            del = NULL;
            while (tmp)
            {
                del = tmp;
                tmp = tmp->next_item;
                list_dispose(del->data.params);
                free(del);
            }
        }
    }
    else
    {
        *ef = INTERNAL_ERR;
    }
}
