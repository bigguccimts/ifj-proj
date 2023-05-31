/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file generator.c
 * @brief File containing implementation of defined functions in the generator.h file
 *
 * @author Matúš Ďurica (xduric06)
 */

#include "generator.h"
#include "list.h"
#include "symtable.h"
#include "dyn_str.h"
#include "datatypes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Generates label of main body of the IFJcode22 program, then creates and pushes local frame
 *
 */
static void gen_main_label()
{
    printf("# Main body of program\n");
    printf("LABEL $$main\n");
    CREATEFRAME;
    PUSHFRAME;
    gen_var_def("_BOOL_IF_RET");
    gen_var_def("_STRING_OP_TEMP_RETURN");
    gen_var_def("_STRING_OP_TEMP");
}

/**
 * @brief Generates header of IFJcode22 program
 *
 */
void gen_header()
{
    printf("# Start of IFJcode22 program\n");
    printf(".IFJcode22\n");
    JUMP("$$main\n");
    GEN_BUILT_IN_ORD;
    GEN_BUILT_IN_CHR;
    GEN_BUILT_IN_STRLEN;
    GEN_BUILT_IN_SUBSTRING;
    GEN_BUILT_IN_FLOATVAL;
    GEN_BUILT_IN_INTVAL;
    GEN_BUILT_IN_STRVAL;
    gen_main_label();
}

/**
 * @brief Aux function generating constant
 *
 * @param type Data type of the constant
 * @param str_val String value of the constant
 * @param num_val Numerical value of the constant
 */
void gen_const(Data_type type, char *str_val, float num_val)
{
    switch (type)
    {
    case INT_T:
        printf("int@%d\n", (int)num_val);
        break;
    case STRING_T:;
        char *str;
        dyn_str_init(&str);
        dyn_str_alloc(&str);
        for (int i = 0, j = 0; str_val[i] != '\0'; j++, i++)
        {
            if (str_val[i] == ' ' ||
                str_val[i] == '#' ||
                str_val[i] == '\\' ||
                !isprint(str_val[i]))
            {
                char temp[4];
                str = dyn_str_addc(str, '\\');
                sprintf(temp, "%03d", str_val[i]);
                str = dyn_str_cat(str, temp);
            }
            else
            {
                str = dyn_str_addc(str, str_val[i]);
            }
        }
        printf("string@%s\n", str);
        free(str);
        break;
    case FLOAT_T:
        printf("float@%a\n", num_val);
        break;
    case VOID_T:
        printf("string@\n");
        break;
    default:
        break;
    }
}

/**
 * @brief Generates WRITE function for printing constant
 *
 * @param type Data type of the constant
 * @param str_val String value
 * @param num_val Numerical value
 */
void gen_write_const(Data_type type, char *str_val, float num_val)
{
    printf("WRITE ");
    gen_const(type, str_val, num_val);
}

/**
 * @brief Generates WRITE function for printing local variable
 *
 * @param id Identificator of the variable
 */
void gen_write_var(char *id)
{
    printf("WRITE LF@%s\n", id);
}

/**
 * @brief Generates calling of function
 *
 * @param id Identificator of the function
 */
void gen_func_call(char *id)
{
    printf("CALL $%s\n", id);
}

/**
 * @brief Generates label of a function and moves its arguments into local frame
 *
 * @param data Data struct of the function
 * @param id Identificator of the function
 */
void gen_func_def_header_args(tab_item_data data, char *id)
{
    GEN_SKIP(id);
    printf("# Definition of function %s\n", id);
    printf("LABEL $%s\n", id);
    if (data.params.firstElement != NULL)
    {
        PUSHFRAME;
        list_first(&data.params);
        int cnt = 1;
        while (data.params.activeElement != NULL)
        {
            gen_var_def(data.params.activeElement->data.identifier);
            printf("MOVE LF@%s LF@_%d\n", data.params.activeElement->data.identifier, cnt);
            list_next(&data.params);
            cnt++;
        }
    }
    gen_var_def("_BOOL_IF_RET");
    gen_var_def("_STRING_OP_TEMP_RETURN");
    gen_var_def("_STRING_OP_TEMP");
}

/**
 * @brief Generates definition of a variable
 *
 * @param id Identificator of the variable
 */
void gen_var_def(char *id)
{
    printf("DEFVAR LF@%s\n", id);
}

/**
 * @brief Generates MOVE instruction to a local variable from a constant
 *
 * @param id Identificator of the variable
 * @param type Data type of the variable
 * @param num_val Numerical value of the variable
 * @param str_val String value of the variable
 */
void gen_var_move_const(char *id, Data_type type, float num_val, char *str_val)
{
    printf("MOVE LF@%s ", id);
    gen_const(type, str_val, num_val);
}

/**
 * @brief Generates MOVE instruction between two variables
 *
 * @param dest_frame Destination frame
 * @param dest_var_id Destination variable identificator
 * @param src_frame Source frame
 * @param src_var_id Source variable identificator
 */
void gen_var_move_var(char *dest_frame, char *dest_var_id, char *src_frame, char *src_var_id)
{
    printf("MOVE %s@%s %s@%s\n", dest_frame, dest_var_id, src_frame, src_var_id);
}

/**
 * @brief Generates adding arguments to function sequence
 *
 * @param func_data Data struct of the functions
 * @param arg_list List containing arguments passed to the function
 */
void gen_params(tab_item_data func_data, List *arg_list)
{
    CREATEFRAME;
    list_first(&func_data.params);
    list_first(arg_list);
    int cnt = 1;
    while (func_data.params.activeElement != NULL)
    {
        printf("DEFVAR TF@_%d\n", cnt);
        if (arg_list->activeElement->data.identifier)
        {
            printf("MOVE TF@_%d LF@%s\n", cnt, arg_list->activeElement->data.identifier);
        }
        else
        {
            printf("MOVE TF@_%d ", cnt);
            gen_const(arg_list->activeElement->data.type,
                      arg_list->activeElement->data.str_val,
                      arg_list->activeElement->data.num_val);
        }
        cnt++;
        list_next(arg_list);
        list_next(&func_data.params);
    }
}

/**
 * @brief Generates returning sequence from a function
 *
 */
void gen_func_return(char *id)
{
    POPFRAME;
    RETURN;
    printf("# End of definition and function definition skip label\n");
    printf("LABEL $_SKIP_DEF_%s\n", id);
}