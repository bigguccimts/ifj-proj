/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file precedence.h
 * @brief kratky popis daneho suboru
 *
 * @author Matúš Ďurica (xduric06)
 * @author Gabriela Paganíková (xpagan00)
 */

#ifndef PRECEDENCE_H
#define PRECEDENCE_H

#include "scanner.h"
#include "stack.h"
#include <stdbool.h>

#define RESET_FLAGS        \
    concat_called = false; \
    is_float = false;      \
    is_int = false;        \
    is_string = false;     \
    out_bool = false;      \
    str_cnt = 0;

#define FREE_SEMANTIC_ERR  \
    stack_free(&stack);    \
    ret = TYPE_COMP_ERR;   \
    concat_called = false; \
    RESET_FLAGS;

#define FREE_SYNTAX_ERR        \
    stack_free(&stack);        \
    ret = SYNTAX_ANALYSIS_ERR; \
    RESET_FLAGS;

typedef enum
{
    SM, // < shift
    BI, // > reduce
    EQ, // =
    EM  // empty - error
} prec_signs;

typedef enum
{
    _PLUS_MINUS_CONCAT, // + - .
    _MUL_DIV,           // * /
    _L_PAR,             // (
    _R_PAR,             // )
    _EQ_NEQ,            // === !==
    _L_LEQ_G_GEQ,       // < <= > >=
    _DATA,              // i
    _DOLLAR,            // $
    _HANDLE,            // <
    _NONTERM            // E
} prec_val;

typedef enum
{
    _RULE_I,                 // E -> i
    _RULE_PLUS_MINUS_CONCAT, // E -> E + - . E
    _RULE_MUL_DIV,           // E -> E * / E
    _RULE_BRACKETS,          // E -> (E)
    _RULE_L_LEQ_G_GEQ,       // E -> E < <= > >= E
    _RULE_EQ_NEQ,            // E -> E === !== E
    _RULE_NOT_A_RULE
} prec_rules;

int process_expression(List *, Symtab, bool, Data_type *);

TOKEN gen_token_val(prec_val);

#endif // PRECEDENCE_H