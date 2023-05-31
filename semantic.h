/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file semantic.h
 * @brief Semantic analysis
 *
 * @author Dušan Slúka (xsluka00)
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "symtable.h"
#include "returncodes.h"
#include "scanner.h"
#include "dyn_str.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int check_params_function(char *, Symtab , List *);
int check_notdef_functions(Symtab , List *);
int check_redef(Symtab , char *);
int check_defvar(Symtab , char *);
int check_return_type(Symtab ,Symtab , TOKEN , char *);

#endif // SEMANTIC_H