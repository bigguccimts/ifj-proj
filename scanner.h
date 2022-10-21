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
typedef struct TOKEN
{
    enum ES
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
        ES_EQU,
        ES_EQU1,
        ES_EQU2,
        ES_Less,
        ES_Less1,
        ES_great,
        ES_great1,
        ES_Com2,
        ES_Com5,
        ES_String1,
        ES_KEY_WORD,
        ES_ERROR,
    }End_States;

    union{
        int intiger;
        char *Str;
    }Value;
}TOKEN;

/*typedef enum 
{
    // TO DO $-this litle shit goes where ?
    LEX_else,
    LEX_float,
    LEX_function,
    LEX_if,
    LEX_int,
    LEX_null,
    LEX_return,
    LEX_string,
    LEX_void,
    LEX_while,
    LEX_plus,
    LEX_minus,
    LEX_multypli,
    LEX_divide,
    LEX_L_round_bracket,
    LEX_R_round_bracket,
    LEX_L_curly_bracket,
    LEX_R_curly_bracket,
    LEX_equals,
    LEX_2equals,
    LEX_3equals,
    LEX_3equals_negation,
    LEX_less,
    LEX_less_eaqual,
    LEX_great,
    LEX_great_eaqual,
    LEX_slash,
    LEX_comments,
}Type_Of_LEX;*/

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
    ES_EQU,
    ES_EQU1,
    ES_EQU2,
    ES_Less,
    ES_Less1,
    ES_great,
    ES_great1,
    ES_Com2,
    ES_Com5,
    ES_String1,
    ES_KEY_WORD,
    ES_ERROR,
} End_States;

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

    ERROR,
}States;


