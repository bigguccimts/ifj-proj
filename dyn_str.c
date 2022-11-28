/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file dyn_str.c
 * @brief File containing the implementation of defined functions in the dyn_str.h file
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#include <stdlib.h>
#include <string.h>
#include "dyn_str.h"

/**
 * @brief Initializes the dynamic string
 *
 * @param src Pointer to the dynamic string
 */
void dyn_str_init(char **src)
{
    *src = NULL;
}

/**
 * @brief Allocates and nulls the first char of the dynamic string
 *
 * @param src Pointer to the dynamic string
 */
void dyn_str_alloc(char **src)
{
    *src = (char *)calloc(1, sizeof(char));
}

/**
 * @brief Equivalent of strcpy for dynamic string
 *
 * @param dest Destination string
 * @param src Source string
 * @return char* Modified destination string
 */
char *dyn_str_cpy(char *dest, char *src)
{
    char *tmp = (char *)realloc(dest, sizeof(char) * (strlen(src) + 1));
    dest = tmp;
    strcpy(dest, src);
    return dest;
}

/**
 * @brief Equivalent of strcat for dynamic string
 *
 * @param dest Destination string
 * @param src Source string
 * @return char* Modified destination string
 */
char *dyn_str_cat(char *dest, char *src)
{
    char *tmp = (char *)realloc(dest, sizeof(char) * (strlen(dest) + strlen(src) + 1));
    dest = tmp;
    strcat(dest, src);
    return dest;
}

/**
 * @brief Adds character to the end of the dynamic string
 *
 * @param dest Destination string
 * @param c Character
 * @return char* Modified destination string
 */
char *dyn_str_addc(char *dest, char c)
{
    char *tmp = (char *)realloc(dest, sizeof(char) * (strlen(dest) + 2));
    dest = tmp;
    strncat(dest, &c, 1);
    return dest;
}
