/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file generator.c
 * @brief File containing implementation of defined functions in the generator.h file
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#include "generator.h"
#include "list.h"
#include "symtable.h"
#include "dyn_str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void gen_header()
{
    printf("# Start of IFJcode22 program\n");
    printf(".IFJcode22\n");
}

void gen_jump_to_main()
{
    printf("JUMP $$main\n");
    printf("\n");
}

void gen_main_label()
{
    printf("# Main body of program\n");
    printf("LABEL $$main\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
}

// problem with TAB
void gen_write_var(tab_item_data data)
{
    printf("WRITE ");
    switch (data.type)
    {
    case INT_T:
        printf("int@%d\n", (int)data.num_val);
        break;
    case STRING_T:;
        char *str;
        dyn_str_init(&str);
        dyn_str_alloc(&str);
        for (int i = 0, j = 0; data.str_val[i] != '\0'; j++, i++)
        {
            if (data.str_val[i] == ' ' || data.str_val[i] == '#' || data.str_val[i] == '\\' || !isprint(data.str_val[i]))
            {
                char temp[4];
                str = dyn_str_addc(str, '\\');
                sprintf(temp, "%03d", data.str_val[i]);
                str = dyn_str_cat(str, temp);
            }
            else
            {
                str = dyn_str_addc(str, data.str_val[i]);
            }
        }
        printf("string@%s\n", str);
        free(str);
        break;
    case FLOAT_T:
        printf("float@%a\n", data.num_val);
        break;
    default:
        break;
    }
}

void gen_func_def_header_args(tab_item_data data, char *id)
{
    printf("LABEL $%s\n", id);
    if (data.params->firstElement != NULL)
    {
        printf("PUSHFRAME\n");
        list_first(data.params);
        int cnt = 1;
        while (data.params->activeElement != NULL)
        {
            printf("DEFVAR LF@%s\n", data.params->activeElement->data.identifier);
            printf("MOVE LF@%s LF@_%d\n", data.params->activeElement->data.identifier, cnt);
            list_next(data.params);
        }
    }
}

void gen_var(char *id)
{
    printf("DEFVAR LF@%s\n", id);
}

void gen_params(tab_item_data func_data, List *arg_list)
{
    printf("CREATEFRAME\n");
    list_first(func_data.params);
    list_first(arg_list);
    int cnt = 1;
    while (func_data.params->activeElement != NULL)
    {
        printf("DEFVAR TF@_%d\n", cnt);
        if (arg_list->activeElement->data.identifier)
        {
            printf("MOVE TF@_%d LF@%s\n", cnt, arg_list->activeElement->data.identifier);
        }
        else
        {
            switch (arg_list->activeElement->data.type)
            {
            case INT_T:
                printf("MOVE TF@_%d int@%d\n", cnt, (int)arg_list->activeElement->data.num_val);
                break;
            case STRING_T:;
                char *str;
                dyn_str_init(&str);
                dyn_str_alloc(&str);
                for (int i = 0, j = 0; arg_list->activeElement->data.str_val[i] != '\0'; j++, i++)
                {
                    if (arg_list->activeElement->data.str_val[i] == ' ' || arg_list->activeElement->data.str_val[i] == '#' || arg_list->activeElement->data.str_val[i] == '\\' || !isprint(arg_list->activeElement->data.str_val[i]))
                    {
                        char temp[4];
                        str = dyn_str_addc(str, '\\');
                        sprintf(temp, "%03d", arg_list->activeElement->data.str_val[i]);
                        str = dyn_str_cat(str, temp);
                    }
                    else
                    {
                        str = dyn_str_addc(str, arg_list->activeElement->data.str_val[i]);
                    }
                }
                printf("MOVE TF@_%d string@%s\n", cnt, str);
                free(str);
                break;
            case FLOAT_T:
                printf("MOVE TF@_%d float@%a", cnt, arg_list->activeElement->data.num_val);
                break;
            case VOID_T:
                break;
            }
        }
        cnt++;
        list_next(arg_list);
        list_next(func_data.params);
    }
}
