/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file parser.h
 * @brief Syntax analysis
 *
 * @author Ivan Mahút (xmahut01)
 */
#ifndef PARSER_H
#define PARSER_H

#include "symtable.h"
#include "scanner.h"
#include "list.h"

extern List beforeDef;

int parse(Symtab, Symtab);

int prog(Symtab, Symtab, TOKEN *);

int defFunction(Symtab, Symtab, TOKEN *);

int param(TOKEN *, Symtab, List *);

int paramN(TOKEN *, Symtab, List *);

int stat(Symtab, Symtab, TOKEN *, bool, char *);

int retType(TOKEN *, Symtab, TOKEN *);

int statIf(Symtab, Symtab, TOKEN *, bool, char *);

int statWhile(Symtab, Symtab, TOKEN *, bool, char *);

int callFunction(Symtab, Symtab, TOKEN *, TOKEN *, bool, char *, char *);

int args(Symtab, Symtab, TOKEN *, List *, bool);

int argsN(Symtab, Symtab, TOKEN *, List *, bool);

int valuesVar(Symtab, Symtab, TOKEN *, TOKEN *, TOKEN *, bool, char *);

int callBIF(Symtab, Symtab, TOKEN *, TOKEN *, bool, char *, char *);

int statRet(Symtab, Symtab, TOKEN *, bool, char *);

int loadBuiltin(Symtab);

#endif
