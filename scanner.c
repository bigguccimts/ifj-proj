#include <stdio.h>

struct TOKEN
{
    enum End_States ES;
    struct Value value;
};

struct Value{
    
    enum lex;
    int intiger;
    float floating;

};
typedef enum Type_OF_LEX{

    IF,
    WHILE,
    C



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
