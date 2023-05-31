/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file precedence.c
 * @brief kratky popis daneho suboru
 *
 * @author Matúš Ďurica (xduric06)
 * @author Gabriela Paganíková (xpagan00)
 */

#include "precedence.h"
#include "scanner.h"
#include "stack.h"
#include "generator.h"

int prec_table[8][8] =
    {
        //|+ - .|* /|(|)|=== !==|< <= > >=|i|$|
        {BI, SM, SM, BI, BI, BI, SM, BI}, // + - .
        {BI, BI, SM, BI, BI, BI, SM, BI}, // * /
        {SM, SM, SM, EQ, SM, SM, SM, EM}, // (
        {BI, BI, EM, BI, BI, BI, EM, BI}, // )
        {SM, SM, SM, BI, BI, SM, SM, BI}, // === !==
        {SM, SM, SM, BI, BI, BI, SM, BI}, // < <= > >=
        {BI, BI, EM, BI, BI, BI, EM, BI}, // i -> INT STRING, FLOAT
        {SM, SM, SM, EM, SM, SM, SM, EM}, // $
};

bool concat_called = false;
bool is_float = false;
bool is_int = false;
bool is_string = false;
int str_cnt = 0;
bool out_bool = false;

End_States state;
/**
 * @brief Get the precedence value of a token
 *
 * @param token Token to be converted
 * @return prec_val Returns number
 */
static int get_prec_val(TOKEN token)
{
    switch (token.end_state)
    {
    case (ES_Add):
    case (ES_Sub):
    case (ES_Conc):
        return _PLUS_MINUS_CONCAT;
    case (ES_Mul):
    case (ES_Div):
        return _MUL_DIV;
    case (ES_Rol):
        return _L_PAR;
    case (ES_Ror):
        return _R_PAR;
    case (ES_Equ2):
    case (ES_Notequ2):
        return _EQ_NEQ;
    case (ES_Less1):
    case (ES_Less):
    case (ES_Great1):
    case (ES_Great):
        return _L_LEQ_G_GEQ;
    case (ES_VID2):
    case (ES_Int):
    case (ES_Float):
    case (ES_String1):
        return _DATA;
    case (ES_Sem):
    case (ES_Cul):
        return _DOLLAR;
    default:
        fprintf(stderr, "Wrong operand\n");
        return -1;
    }
}

/**
 * @brief Gets the closest to stack top term
 *
 * @param stack Pointer to the stack
 * @return struct stack_item Returns the stack item struct
 */
static struct stack_item stack_get_term(Precstack *stack)
{
    stack_item_ptr tmp;
    struct stack_item ret;

    for (tmp = stack->top; tmp != NULL;)
    {
        if (tmp->prec_val < 8)
        {
            ret = *tmp;
            break;
        }
        tmp = tmp->next;
    }
    return ret;
}

/**
 * @brief Counts the number of items above the handle <
 *
 * @param stack Pointer to the stack
 * @return int Returns the number of items above the handle
 */
static int items_above_handle(Precstack *stack)
{
    stack_item_ptr current = stack->top;
    int cnt = 0;
    while (current->prec_val != _HANDLE)
    {
        cnt++;
        current = current->next;
    }
    return cnt;
}

/**
 * @brief Reduces expression by rules
 *
 * @param stack Pointer to the stack
 * @return prec_rules Returns which rule was used
 */
static prec_rules reduce(Precstack *stack, Symtab tab)
{
    int err = 0;
    stack_item_ptr ret = stack->top;
    int i = items_above_handle(stack);
    prec_rules rule = _RULE_NOT_A_RULE;
    Precstack aux;
    stack_init(&aux, &err);
    if (err)
    {
        RESET_FLAGS;
        stack_free(stack);
        return err;
    }
    if (i == 1)
    {
        if (ret->prec_val == _DATA)
        {
            if (ret->token.end_state == ES_VID2)
            {
                tab_item_data ret_data;
                if (symt_find(tab, ret->token.Value.Str, &ret_data, &err))
                {
                    if (err)
                    {
                        err = UNDEF_VAR_ERR;
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    switch (ret_data.type)
                    {
                    case INT_T:
                        INT2FLOATS;
                        is_int = true;
                        state = ES_Float;
                        break;
                    case FLOAT_T:
                        is_float = true;
                        state = ES_Float;
                        break;
                    case STRING_T:
                        is_string = true;
                        state = ES_String1;
                        break;
                    default:
                        free(ret->token.Value.Str);
                        stack_free(stack);
                        err = TYPE_COMP_ERR;
                        concat_called = false;
                        is_float = false;
                        is_int = false;
                        is_string = false;
                        out_bool = false;
                        return err;
                        break;
                    }
                    free(ret->token.Value.Str);
                }
            }
            else
            {
                state = ret->token.end_state;
                concat_called = true;
            }
            rule = _RULE_I;
            stack_pop_out(stack);
            ret = stack_peek(stack);
            if (ret->prec_val == _HANDLE)
            {
                TOKEN foo;
                foo.end_state = state;
                stack_pop_out(stack);
                stack_push(stack, foo, _NONTERM, &err);
                if (err)
                {
                    RESET_FLAGS;
                    stack_free(stack);
                    return err;
                }
                concat_called = true;

                return rule;
            }
        }
        else
        {
            ret = stack_peek(stack);
            free(ret->token.Value.Str);
            return rule;
        }
    }
    else if (i == 3)
    {
        if (ret->prec_val == _R_PAR && ret->next->prec_val == _NONTERM && ret->next->next->prec_val == _L_PAR)
        {
            stack_pop_out(stack);
            ret = stack_peek(stack);
            if (ret->token.end_state == ES_String1)
            {
                stack_pop_out(stack);
                ret = stack_peek(stack);
                free(ret->token.Value.Str);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    rule = _RULE_BRACKETS;
                    return rule;
                }
            }
            else
            {
                stack_pop_out(stack);
                ret = stack_peek(stack);
                free(ret->token.Value.Str);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    rule = _RULE_BRACKETS;
                    return rule;
                }
            }
        }
        else if (ret->prec_val == _NONTERM && ret->next->next->prec_val == _NONTERM)
        {
            ret = stack_peek(stack);
            stack_pop_out(stack);
            ret = stack_peek(stack);
            free(ret->token.Value.Str);
            switch (ret->token.end_state)
            {
            case ES_Add:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    ADDS;
                    rule = _RULE_PLUS_MINUS_CONCAT;
                    return rule;
                }
                break;
            case ES_Sub:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    SUBS;
                    rule = _RULE_PLUS_MINUS_CONCAT;
                    return rule;
                }
                break;
            case ES_Conc:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    if (concat_called)
                        GEN_CONCAT;
                    rule = _RULE_PLUS_MINUS_CONCAT;
                    return rule;
                }
                break;
            case ES_Mul:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    MULS;
                    rule = _RULE_MUL_DIV;
                    return rule;
                }
                break;
            case ES_Div:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    DIVS;
                    rule = _RULE_MUL_DIV;
                    return rule;
                }
                break;
            case ES_Less:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    LTS;
                    out_bool = true;
                    rule = _RULE_L_LEQ_G_GEQ;
                    return rule;
                }
                break;
            case ES_Great:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    GTS;
                    out_bool = true;
                    rule = _RULE_L_LEQ_G_GEQ;
                    return rule;
                }
                break;
            case ES_Great1:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    LTS;
                    NOTS;
                    out_bool = true;
                    rule = _RULE_L_LEQ_G_GEQ;
                    return rule;
                }
                break;
            case ES_Less1:
                stack_pop_out(stack);
                stack_pop_out(stack);
                ret = stack_peek(stack);
                if (ret->prec_val == _HANDLE)
                {
                    TOKEN foo;
                    foo.end_state = state;
                    stack_pop_out(stack);
                    stack_push(stack, foo, _NONTERM, &err);
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(stack);
                        return err;
                    }
                    GTS;
                    NOTS;
                    out_bool = true;
                    rule = _RULE_L_LEQ_G_GEQ;
                    return rule;
                }
                break;
            case ES_Equ2:
                stack_pop_out(stack);
                if (!is_string)
                {
                    stack_pop_out(stack);
                    ret = stack_peek(stack);
                    if (ret->prec_val == _HANDLE)
                    {
                        TOKEN foo;
                        foo.end_state = state;
                        stack_pop_out(stack);
                        stack_push(stack, foo, _NONTERM, &err);
                        if (err)
                        {
                            RESET_FLAGS;
                            stack_free(stack);
                            return err;
                        }
                        EQS;
                        out_bool = true;
                        rule = _RULE_EQ_NEQ;
                        return rule;
                    }
                }
                else
                {
                    stack_pop_out(stack);
                    ret = stack_peek(stack);
                    if (ret->prec_val == _HANDLE)
                    {
                        TOKEN foo;
                        foo.end_state = state;
                        stack_pop_out(stack);
                        stack_push(stack, foo, _NONTERM, &err);
                        if (err)
                        {
                            RESET_FLAGS;
                            stack_free(stack);
                            return err;
                        }
                        GEN_EQ_STR;
                        out_bool = true;
                        rule = _RULE_EQ_NEQ;
                        return rule;
                    }
                }
                break;
            case ES_Notequ2:
                stack_pop_out(stack);
                if (!is_string)
                {
                    stack_pop_out(stack);
                    ret = stack_peek(stack);
                    if (ret->prec_val == _HANDLE)
                    {
                        TOKEN foo;
                        foo.end_state = state;
                        stack_pop_out(stack);
                        stack_push(stack, foo, _NONTERM, &err);
                        if (err)
                        {
                            RESET_FLAGS;
                            stack_free(stack);
                            return err;
                        }
                        EQS;
                        NOTS;
                        out_bool = true;
                        rule = _RULE_EQ_NEQ;
                        return rule;
                    }
                }
                else
                {
                    stack_pop_out(stack);
                    ret = stack_peek(stack);
                    if (ret->prec_val == _HANDLE)
                    {
                        TOKEN foo;
                        foo.end_state = state;
                        stack_pop_out(stack);
                        stack_push(stack, foo, _NONTERM, &err);
                        if (err)
                        {
                            RESET_FLAGS;
                            stack_free(stack);
                            return err;
                        }
                        GEN_NEQ_STR
                        out_bool = true;
                        rule = _RULE_EQ_NEQ;
                        return rule;
                    }
                }
                break;
            default:
                stack_free(stack);
                return rule;
                break;
            }
        }
    }
    free(ret->token.Value.Str);
    stack_free(stack);
    return rule;
}

/**
 * @brief Pushes handle before i or E
 *
 * @param stack Pointer to the stack
 */
static void push_handle(Precstack *stack, int *flag)
{
    Precstack aux;
    stack_init(&aux, flag);
    if (!flag)
    {
        RESET_FLAGS;
        return;
    }
    TOKEN foo;
    for (stack_item_ptr tmp = stack->top; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->prec_val <= _NONTERM)
        {
            if (tmp->prec_val < _HANDLE)
            {
                break;
            }
            stack_push(&aux, stack->top->token, stack->top->prec_val, flag);
            if (!flag)
            {
                RESET_FLAGS;
                return;
            }
            stack_pop_out(stack);
        }
    }

    // valgrind error, dont know how to fix it, shouldnt be critical
    stack_push(stack, foo, _HANDLE, flag);
    if (!flag)
    {
        RESET_FLAGS;
        return;
    }
    while (aux.top != NULL)
    {
        stack_push(stack, aux.top->token, aux.top->prec_val, flag);
        if (!flag)
        {
            RESET_FLAGS;
            return;
        }
        stack_pop_out(&aux);
    }
}

int process_expression(List *l, Symtab tab, bool in_if, Data_type *ret_type)
{
    Precstack stack;
    int err = ALL_GOOD;
    stack_init(&stack, &err);
    if (err)
    {
        RESET_FLAGS;
        stack_free(&stack);
        return err;
    }
    struct stack_item top_item;
    stack_push_dollar(&stack);
    TOKEN on_input;
    prec_rules rule;
    bool success = false;
    int ret = ALL_GOOD;

    if (l->firstElement != NULL)
    {
        on_input = l->firstElement->data.token;
        list_remove_first(l);
    }
    else
    {
        on_input = generate_token();
    }

    while (!success)
    {
        top_item = stack_get_term(&stack);
        prec_val on_input_prec_val = get_prec_val(on_input);
        if (on_input_prec_val == (prec_val)-1 && on_input.end_state != ES_EOP2 && on_input.end_state != ES_EOF)
        {
            stack_free(&stack);
            return SYNTAX_ANALYSIS_ERR;
        }
        else if (on_input_prec_val == (prec_val)-1 && (on_input.end_state == ES_EOP2 || on_input.end_state == ES_EOF))
        {
            stack_free(&stack);
            return 999;
        }
        switch (prec_table[top_item.prec_val][on_input_prec_val])
        {
        case SM:
            push_handle(&stack, &err);
            if (err)
            {
                RESET_FLAGS;
                stack_free(&stack);
                return err;
            }
            stack_push(&stack, on_input, get_prec_val(on_input), &err);
            if (err)
            {
                RESET_FLAGS;
                stack_free(&stack);
                return err;
            }
            switch (on_input.end_state)
            {
            case ES_VID2:;
                tab_item_data ret_dat;
                if (symt_find(tab, on_input.Value.Str, &ret_dat, &err))
                {
                    if (err)
                    {
                        RESET_FLAGS;
                        stack_free(&stack);
                        err = UNDEF_VAR_ERR;
                        return err;
                    }
                    if (ret_dat.type == STRING_T)
                    {
                        if (!is_string)
                        {
                            GEN_MOVE_LOCAL_VARS("_STRING_OP_TEMP_RETURN", on_input.Value.Str);
                            str_cnt = 1;
                        }
                        else
                        {
                            GEN_MOVE_LOCAL_VARS("_STRING_OP_TEMP", on_input.Value.Str);
                        }
                        is_string = true;
                    }
                    else
                        PUSHS_VAR(on_input.Value.Str);
                }
                break;
            case ES_Int:
                if (!is_string)
                {
                    PUSHS_INT(on_input.Value.intiger);
                    is_int = true;
                }
                else
                {
                    FREE_SEMANTIC_ERR;
                    return ret;
                }
                break;
            case ES_Float:
                if (!is_string)
                {
                    PUSHS_FLOAT(on_input.Value.floating);
                    is_float = true;
                }
                else
                {
                    FREE_SEMANTIC_ERR;
                    return ret;
                }
                break;
            case ES_String1:
                if (!is_float && !is_int)
                {
                    if (concat_called == false)
                    {
                        GEN_VAR_MOVE_CONST_STR("_STRING_OP_TEMP_RETURN", on_input.Value.Str);
                        str_cnt = 1;
                    }
                    else if (concat_called == true)
                    {
                        GEN_VAR_MOVE_CONST_STR("_STRING_OP_TEMP", on_input.Value.Str);
                        str_cnt++;
                    }
                    is_string = true;
                    free(on_input.Value.Str);
                }
                else
                {
                    free(on_input.Value.Str);
                    FREE_SEMANTIC_ERR;
                    return ret;
                }
                break;
            default:
                break;
            }
            if (get_prec_val(on_input) != _DOLLAR)
            {
                if (!on_input.Value.Str)
                {
                    free(on_input.Value.Str);
                }
                if (l->firstElement != NULL)
                {
                    on_input = l->firstElement->data.token;
                    list_remove_first(l);
                }
                else
                {
                    on_input = generate_token();
                }
            }
            break;
        case EQ:
            stack_push(&stack, on_input, get_prec_val(on_input), &err);
            if (err)
            {
                RESET_FLAGS;
                stack_free(&stack);
                return err;
            }
            if (get_prec_val(on_input) != 7)
            {
                free(on_input.Value.Str);
                if (l->firstElement != NULL)
                {
                    on_input = l->firstElement->data.token;
                    list_remove_first(l);
                }
                else
                {
                    on_input = generate_token();
                }
            }
            break;
        case BI:;
            rule = reduce(&stack, tab);
            if (rule == _RULE_NOT_A_RULE)
            {
                free(on_input.Value.Str);
                FREE_SYNTAX_ERR;
                return ret;
            }
            break;
        case EM:
            if (top_item.prec_val == _DOLLAR && get_prec_val(on_input) == _DOLLAR)
            {
                // Setting return type of expression
                if (!in_if)
                {
                    if (!is_int && !is_string)
                        *ret_type = FLOAT_T;
                    else if (is_int && !is_string && !is_float)
                        *ret_type = INT_T;
                    else if (is_string && !is_float && !is_int)
                        *ret_type = STRING_T;
                }
                if ((rule == _RULE_BRACKETS && in_if) || (rule == _RULE_I && in_if))
                {
                    if (!is_float && !out_bool && !is_string)
                    {
                        FLOAT2INTS;
                    }
                    if (!is_string)
                    {
                        POPS("_BOOL_IF_RET");
                    }

                    if (!out_bool && !is_string && is_int)
                    {
                        GEN_GET_BOOL_INT_SINGLE;
                    }
                    else if (!out_bool && !is_string && is_float)
                    {
                        GEN_GET_BOOL_FLOAT_SINGLE;
                    }
                    else if (is_string && str_cnt == 1)
                    {
                        *ret_type = STRING_T;
                        GEN_GET_BOOL_STRING_SINGLE;
                    }
                    else if (is_string && concat_called)
                    {
                        *ret_type = STRING_T;
                        GEN_GET_BOOL_STRING_SINGLE;
                    }
                }
                else
                {
                    if (!is_float && !out_bool && !is_string)
                    {
                        FLOAT2INTS;
                    }
                }
                // RESET_FLAGS;
                concat_called = false;
                is_float = false;
                is_int = false;
                is_string = false;
                out_bool = false;
                str_cnt = 0;
                stack_free(&stack);
                success = true;
            }
            else
            {
                free(on_input.Value.Str);
                stack_free(&stack);
                ret = SEMANTIC_OTHER_ERR;
                return ret;
            }
            break;
        }
    }
    free(on_input.Value.Str);
    return ret;
}