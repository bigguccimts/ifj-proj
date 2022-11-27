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
#include "scanner.h"

#include <stdio.h>
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
    case BOOL_T:
        break;
    case STRING_T:;
        char *tmp_str = NULL;

        tmp_str = init_Str(tmp_str, DYN_STR_SIZE);
        for (int i, j = 0; data.str_val[i] != '\0'; j++, i++)
        {
            if (data.str_val[i] == ' ' || data.str_val[i] == '#' || data.str_val[i] == '\\' || data.str_val[i] <= 32)
            {
                char temp[4];
                tmp_str[j] = '\\';
                tmp_str = resize_Str(tmp_str, j + 4);
                sprintf(temp, "%03d", data.str_val[i]);
                strcat(tmp_str, temp);
                j = j + 3;
            }
            else
            {
                tmp_str[j] = data.str_val[i];
                tmp_str = resize_Str(tmp_str, j + 1);
            }
        }
        printf("string@%s\n", tmp_str);
        clean_Str(tmp_str);
        break;
    case FLOAT_T:
        printf("float@%a\n", data.num_val);
        break;
    default:
        break;
    }
}

void gen_func_def(Symtab_item item)
{
    printf("LABEL %s\n", item->identifier);
    printf("PUSHFRAME\n");
}