#include <stdio.h>

struct TOKEN
{
    enum End_States ES;
    struct Value value;
};

struct Value
{   
    //TO DO somethinng for string values dinamicle array
    enum lex;
    int intiger;
    float floating;
};
typedef enum Type_Of_LEX
{
    //TO DO $-this litle shit goes where ?
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
    LEX_lomeno,
    LEX_coments,
};
enum End_States
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
    ES_KEY_WORD
};
