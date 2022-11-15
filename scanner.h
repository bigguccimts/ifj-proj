/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file subor.c
 * @brief kratky popis daneho suboru
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    Start,
    ID,
    Int,
    Float,
    Exp,
    Exp1,
    Exp2,
    Add,
    Sub,
    Mul,
    Div,
    Conc,
    Rol,
    Ror,
    Cul,
    Cur,
    Equ,
    Equ1,
    Equ2,
    Less,
    Less1,
    Great,
    Great1,
    Com,
    Com1,
    Com2,
    Com3,
    Com4,
    String,
    String1,
    Esc,
    Esc1,
    Esc2,
    Esc3,
    Esc4,

    Sem,
    Col,
    ERROR,
} States;

typedef enum
{
    ES_ID,
    ES_Int,
    ES_Float,
    ES_Exp2,
    ES_Add,
    ES_Sub,
    ES_Mul,
    ES_Div,
    ES_Conc,
    ES_Rol,
    ES_Ror,
    ES_Cul,
    ES_Cur,
    ES_Equ,
    ES_Equ1,
    ES_Equ2,
    ES_Less,
    ES_Less1,
    ES_Great,
    ES_Great1,
    ES_Com1,
    ES_Com4,
    ES_String1,
    ES_KEY_WORD,
    ES_ERROR,
} End_States;

typedef struct TOKEN
{
    End_States end_state;

    union
    {
        float floating;
        int intiger;
        char *Str;
    } Value;
    //size_t value;
} TOKEN;

