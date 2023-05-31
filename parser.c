/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file parser.c
 * @brief Syntax analysis
 *
 * @author Ivan Mahút (xmahut01)
 */
#include "parser.h"
#include "scanner.h"
#include "returncodes.h"
#include "symtable.h"
#include "stack.h"
#include "list.h"
#include "string.h"
#include "generator.h"
#include "semantic.h"

#define TOKCHCK(token)                                                 \
    if (token.end_state == ES_ERROR)                                   \
    {                                                                  \
        if (token.Value.intiger == 3)                                  \
            return LEX_ANALYSIS_ERR;                                   \
        else                                                           \
        {                                                              \
            return SYNTAX_ANALYSIS_ERR;                                \
        }                                                              \
    }                                                                  \
    else if (token.end_state == ES_Com1 || token.end_state == ES_Com4) \
    {                                                                  \
        token = generate_token();                                      \
    }

int returnCount = 0;
int ifCount = 1;
int whileCount = 1;
List beforeDef;

int parse(Symtab globalT, Symtab localT)
{
    int error = ALL_GOOD;

    list_init(&beforeDef);
    error = loadBuiltin(globalT);
    if (error != ALL_GOOD)
        return error;
    TOKEN token;
    token = generate_token();
    TOKCHCK(token);
    gen_header();
    token = generate_token();
    TOKCHCK(token);
    error = prog(globalT, localT, &token);
    if (beforeDef.firstElement != NULL)
    {
        check_notdef_functions(globalT, &beforeDef);
        list_dispose(&beforeDef);
    }
    return error;
}

/**
 * @brief Function for rule #1-4
 *
 * @param globatT global symtable
 * @param localT local symtable
 * @param token current token
 * @return int err flag
 */
int prog(Symtab globalT, Symtab localT, TOKEN *token)
{
    int err = ALL_GOOD;
    // rule #2 Program -> ?>
    if (token->end_state == ES_EOP2)
    {
        return err;
    }
    else if ((token->end_state == ES_KEY_WORD) && (strcmp(token->Value.Str, "function") == 0))
    {
        // rule #3 Program -> function def
        err = defFunction(globalT, localT, token);
        if (err != ALL_GOOD)
            return err;
    }
    else
    {
        // rule #4 Program -> Statement
        err = stat(globalT, localT, token, false, NULL);
        if (err != ALL_GOOD)
            return err;
    }
    return err;
}
/**
 * @brief Function for rule #3 Program -> function def
 *
 * @param globalT global symtab
 * @param localT local symtab
 * @param tokenPtr current token
 * @return int err flag
 */
int defFunction(Symtab globalT, Symtab localT, TOKEN *tokenPtr)
{
    int err = ALL_GOOD;
    TOKEN token = generate_token();
    TOKCHCK(token);
    // function_ID
    if (token.end_state == ES_FID)
    {
        TOKEN idTok = token;
        tab_item_data data;
        INIT_SYMT_DATA(data);
        // second definition check
        err = check_redef(globalT, token.Value.Str);
        if (err != ALL_GOOD)
            exit(err);
        token = generate_token();
        TOKCHCK(token);
        // Left (
        if (token.end_state == ES_Rol)
        {
            token = generate_token();
            TOKCHCK(token);
            // params
            if (token.end_state == ES_KEY_WORD)
            {
                List params;
                list_init(&params);
                err = param(&token, localT, &params);
                if (!symt_find(globalT, idTok.Value.Str, &data, &err))
                {
                    data.def = true;
                    symt_add_list(&data, params, &err);
                    list_dispose(&params);
                    symt_add(globalT, idTok.Value.Str, data, &err);
                    gen_func_def_header_args(data, idTok.Value.Str);
                }
                else
                {
                    if (data.def == false)
                    {
                        tab_item_data tmpData;
                        INIT_SYMT_DATA(tmpData);
                        tmpData.def = true;
                        symt_find(globalT, idTok.Value.Str, &tmpData, &err);
                        // list_dispose(&tmpData.params);
                        List calledArgs;
                        list_init(&calledArgs);
                        calledArgs = tmpData.params;
                        symt_add_list(&tmpData, params, &err);
                        symt_add(globalT, idTok.Value.Str, tmpData, &err);
                        gen_func_def_header_args(tmpData, idTok.Value.Str);
                        err = check_params_function(idTok.Value.Str, globalT, &calledArgs);
                        list_dispose(&calledArgs);
                        if (err != ALL_GOOD)
                            return err;
                    }
                }
                if (err != ALL_GOOD)
                    return err;
                if (token.end_state == ES_Ror)
                {
                    token = generate_token();
                    TOKCHCK(token);
                }
                else
                {
                    fprintf(stderr, "Expected a ')'\n");
                    return SYNTAX_ANALYSIS_ERR;
                }
            }
            else if (token.end_state == ES_Ror)
            {
                GEN_FUNC_DEF_HEADER_NOARGS(idTok.Value.Str);
                token = generate_token();
                TOKCHCK(token);
            }
            else
            {
                fprintf(stderr, "Expected a ')'\n");
                return SYNTAX_ANALYSIS_ERR;
            }

            // :
            if (token.end_state == ES_Col)
            {
                token = generate_token();
                TOKCHCK(token);
                // Ret_Type
                err = retType(&token, globalT, &idTok);
                if (err != ALL_GOOD)
                    return err;
                token = generate_token();
                TOKCHCK(token);
                // {
                if (token.end_state == ES_Cul)
                {
                    token = generate_token();
                    TOKCHCK(token);
                    err = stat(globalT, localT, &token, true, idTok.Value.Str);
                    if (err != ALL_GOOD)
                        return err;
                }
                else
                {
                    fprintf(stderr, "Expected a '{'\n");
                    return SYNTAX_ANALYSIS_ERR;
                }

                // }
                if (token.end_state == ES_Cur)
                {
                    gen_func_return(idTok.Value.Str);
                    token = generate_token();
                    TOKCHCK(token);
                    symt_free(localT, &err);
                    symt_init(localT, &err);
                    if (returnCount > 0)
                    {
                        returnCount = 0;
                    }
                    else
                    {
                        fprintf(stderr, "Expected a return\n");
                        return SYNTAX_ANALYSIS_ERR;
                    }
                    err = prog(globalT, localT, &token);
                    if (err != ALL_GOOD)
                        return err;
                }
                else
                {
                    token = generate_token();
                    TOKCHCK(token);
                    if (token.end_state == ES_Cur)
                    {
                        gen_func_return(idTok.Value.Str);
                        token = generate_token();
                        TOKCHCK(token);
                        symt_free(localT, &err);
                        symt_init(localT, &err);
                        if (returnCount > 0)
                        {
                            returnCount = 0;
                        }
                        else
                        {
                            fprintf(stderr, "Expected a return\n");
                            return WRONG_RET_ERR;
                        }
                        err = prog(globalT, localT, &token);
                        if (err != ALL_GOOD)
                            return err;
                    }
                    else
                    {
                        fprintf(stderr, "Expected a '}'\n");
                        return SYNTAX_ANALYSIS_ERR;
                    }
                }
            }
            else
            {
                fprintf(stderr, "Expected a ':'\n");
            }
        }
        else
        {
            fprintf(stderr, "Expected a '('\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else
    {
        fprintf(stderr, "Missing function ID\n");
        return SYNTAX_ANALYSIS_ERR;
    }
    (*tokenPtr) = token;
    return err;
}

/**
 * @brief Function for rule # 5
 *
 * @param token current token
 * @return int err code
 */
int param(TOKEN *tokenPtr, Symtab localT, List *list)
{
    int err = ALL_GOOD;
    TOKEN token = (*tokenPtr);
    TOKEN idTOK = generate_token();
    TOKCHCK(idTOK);
    if (idTOK.end_state != ES_VID2)
    {
        fprintf(stderr, "Expected an ID\n");
        return SYNTAX_ANALYSIS_ERR;
    }
    tab_item_data data;
    INIT_SYMT_DATA(data);
    params_data paramDT;
    INIT_PARAM_DATA(paramDT);
    if (strcmp(token.Value.Str, "int") == 0)
    {
        // adding param to local symtab
        data.type = INT_T;
        symt_add(localT, idTOK.Value.Str, data, &err);
        if (err != ALL_GOOD)
            return err;
        // adding param to list of params
        paramDT.identifier = idTOK.Value.Str;
        paramDT.type = INT_T;
        list_insert(list, paramDT, &err);
        if (err != ALL_GOOD)
            return err;
        paramN(&token, localT, list);
    }
    else if (strcmp(token.Value.Str, "float") == 0)
    {
        data.type = FLOAT_T;
        symt_add(localT, idTOK.Value.Str, data, &err);
        if (err != ALL_GOOD)
            return err;

        paramDT.identifier = idTOK.Value.Str;
        paramDT.type = FLOAT_T;
        list_insert(list, paramDT, &err);
        if (err != ALL_GOOD)
            return err;
        paramN(&token, localT, list);
    }
    else if (strcmp(token.Value.Str, "String") == 0)
    {
        data.type = STRING_T;
        symt_add(localT, idTOK.Value.Str, data, &err);
        if (err != ALL_GOOD)
            return err;

        paramDT.identifier = idTOK.Value.Str;
        paramDT.type = STRING_T;
        list_insert(list, paramDT, &err);
        if (err != ALL_GOOD)
            return err;
        paramN(&token, localT, list);
    }
    else
    {
        fprintf(stderr, "Undefined data type\n");
        return SYNTAX_ANALYSIS_ERR;
    }
    (*tokenPtr) = token;
    return ALL_GOOD;
}

/**
 * @brief Rule #7
 *
 * @param token current token
 * @param localT local symtable
 * @param list param list
 * @return int err flag
 */
int paramN(TOKEN *tokenPtr, Symtab localT, List *list)
{
    int err;
    TOKEN token = generate_token();
    TOKCHCK(token);
    if (token.end_state == ES_Comm)
    {
        token = generate_token();
        TOKCHCK(token);
        if (token.end_state == ES_KEY_WORD)
        {
            err = param(&token, localT, list);
            if (err != ALL_GOOD)
                return err;
            (*tokenPtr) = token;
            return ALL_GOOD;
        }
    }
    else
    {
        (*tokenPtr) = token;
        return ALL_GOOD;
    }
    return ALL_GOOD;
}

/**
 * @brief Rule #16 - 21
 *
 * @param globalT global symtable
 * @param localT local symtable
 * @param token current token
 * @param inFunction If true, statement is in function
 * @return int error flag
 */
int stat(Symtab globalT, Symtab localT, TOKEN *tokenPtr, bool inFunction, char *idFunc)
{
    int err = ALL_GOOD;
    bool semDone = false;
    // checks } for end of if/function
    if (tokenPtr->end_state == ES_Cur)
    {
        return ALL_GOOD;
    }
    else if (tokenPtr->end_state == ES_KEY_WORD)
    {
        if (strcmp(tokenPtr->Value.Str, "if") == 0)
        {
            err = statIf(globalT, localT, tokenPtr, inFunction, idFunc);
            if (err != ALL_GOOD)
                return err;
        }
        else if (strcmp(tokenPtr->Value.Str, "while") == 0)
        {
            err = statWhile(globalT, localT, tokenPtr, inFunction, idFunc);
            if (err != ALL_GOOD)
                return err;
        }
        else if (strcmp(tokenPtr->Value.Str, "return") == 0)
        {
            returnCount++;
            err = statRet(globalT, localT, tokenPtr, inFunction, idFunc);
            if (err != ALL_GOOD)
                return err;
        }
    }
    else if (tokenPtr->end_state == ES_FID)
    {
        // function call
        TOKEN idTok = (*tokenPtr);
        TOKEN token = generate_token();
        TOKCHCK(token);
        if (token.end_state == ES_Rol)
        {
            err = callFunction(globalT, localT, &token, &idTok, inFunction, NULL, idFunc);
            if (err != ALL_GOOD)
                return err;
            (*tokenPtr) = token;
            stat(globalT, localT, tokenPtr, inFunction, idFunc);
        }
        else
        {
            fprintf(stderr, "Expected a '('\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else if (tokenPtr->end_state == ES_BIF)
    {
        // built-in function call
        TOKEN idTok = (*tokenPtr);
        TOKEN token = (*tokenPtr);
        err = callBIF(globalT, localT, &token, &idTok, inFunction, NULL, idFunc);
        if (err != ALL_GOOD)
            return err;
        token = generate_token();
        TOKCHCK(token);
        (*tokenPtr) = token;
        stat(globalT, localT, tokenPtr, inFunction, idFunc);
    }
    else if (tokenPtr->end_state == ES_VID2)
    {
        // Variable
        TOKEN idTok = (*tokenPtr);
        TOKEN token;
        if (inFunction)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            // second definition check
            if (!symt_find(localT, idTok.Value.Str, &data, &err))
            {
                tab_item_data data1;
                INIT_SYMT_DATA(data1);
                data1.def = true;
                symt_add(localT, idTok.Value.Str, data1, &err);
                if (err != ALL_GOOD)
                    return err;
                gen_var_def(idTok.Value.Str);
            }
        }
        else
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            // second definition check
            if (!symt_find(globalT, idTok.Value.Str, &data, &err))
            {
                tab_item_data data1;
                INIT_SYMT_DATA(data1);
                data1.def = true;
                symt_add(globalT, idTok.Value.Str, data1, &err);
                gen_var_def(idTok.Value.Str);
            }
        }

        token = generate_token();
        TOKCHCK(token);

        switch (token.end_state)
        {
        case ES_Equ:
            token = generate_token();
            TOKCHCK(token);
            switch (token.end_state)
            {
            case ES_VID2:
            case ES_Int:
            case ES_Float:
            case ES_String1:;
                TOKEN tmpTok = token;
                token = generate_token();
                TOKCHCK(token);
                if (token.end_state == ES_Sem)
                {
                    err = valuesVar(globalT, localT, &token, &tmpTok, &idTok, inFunction, idFunc);
                    if (err != ALL_GOOD)
                        return err;
                    token = generate_token();
                    TOKCHCK(token);
                    err = stat(globalT, localT, &token, inFunction, idFunc);
                    (*tokenPtr) = token;
                    return err;
                }
                else
                {
                    List precL;
                    list_init(&precL);
                    params_data pd1;
                    INIT_PARAM_DATA(pd1);
                    pd1.token = tmpTok;
                    list_insert(&precL, pd1, &err);
                    params_data pd2;
                    INIT_PARAM_DATA(pd2);
                    pd2.token = token;
                    list_insert(&precL, pd2, &err);
                    Data_type datType;
                    if (inFunction)
                    {
                        err = process_expression(&precL, localT, false, &datType);
                        if (err == 999)
                        {
                            TOKEN retTok;
                            retTok.end_state = ES_EOP2;
                            (*tokenPtr) = retTok;
                            return SYNTAX_ANALYSIS_ERR;
                        }
                        else if (err != ALL_GOOD)
                            return err;
                        if (datType == STRING_T)
                        {
                            GEN_MOVE_LOCAL_VARS(idTok.Value.Str, "_STRING_OP_TEMP_RETURN");
                        }
                        else
                        {
                            POPS(idTok.Value.Str);
                        }
                        semDone = true;
                        if (datType == INT_T || datType == STRING_T || datType == FLOAT_T)
                        {
                            tab_item_data dat;
                            INIT_SYMT_DATA(dat);
                            symt_find(localT, idTok.Value.Str, &dat, &err);
                            dat.type = datType;
                            symt_add(localT, idTok.Value.Str, dat, &err);
                        }
                    }
                    else
                    {
                        err = process_expression(&precL, globalT, false, &datType);
                        if (err == 999)
                        {
                            TOKEN retTok;
                            retTok.end_state = ES_EOP2;
                            (*tokenPtr) = retTok;
                            return SYNTAX_ANALYSIS_ERR;
                        }
                        else if (err != ALL_GOOD)
                            return err;
                        if (datType == STRING_T)
                        {
                            GEN_MOVE_LOCAL_VARS(idTok.Value.Str, "_STRING_OP_TEMP_RETURN");
                        }
                        else
                        {
                            POPS(idTok.Value.Str);
                        }
                        semDone = true;
                        if (datType == INT_T || datType == STRING_T || datType == FLOAT_T)
                        {
                            tab_item_data dat;
                            INIT_SYMT_DATA(dat);
                            symt_find(globalT, idTok.Value.Str, &dat, &err);
                            dat.type = datType;
                            symt_add(globalT, idTok.Value.Str, dat, &err);
                        }
                    }
                }
                break;
            case ES_Rol:;
                List precRolL;
                list_init(&precRolL);
                params_data pdRol1;
                INIT_PARAM_DATA(pdRol1);
                pdRol1.token = token;
                list_insert(&precRolL, pdRol1, &err);
                Data_type datType;
                if (inFunction)
                {
                    err = process_expression(&precRolL, localT, false, &datType);
                    if (err == 999)
                    {
                        TOKEN retTok;
                        retTok.end_state = ES_EOP2;
                        (*tokenPtr) = retTok;
                        return SYNTAX_ANALYSIS_ERR;
                    }
                    else if (err != ALL_GOOD)
                        return err;
                    POPS(idTok.Value.Str);
                    semDone = true;
                    if (datType == INT_T || datType == STRING_T || datType == FLOAT_T)
                    {
                        tab_item_data dat;
                        INIT_SYMT_DATA(dat);
                        symt_find(localT, idTok.Value.Str, &dat, &err);
                        dat.type = datType;
                        symt_add(localT, idTok.Value.Str, dat, &err);
                    }
                }
                else
                {
                    err = process_expression(&precRolL, globalT, false, &datType);
                    if (err == 999)
                    {
                        TOKEN retTok;
                        retTok.end_state = ES_EOP2;
                        (*tokenPtr) = retTok;
                        return SYNTAX_ANALYSIS_ERR;
                    }
                    else if (err != ALL_GOOD)
                        return err;
                    POPS(idTok.Value.Str);
                    semDone = true;
                    if (datType == INT_T || datType == STRING_T || datType == FLOAT_T)
                    {
                        tab_item_data dat;
                        INIT_SYMT_DATA(dat);
                        symt_find(globalT, idTok.Value.Str, &dat, &err);
                        dat.type = datType;
                        symt_add(globalT, idTok.Value.Str, dat, &err);
                    }
                }
                break;
            case ES_FID:;
                TOKEN fidTok;
                fidTok.end_state = ES_FID;
                err = valuesVar(globalT, localT, &token, &fidTok, &idTok, inFunction, idFunc);
                if (err != ALL_GOOD)
                    return err;
                break;
            case ES_BIF:;
                TOKEN bifTok;
                bifTok.end_state = ES_BIF;
                err = valuesVar(globalT, localT, &token, &bifTok, &idTok, inFunction, idFunc);
                if (err != ALL_GOOD)
                    return err;
                break;
            default:
                break;
            }

            if (!semDone)
            {
                token = generate_token();
                TOKCHCK(token);
                if (token.end_state != ES_Sem)
                {
                    fprintf(stderr, "Expected a ';'\n");
                    return SYNTAX_ANALYSIS_ERR;
                }
            }

            token = generate_token();
            TOKCHCK(token);
            (*tokenPtr) = token;
            err = stat(globalT, localT, tokenPtr, inFunction, idFunc);
            if (err != ALL_GOOD)
                return err;
            break;
        case ES_Mul:
        case ES_Div:
        case ES_Add:
        case ES_Sub:
        case ES_Conc:
        case ES_Rol:
        case ES_Equ2:
        case ES_Notequ2:
        case ES_Less1:
        case ES_Great1:
        case ES_Less:
        case ES_Great:;
            List precL1;
            list_init(&precL1);
            params_data pd3;
            INIT_PARAM_DATA(pd3);
            pd3.token = token;
            list_insert(&precL1, pd3, &err);
            Data_type datType;
            err = process_expression(&precL1, globalT, false, &datType);
            CLEARS;
            if (err == 999)
            {
                TOKEN retTok;
                retTok.end_state = ES_EOP2;
                (*tokenPtr) = retTok;
                return SYNTAX_ANALYSIS_ERR;
            }
            else if (err != ALL_GOOD)
                return err;

            token = generate_token();
            TOKCHCK(token);
            (*tokenPtr) = token;
            err = stat(globalT, localT, tokenPtr, inFunction, idFunc);
            if (err != ALL_GOOD)
                return err;
            break;
        default:
            break;
        }
    }
    else
    {
        return err;
    }

    if (!inFunction)
    {
        err = prog(globalT, localT, tokenPtr);
    }

    return err;
}

/**
 * @brief
 *
 * @param localT local symtable
 * @param tokenPtr current token
 * @param inFunction If true, statement is in function
 * @return int error flag
 */
int statRet(Symtab globalT, Symtab localT, TOKEN *tokenPtr, bool inFunction, char *idFunc)
{
    int err = ALL_GOOD;
    TOKEN token = generate_token();
    TOKCHCK(token);
    switch (token.end_state)
    {
    case ES_Int:
    case ES_Float:
    case ES_String1:;
        TOKEN tmpTok = token;
        token = generate_token();
        TOKCHCK(token);
        if (token.end_state == ES_Div || token.end_state == ES_Add || token.end_state == ES_Sub ||
            token.end_state == ES_Mul || token.end_state == ES_Less || token.end_state == ES_Less1 ||
            token.end_state == ES_Great1 || token.end_state == ES_Great || token.end_state == ES_Conc ||
            token.end_state == ES_Equ2 || token.end_state == ES_Notequ2)
        {
            List precL;
            list_init(&precL);
            params_data pd1;
            INIT_PARAM_DATA(pd1);
            pd1.token = tmpTok;
            list_insert(&precL, pd1, &err);
            params_data pd2;
            INIT_PARAM_DATA(pd2);
            pd2.token = token;
            list_insert(&precL, pd2, &err);
            Data_type datType;
            err = process_expression(&precL, globalT, false, &datType);
            if (err == 999)
            {
                TOKEN retTok;
                retTok.end_state = ES_EOP2;
                (*tokenPtr) = retTok;
                return SYNTAX_ANALYSIS_ERR;
            }
            else if (err != ALL_GOOD)
                return err;
            TOKEN datCheck;
            switch (datType)
            {
            case INT_T:
                datCheck.end_state = ES_Int;
                break;
            case FLOAT_T:
                datCheck.end_state = ES_Float;
                break;
            case STRING_T:
                datCheck.end_state = ES_String1;
                break;
            default:
                // undefined type
                datCheck.end_state = ES_Sem;
                break;
            }
            err = check_return_type(globalT, localT, datCheck, idFunc);
            if (err != ALL_GOOD)
                return err;
            POPS("_retval");
            token = generate_token();
            TOKCHCK(token);
            err = stat(globalT, localT, &token, inFunction, idFunc);
            (*tokenPtr) = token;
            if (err != ALL_GOOD)
                return err;
        }
        else if (token.end_state == ES_Sem)
        {
            check_return_type(globalT, localT, tmpTok, idFunc);
            switch (tmpTok.end_state)
            {
            case ES_Int:
                GEN_VAR_MOVE_CONST_INT("_retval", tmpTok.Value.intiger);
                break;
            case ES_Float:
                GEN_VAR_MOVE_CONST_FLOAT("_retval", tmpTok.Value.floating);
                break;
            case ES_String1:
                GEN_VAR_MOVE_CONST_STR("_retval", tmpTok.Value.Str);
                break;
            default:
                break;
            }
            token = generate_token();
            TOKCHCK(token);
            err = stat(globalT, localT, &token, inFunction, idFunc);
            (*tokenPtr) = token;
            if (err != ALL_GOOD)
                return err;
        }
        break;
    case ES_VID2:;
        TOKEN idTok = token;
        token = generate_token();
        TOKCHCK(token);
        if (token.end_state == ES_Sem)
        {
            err = check_return_type(globalT, localT, idTok, idFunc);
            if (err != ALL_GOOD)
                return err;
            GEN_MOVE_VAR_TO_RET(idTok.Value.Str);
            token = generate_token();
            TOKCHCK(token);
            err = stat(globalT, localT, &token, inFunction, idFunc);
            (*tokenPtr) = token;
            if (err != ALL_GOOD)
                return err;
        }
        else
        {
            // ego
        }
        break;
    case ES_Sem:
        err = check_return_type(globalT, localT, token, idFunc);
        if (err != ALL_GOOD)
            return err;
        POPFRAME;
        RETURN;
        token = generate_token();
        TOKCHCK(token);
        err = stat(globalT, localT, &token, inFunction, idFunc);
        (*tokenPtr) = token;
        if (err != ALL_GOOD)
            return err;
    default:
        break;
    }
    return err;
}

/**
 * @brief Rule #12-15
 *
 * @param token current token
 * @param globalT global symtab
 * @param idTOk token with function ID
 * @return int
 */
int retType(TOKEN *token, Symtab globalT, TOKEN *idTOk)
{
    int err = ALL_GOOD;
    if (token->end_state == ES_KEY_WORD)
    {
        if (strcmp(token->Value.Str, "void") == 0)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            symt_find(globalT, idTOk->Value.Str, &data, &err);
            data.type = VOID_T;
            symt_add(globalT, idTOk->Value.Str, data, &err);
        }
        else if (strcmp(token->Value.Str, "int") == 0)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            symt_find(globalT, idTOk->Value.Str, &data, &err);
            data.type = INT_T;
            symt_add(globalT, idTOk->Value.Str, data, &err);
            GEN_FUNC_RET_VAR_DEF;
        }
        else if (strcmp(token->Value.Str, "float") == 0)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            symt_find(globalT, idTOk->Value.Str, &data, &err);
            data.type = FLOAT_T;
            symt_add(globalT, idTOk->Value.Str, data, &err);
            GEN_FUNC_RET_VAR_DEF;
        }
        else if (strcmp(token->Value.Str, "string") == 0)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            symt_find(globalT, idTOk->Value.Str, &data, &err);
            data.type = STRING_T;
            symt_add(globalT, idTOk->Value.Str, data, &err);
            GEN_FUNC_RET_VAR_DEF;
        }
        else
        {
            fprintf(stderr, "Wrong or missing type\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else
    {
        fprintf(stderr, "Wrong or missing type\n");
        return SYNTAX_ANALYSIS_ERR;
    }
    return err;
}

/**
 * @brief Rule #16,#24-25
 *
 * @param globalT global symtable
 * @param localT local symtable
 * @param token current token
 * @param inFunction If true, if is in function
 * @return int err flag
 */
int statIf(Symtab globalT, Symtab localT, TOKEN *tokenPtr, bool inFunction, char *idFunc)
{
    int err = ALL_GOOD;
    int ifLabel = ifCount;
    ifCount++;
    TOKEN token;
    List ifL;
    list_init(&ifL);
    Data_type dat;
    if (inFunction)
    {
        err = process_expression(&ifL, localT, true, &dat);
    }
    else
    {
        err = process_expression(&ifL, globalT, true, &dat);
    }
    if (err == 999)
    {
        TOKEN retTok;
        retTok.end_state = ES_EOP2;
        (*tokenPtr) = retTok;
        return SYNTAX_ANALYSIS_ERR;
    }
    else if (err != ALL_GOOD)
        return err;
    GEN_IF_COND(ifLabel);
    token = generate_token();
    TOKCHCK(token);
    err = stat(globalT, localT, &token, inFunction, idFunc);
    if (err != ALL_GOOD)
        return err;
    if (token.end_state == ES_Cur)
    {
        token = generate_token();
        TOKCHCK(token);
        //  Rule #24-25
        if (token.end_state == ES_KEY_WORD && strcmp(token.Value.Str, "else") == 0)
        {
            GEN_JUMP_OVER_ELSE(ifLabel);
            GEN_IF_FALSE_LABEL(ifLabel);
            token = generate_token();
            TOKCHCK(token);
            if (token.end_state == ES_Cul)
            {
                token = generate_token();
                TOKCHCK(token);
                err = stat(globalT, localT, &token, inFunction, idFunc);
                if (err != ALL_GOOD)
                    return err;
                // token = generate_token();
                // TOKCHCK(token);
                if (token.end_state == ES_Cur)
                {
                    GEN_JUMP_OVER_ELSE_LABEL(ifLabel);
                    token = generate_token();
                    TOKCHCK(token);
                    err = stat(globalT, localT, &token, inFunction, idFunc);
                    (*tokenPtr) = token;
                    return err;
                }
                else
                {
                    fprintf(stderr, "Expected a '}'\n");
                    return SYNTAX_ANALYSIS_ERR;
                }
            }
        }
        else
        {
            GEN_IF_FALSE_LABEL(ifLabel);
            err = stat(globalT, localT, &token, inFunction, idFunc);
            (*tokenPtr) = token;
            return err;
        }
    }
    (*tokenPtr) = token;
    return ALL_GOOD;
}

/**
 * @brief
 *
 * @param globalT global symtab
 * @param localT local symtab
 * @param token current token
 * @param inFunction If true, if is in function
 * @return int err flag
 */
int statWhile(Symtab globalT, Symtab localT, TOKEN *tokenPtr, bool inFunction, char *idFunc)
{
    int err = ALL_GOOD;
    TOKEN token;
    int whileLabel = whileCount;
    whileCount++;
    List ifL;
    list_init(&ifL);
    Data_type dat;
    GEN_WHILE_LABEL(whileLabel);
    if (inFunction)
    {
        err = process_expression(&ifL, localT, true, &dat);
    }
    else
    {
        err = process_expression(&ifL, globalT, true, &dat);
    }

    if (err == 999)
    {
        TOKEN retTok;
        retTok.end_state = ES_EOP2;
        (*tokenPtr) = retTok;
        return SYNTAX_ANALYSIS_ERR;
    }
    else if (err != ALL_GOOD)
        return err;
    GEN_WHILE_COND(whileLabel);
    token = generate_token();
    TOKCHCK(token);
    err = stat(globalT, localT, &token, inFunction, idFunc);
    if (err != ALL_GOOD)
        return err;
    if (token.end_state == ES_Cur)
    {
        GEN_WHILE(whileLabel);
        GEN_WHILE_FALSE_LABEL(whileLabel);
        token = generate_token();
        TOKCHCK(token);
        err = stat(globalT, localT, &token, inFunction, idFunc);
        (*tokenPtr) = token;
        return err;
    }
    else
    {
        fprintf(stderr, "Expected a '}'\n");
        return SYNTAX_ANALYSIS_ERR;
    }

    (*tokenPtr) = token;
    return err;
}

/**
 * @brief
 *
 * @param globalT global symtab
 * @param token current token
 * @param idTok token with id of function
 * @return int
 */
int callFunction(Symtab globalT, Symtab localT, TOKEN *tokenPtr, TOKEN *idTok, bool inFunction, char *idVar, char *idFunc)
{
    int err = ALL_GOOD;
    // pridat z list na volanie funkcii pred def
    TOKEN token = generate_token();
    TOKCHCK(token);
    if (token.end_state == ES_Ror)
    {
        gen_func_call(idTok->Value.Str);
    }
    else
    {
        List argList;
        list_init(&argList);
        err = args(globalT, localT, &token, &argList, inFunction);
        if (err != ALL_GOOD)
            return err;

        tab_item_data data;
        INIT_SYMT_DATA(data);
        if (symt_find(globalT, idTok->Value.Str, &data, &err))
        {
            err = check_params_function(idTok->Value.Str, globalT, &argList);
            if (err != ALL_GOOD)
                return err;
        }
        else
        {
            data.def = false;
            symt_add_list(&data, argList, &err);
            symt_add(globalT, idTok->Value.Str, data, &err);
            params_data pData;
            INIT_PARAM_DATA(pData);
            pData.identifier = idTok->Value.Str;
            list_insert(&beforeDef, pData, &err);
            list_first(&beforeDef);
            if (err != ALL_GOOD)
                return err;
        }
        if (token.end_state == ES_Ror)
        {
            gen_params(data, &argList);
            list_dispose(&argList);
            gen_func_call(idTok->Value.Str);
            if (idVar != NULL)
            {
                GEN_MOVE_RET_TO_LOC(idVar);
            }
            else
            {
                token = generate_token();
                TOKCHCK(token);
                if (token.end_state == ES_Sem)
                {
                    token = generate_token();
                    TOKCHCK(token);

                    err = stat(globalT, localT, &token, inFunction, idFunc);
                }
                else
                {
                    fprintf(stderr, "Expected a ';'\n");
                    return SYNTAX_ANALYSIS_ERR;
                }
                (*tokenPtr) = token;
                return err;
            }
        }
        else if (token.end_state == ES_VID2)
        {
            fprintf(stderr, "Expected a ','\n");
            return SYNTAX_ANALYSIS_ERR;
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    return err;
}

/**
 * @brief Rule #27-28
 *
 * @param token current token
 * @param argList lis of arguments
 * @return int err flag
 */
int args(Symtab globalT, Symtab localT, TOKEN *tokenPtr, List *argList, bool inFunction)
{
    int err = ALL_GOOD;
    TOKEN token = (*tokenPtr);
    params_data paramDT;
    INIT_PARAM_DATA(paramDT);
    if (token.end_state == ES_Int)
    {
        paramDT.type = INT_T;
        paramDT.num_val = token.Value.intiger;
        list_insert(argList, paramDT, &err);
        if (err != ALL_GOOD)
            return err;
        err = argsN(globalT, localT, &token, argList, inFunction);
        (*tokenPtr) = token;
        return err;
    }
    else if (token.end_state == ES_Float)
    {
        paramDT.type = FLOAT_T;
        paramDT.num_val = token.Value.floating;
        list_insert(argList, paramDT, &err);
        if (err != ALL_GOOD)
            return err;
        err = argsN(globalT, localT, &token, argList, inFunction);
        (*tokenPtr) = token;
        return err;
    }
    else if (token.end_state == ES_String1)
    {
        paramDT.type = STRING_T;
        paramDT.str_val = token.Value.Str;
        list_insert(argList, paramDT, &err);
        if (err != ALL_GOOD)
            return err;
        err = argsN(globalT, localT, &token, argList, inFunction);
        (*tokenPtr) = token;
        return err;
    }
    else if (token.end_state == ES_VID2)
    {
        paramDT.identifier = token.Value.Str;
        tab_item_data data;
        INIT_SYMT_DATA(data);
        if (inFunction)
        {
            symt_find(localT, token.Value.Str, &data, &err);
            paramDT.type = data.type;
        }
        else
        {
            symt_find(globalT, token.Value.Str, &data, &err);
            paramDT.type = data.type;
        }
        list_insert(argList, paramDT, &err);
        if (err != ALL_GOOD)
            return err;
        err = argsN(globalT, localT, &token, argList, inFunction);
        (*tokenPtr) = token;
        return err;
    }
    return ALL_GOOD;
}

/**
 * @brief Rule #29-30
 *
 * @param token current token
 * @param argList list of arguments
 * @return int err flag
 */
int argsN(Symtab globalT, Symtab localT, TOKEN *tokenPtr, List *argList, bool inFunction)
{
    int err = ALL_GOOD;
    TOKEN token = generate_token();
    TOKCHCK(token);
    if (token.end_state == ES_Comm)
    {
        TOKEN token = generate_token();
        TOKCHCK(token);
        err = args(globalT, localT, &token, argList, inFunction);
        (*tokenPtr) = token;
        return err;
    }
    else
    {
        (*tokenPtr) = token;
        return ALL_GOOD;
    }
}

/**
 * @brief
 *
 * @param globalT global table
 * @param localT local table
 * @param tokenPtr current token
 * @param idTok ID of function
 * @param inFunction If true, if is in function
 * @param idVar variable ID
 * @return int err flag
 */
int callBIF(Symtab globalT, Symtab localT, TOKEN *tokenPtr, TOKEN *idTok, bool inFunction, char *idVar, char *inFunc)
{
    int err = ALL_GOOD;
    TOKEN token = generate_token();
    TOKCHCK(token);
    if (token.end_state != ES_Rol)
    {
        fprintf(stderr, "Expected a '('\n");
        return SYNTAX_ANALYSIS_ERR;
    }

    if (strcmp(idTok->Value.Str, "reads") == 0)
    {
        token = generate_token();
        TOKCHCK(token);
        if (token.end_state == ES_Ror)
        {
            if (idVar != NULL)
            {
                GEN_READS(idVar);
            }
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else if (strcmp(idTok->Value.Str, "readi") == 0)
    {
        token = generate_token();
        TOKCHCK(token);
        if (token.end_state == ES_Ror)
        {
            if (idVar != NULL)
            {
                GEN_READI(idVar);
            }
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else if (strcmp(idTok->Value.Str, "readf") == 0)
    {
        token = generate_token();
        TOKCHCK(token);
        if (token.end_state == ES_Ror)
        {
            if (idVar != NULL)
            {
                GEN_READF(idVar);
            }
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else if (strcmp(idTok->Value.Str, "strlen") == 0)
    {
        token = generate_token();
        TOKCHCK(token);

        List strL;
        list_init(&strL);
        err = args(globalT, localT, &token, &strL, inFunction);
        if (err != ALL_GOOD)
            return err;

        // err = check_params_function("strlen", globalT, &strL);
        if (err != ALL_GOOD)
            return err;

        if (token.end_state == ES_Ror)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            symt_find(globalT, "strlen", &data, &err);
            gen_params(data, &strL);
            gen_func_call("strlen");
            GEN_MOVE_RET_TO_LOC(idVar);
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else if (strcmp(idTok->Value.Str, "write") == 0)
    {
        token = generate_token();
        TOKCHCK(token);
        List argList;
        list_init(&argList);
        err = args(globalT, localT, &token, &argList, inFunction);
        if (err != ALL_GOOD)
            return err;
        if (token.end_state == ES_Ror)
        {
            list_first(&argList);
            // writing for every argument
            while (argList.activeElement != NULL)
            {
                if (argList.activeElement->data.type == INT_T && argList.activeElement->data.identifier == NULL)
                {
                    GEN_WRITE_CONST_INT(argList.activeElement->data.num_val);
                }
                else if (argList.activeElement->data.type == STRING_T && argList.activeElement->data.identifier == NULL)
                {
                    GEN_WRITE_CONST_STR(argList.activeElement->data.str_val);
                }
                else if (argList.activeElement->data.type == FLOAT_T && argList.activeElement->data.identifier == NULL)
                {
                    GEN_WRITE_CONST_FLOAT(argList.activeElement->data.num_val);
                }
                else if (argList.activeElement->data.identifier != NULL)
                {
                    if (inFunction)
                    {
                        err = check_defvar(localT, argList.activeElement->data.identifier);
                    }
                    else
                    {
                        err = check_defvar(globalT, argList.activeElement->data.identifier);
                    }
                    if (err != ALL_GOOD)
                    {
                        return UNDEF_VAR_ERR;
                    }
                    gen_write_var(argList.activeElement->data.identifier);
                }
                list_next(&argList);
            }
            list_dispose(&argList);
        }
        else if (token.end_state == ES_VID2 || token.end_state == ES_Int || token.end_state == ES_Float || token.end_state == ES_String1)
        {
            fprintf(stderr, "Expected a ','\n");
            return SYNTAX_ANALYSIS_ERR;
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }

        if (idVar == NULL)
        {
            token = generate_token();
            TOKCHCK(token);
            if (token.end_state == ES_Sem)
            {
                token = generate_token();
                TOKCHCK(token);
                err = stat(globalT, localT, &token, inFunction, inFunc);
                if (err != ALL_GOOD)
                    return err;
                (*tokenPtr) = token;
            }
            else
            {
                fprintf(stderr, "Expected a ';'\n");
                return SYNTAX_ANALYSIS_ERR;
            }
        }
    }
    else if (strcmp(idTok->Value.Str, "ord") == 0)
    {
        List strL;
        list_init(&strL);
        token = generate_token();
        TOKCHCK(token);
        err = args(globalT, localT, &token, &strL, inFunction);
        if (err != ALL_GOOD)
            return err;

        err = check_params_function("ord", globalT, &strL);
        if (err != ALL_GOOD)
            return err;

        if (token.end_state == ES_Ror)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            symt_find(globalT, "ord", &data, &err);
            gen_params(data, &strL);
            gen_func_call("ord");
            GEN_MOVE_RET_TO_LOC(idVar);
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else if (strcmp(idTok->Value.Str, "chr") == 0)
    {
        List strL;
        list_init(&strL);
        token = generate_token();
        TOKCHCK(token);
        err = args(globalT, localT, &token, &strL, inFunction);
        if (err != ALL_GOOD)
            return err;

        err = check_params_function("chr", globalT, &strL);
        if (err != ALL_GOOD)
            return err;

        if (token.end_state == ES_Ror)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            symt_find(globalT, "chr", &data, &err);
            gen_params(data, &strL);
            gen_func_call("chr");
            GEN_MOVE_RET_TO_LOC(idVar);
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else if (strcmp(idTok->Value.Str, "substring") == 0)
    {
        List strL;
        list_init(&strL);
        token = generate_token();
        TOKCHCK(token);
        err = args(globalT, localT, &token, &strL, inFunction);
        if (err != ALL_GOOD)
            return err;

        err = check_params_function("substring", globalT, &strL);
        if (err != ALL_GOOD)
            return err;
        if (token.end_state == ES_Ror)
        {
            tab_item_data data;
            INIT_SYMT_DATA(data);
            symt_find(globalT, "substring", &data, &err);
            gen_params(data, &strL);
            gen_func_call("substring");
            GEN_MOVE_RET_TO_LOC(idVar);
        }
        else
        {
            fprintf(stderr, "Expected a ')'\n");
            return SYNTAX_ANALYSIS_ERR;
        }
    }
    else
    {
        fprintf(stderr, "Unknown function\n");
        return SYNTAX_ANALYSIS_ERR;
    }

    (*tokenPtr) = token;
    return err;
}

/**
 * @brief
 *
 * @param globalT global symtable
 * @param localT local symtable
 * @param tokenPtr current token
 * @param token2Ptr second token
 * @param idTok ID of var
 * @param inFunction If true, if is in function
 * @return int err flag
 */
int valuesVar(Symtab globalT, Symtab localT, TOKEN *tokenPtr, TOKEN *token2Ptr, TOKEN *idTok, bool inFunction, char *idFunc)
{
    int err = ALL_GOOD;
    TOKEN token;
    if (token2Ptr->end_state == ES_FID || token2Ptr->end_state == ES_BIF)
    {
        token = (*tokenPtr);
    }
    else
    {
        token = (*token2Ptr);
    }

    tab_item_data data;
    INIT_SYMT_DATA(data);
    switch (token.end_state)
    {

    case ES_Int:
        if (inFunction)
        {
            // semantika check
            if (symt_find(localT, idTok->Value.Str, &data, &err))
            {
                data.type = INT_T;
                symt_add(localT, idTok->Value.Str, data, &err);
                GEN_VAR_MOVE_CONST_INT(idTok->Value.Str, token.Value.intiger);
            }
            else
            {
                //?
            }
        }
        else
        {
            if (symt_find(globalT, idTok->Value.Str, &data, &err))
            {
                data.type = INT_T;
                symt_add(globalT, idTok->Value.Str, data, &err);
                GEN_VAR_MOVE_CONST_INT(idTok->Value.Str, token.Value.intiger);
            }
        }
        break;
    case ES_Float:
        if (inFunction)
        {
            // semantika check
            if (symt_find(localT, idTok->Value.Str, &data, &err))
            {
                data.type = FLOAT_T;
                symt_add(localT, idTok->Value.Str, data, &err);
                GEN_VAR_MOVE_CONST_FLOAT(idTok->Value.Str, token.Value.floating);
            }
            else
            {
                //?
            }
        }
        else
        {
            if (symt_find(globalT, idTok->Value.Str, &data, &err))
            {
                data.type = FLOAT_T;
                symt_add(globalT, idTok->Value.Str, data, &err);
                GEN_VAR_MOVE_CONST_FLOAT(idTok->Value.Str, token.Value.floating);
            }
        }
        break;
    case ES_String1:
        // semantika check
        if (inFunction)
        {
            if (symt_find(localT, idTok->Value.Str, &data, &err))
            {
                data.type = STRING_T;
                symt_add(localT, idTok->Value.Str, data, &err);
                GEN_VAR_MOVE_CONST_STR(idTok->Value.Str, token.Value.Str);
            }
            else
            {
                //?
            }
        }
        else
        {
            if (symt_find(globalT, idTok->Value.Str, &data, &err))
            {
                data.type = STRING_T;
                symt_add(globalT, idTok->Value.Str, data, &err);
                GEN_VAR_MOVE_CONST_STR(idTok->Value.Str, token.Value.Str);
            }
            else
            {
                // nevim
            }
        }
        break;
    case ES_VID2:
        // semantic check
        if (inFunction)
        {
            if (symt_find(localT, token.Value.Str, &data, &err))
            {
                if (data.type == INT_T || data.type == FLOAT_T || data.type == STRING_T)
                {
                    tab_item_data secData;
                    INIT_SYMT_DATA(secData);
                    symt_find(localT, idTok->Value.Str, &data, &err);
                    secData.type = data.type;
                    symt_add(localT, idTok->Value.Str, secData, &err);
                }
                GEN_MOVE_LOCAL_VARS(idTok->Value.Str, token.Value.Str);
            }
            else
            {
                //?
            }
        }
        else
        {
            if (symt_find(globalT, token.Value.Str, &data, &err))
            {
                if (data.type == INT_T || data.type == FLOAT_T || data.type == STRING_T)
                {
                    tab_item_data secData;
                    INIT_SYMT_DATA(secData);
                    symt_find(globalT, idTok->Value.Str, &data, &err);
                    secData.type = data.type;
                    symt_add(globalT, idTok->Value.Str, secData, &err);
                }
                GEN_MOVE_LOCAL_VARS(idTok->Value.Str, token.Value.Str);
                return ALL_GOOD;
            }
        }
        break;
    case ES_FID:;
        TOKEN funTok = token;
        token = generate_token();
        TOKCHCK(token);
        if (token.end_state == ES_Rol)
        {
            err = callFunction(globalT, localT, &token, &funTok, inFunction, idTok->Value.Str, idFunc);
            tab_item_data data;
            INIT_SYMT_DATA(data);
            if (symt_find(globalT, funTok.Value.Str, &data, &err))
            {
                tab_item_data varData;
                INIT_SYMT_DATA(varData);
                if (inFunction)
                {
                    symt_find(localT, idTok->Value.Str, &varData, &err);
                    varData.type = data.type;
                    symt_add(localT, idTok->Value.Str, varData, &err);
                }
                else
                {
                    symt_find(globalT, idTok->Value.Str, &varData, &err);
                    varData.type = data.type;
                    symt_add(globalT, idTok->Value.Str, varData, &err);
                }
            }
        }
        else
        {
            fprintf(stderr, "Expected a '('\n");
            return SYNTAX_ANALYSIS_ERR;
        }
        break;
    case ES_BIF:;
        TOKEN bifTok = token;
        err = callBIF(globalT, localT, &token, &bifTok, inFunction, idTok->Value.Str, idFunc);
        (*tokenPtr) = token;
        break;
    default:
        break;
    }
    return err;
}

/**
 * @brief Function loads built in functions into symtable
 *
 * @param globalT global symtable
 * @return int err flag
 */
int loadBuiltin(Symtab globalT)
{
    int err = ALL_GOOD;
    // strlen
    tab_item_data strDat;
    INIT_SYMT_DATA(strDat);
    List strL;
    list_init(&strL);
    params_data pData;
    INIT_PARAM_DATA(pData);
    pData.type = STRING_T;
    list_insert(&strL, pData, &err);
    symt_add_list(&strDat, strL, &err);
    list_dispose(&strL);
    strDat.def = true;
    strDat.type = INT_T;
    symt_add(globalT, "strlen", strDat, &err);

    // substring
    tab_item_data subStrDat;
    INIT_SYMT_DATA(subStrDat);
    List subStrL;
    list_init(&subStrL);
    params_data subD1;
    INIT_PARAM_DATA(subD1);
    subD1.type = STRING_T;
    list_insert(&subStrL, subD1, &err);
    params_data subD2;
    INIT_PARAM_DATA(subD2);
    subD2.type = INT_T;
    list_insert(&subStrL, subD2, &err);
    params_data subD3;
    INIT_PARAM_DATA(subD3);
    subD1.type = INT_T;
    list_insert(&subStrL, subD3, &err);
    subStrDat.def = true;
    subStrDat.type = INT_T;
    symt_add_list(&subStrDat, subStrL, &err);
    list_dispose(&subStrL);
    symt_add(globalT, "substring", subStrDat, &err);

    // ord
    tab_item_data ordDat;
    INIT_SYMT_DATA(ordDat);
    List ordL;
    list_init(&ordL);
    params_data ordP;
    INIT_PARAM_DATA(ordP);
    ordP.type = STRING_T;
    list_insert(&ordL, ordP, &err);
    ordDat.def = true;
    ordDat.type = INT_T;
    symt_add_list(&ordDat, ordL, &err);
    list_dispose(&ordL);
    symt_add(globalT, "ord", ordDat, &err);

    // chr
    tab_item_data chrDat;
    INIT_SYMT_DATA(chrDat);
    List chrL;
    list_init(&chrL);
    params_data chrP;
    INIT_PARAM_DATA(chrP);
    chrP.type = INT_T;
    list_insert(&chrL, chrP, &err);
    chrDat.def = true;
    chrDat.type = STRING_T;
    symt_add_list(&chrDat, chrL, &err);
    list_dispose(&chrL);
    symt_add(globalT, "chr", chrDat, &err);

    return err;
}