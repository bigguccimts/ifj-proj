/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file dyn_str.h
 * @brief File containing definitions of functions for the dynamic string
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#ifndef DYN_STR_H
#define DYN_STR_H

void dyn_str_init(char **);

void dyn_str_alloc(char **);

char *dyn_str_cpy(char *, char *);

char *dyn_str_cat(char *, char *);

char *dyn_str_addc(char *, char);

#endif // DYN_STR_H
