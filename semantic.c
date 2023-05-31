/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file semantic.c
 * @brief Semantic analysis
 *
 * @author Dušan Slúka (xsluka00)
 */

#include "symtable.h"
#include "returncodes.h"
#include "scanner.h"
#include "dyn_str.h"
#include "semantic.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief comparse 2 lists to check if they are same.
 *
 * @param args_called List of parameters with which was function at hand called
 * @param args_tab    List of parameters with which was function at hand declared
 * @param table       which contains functions in main scoup of program
 * @return int 0 - good
 * @return int 1 - error for args
 * @return int 2 - error for not found VAR
 */
int check_args(List *args_called, List *args_tab , Symtab tab)
{
    list_first(args_called);
    list_first(args_tab);

    while (args_called->activeElement != NULL && args_tab->activeElement != NULL)
    {
        if(args_called->activeElement->data.identifier != NULL){
            
            if(check_defvar(tab, args_called->activeElement->data.identifier) != 0){
                return 2;
            }
        }

        if (args_tab->activeElement->data.type != args_called->activeElement->data.type)
        {
            return 1;
        }

        args_called->activeElement = args_called->activeElement->nextElement;
        args_tab->activeElement = args_tab->activeElement->nextElement;
    }

    // Both list should be on end after check
    if (args_called->activeElement != NULL || args_tab->activeElement != NULL)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief checks right number of argumants and their types
 *
 * @param ID    ID of function in which we want to check
 * @param table which contains functions in main scoup of program
 * @param args_called List of parameters with which was function at hand called
 * @return  - ALL_GOOD
 * @return  - UNDEF_PARAM_ERR
 * @return  - UNDEF_VAR_ERR
 */

int check_params_function(char *ID, Symtab table, List *args_called)
{
    tab_item_data wanted;
    int flag = 0;

    symt_find(table, ID, &wanted, &flag);

    if (flag != 0)
    {
        return flag;
    }

    List args_tab = wanted.params;
    int err = check_args(args_called, &args_tab, table);
    if (err != 0)
    {
        if (err == 1)
        {
            return UNDEF_PARAM_ERR;
        }
        if (err == 2)
        {
           return UNDEF_VAR_ERR;
        }
    }

    return ALL_GOOD;
}
/**
 * @brief checks if function is defined
 *
 * @param table which contains functions in main scoup of program
 * @param ID    ID of function in which we want to check
 * @return true if was def
 * @return false if was´t def
 */
bool check_is_def(Symtab table, char *ID)
{
    tab_item_data item;
    int flag = 0;
    bool found;
    found = symt_find(table, ID, &item, &flag);

    if (flag == 0 && item.def == true && found)
    {
        return true;
    }

    return false;
}
/**
 * @brief Checks if evry callled function is defined
 *
 * @param table           which contains functions in main scoup of program
 * @param ID_of_functions List of function ID´s , which where called before declaration of function
 * @return int 0 - Doood
 * @return int 1 - Error
 */
int check_notdef_functions(Symtab table, List *ID_of_functions)
{

    list_first(ID_of_functions);
    char *ID;

    while (ID_of_functions->activeElement != NULL)
    {

        ID = ID_of_functions->activeElement->data.identifier;

        if (check_is_def(table, ID) == false)
        {
            return UNDEF_FUNC_ERR;
        }

        ID_of_functions->activeElement = ID_of_functions->activeElement->nextElement;
    }
    return ALL_GOOD;
}
/**
 * @brief checks if  the functin isn t redifibned
 *
 * @param table which contains function in main scoup of program
 * @param ID    ID of function in which we are at the moment
 * @return int 0 - GOOD
 * @return int 1 - ERROR
 */
int check_redef(Symtab table, char *ID)
{
    bool found;
    int *flag = 0;
    tab_item_data item;
    found = symt_find(table, ID, &item, flag);

    if (item.def == true && found)
    {
        return UNDEF_FUNC_ERR;
    }

    return ALL_GOOD;
}
/**
 * @brief checks if variable is in symtable
 *
 * @param table which contains variables in main or local scoup of program
 * @param ID    ID of variable in which we are at the moment
 * @return int 0 - GOOD
 * @return int 1 - ERROR
 */

int check_defvar(Symtab table, char *ID)
{
    bool found;
    int *flag = 0;
    tab_item_data item;
    found = symt_find(table, ID, &item, flag);

    if (found == false)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief checks if return has same ttpe as function it is in also if var exists.
 *
 * @param table which contains variables in main scoup of program
 * @param table which contains variables in local scoup of function
 * @param t1    token after keyword return 
 * @param ID    ID of function in which we are at the moment
 * @return int
 */
int check_return_type(Symtab Gtable,Symtab Ltable, TOKEN t1, char *ID)
{
    int *flag = 0;
    tab_item_data item;

    symt_find(Gtable, ID, &item, flag);

    if (item.type == VOID_T && t1.end_state != ES_Sem)
    {
        return WRONG_RET_ERR;
    }

    if (item.type == VOID_T && t1.end_state == ES_Sem)
    {
        return ALL_GOOD;
    }

    if (t1.end_state == ES_Sem)
    {
        return WRONG_RET_ERR;
    }

    if (t1.end_state == ES_Int)
    {
        if (item.type == INT_T)
        {
            return ALL_GOOD;
        }
        return UNDEF_RETURN_ERR;
    }
    else if (t1.end_state == ES_Float)
    {
        if (item.type == FLOAT_T)
        {
            return ALL_GOOD;
        }
        return UNDEF_RETURN_ERR;
    }
    else if (t1.end_state == ES_String1)
    {
        if (item.type == STRING_T)
        {
            return ALL_GOOD;
        }
        return UNDEF_RETURN_ERR;
    }
    else if (t1.end_state == ES_VID2)
    {

        bool varE;
        tab_item_data item2;
        varE = symt_find(Ltable, t1.Value.Str, &item2, flag);


        if (item.type == item2.type && varE)
        {
            return ALL_GOOD;
        }
        return UNDEF_RETURN_ERR;
    }

    return 1;
}
