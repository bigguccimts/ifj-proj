/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file generator.h
 * @brief File containing definitions of functions and macros for code generations
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "symtable.h"
#include "list.h"
#include "datatypes.h"

/**
 * @brief Macro for generating aux variable for return from a function and initializing it to nil
 *
 */
#define GEN_FUNC_RET_VAR_DEF \
    gen_var_def("_retval");  \
    printf("MOVE LF@_retval nil@nil\n");

/**
 * @brief Macro for generating MOVE instruction between two variables in the LF
 *
 * @param dest Destination variable identificator
 * @param src Source variable identificator
 */
#define GEN_MOVE_LOCAL_VARS(dest, src) \
    gen_var_move_var("LF", dest, "LF", src);

/**
 * @brief Macro for generating MOVE instruction from returning value in TF to variable in LF
 *
 * @param dest Destination variable identificator
 */
#define GEN_MOVE_RET_TO_LOC(dest) \
    gen_var_move_var("LF", dest, "TF", "_retval");

/**
 * @brief Macro for generating MOVE instruction from return variable in IFJ22 to aux returning variable in function
 *
 * @param src Source variable identificator
 */
#define GEN_MOVE_VAR_TO_RET(src) \
    gen_var_move_var("LF", "_retval", "LF", src);

/**
 * @brief Macro generating PUSHFRAME instruction
 *
 */
#define PUSHFRAME \
    printf("PUSHFRAME\n");

/**
 * @brief Macro generating CREATEFRAME instruction
 *
 */
#define CREATEFRAME \
    printf("CREATEFRAME\n");

/**
 * @brief Macro generating POPFRAME instruction
 *
 */
#define POPFRAME \
    printf("POPFRAME\n");

/**
 * @brief Macro generating RETURN instruction
 *
 */
#define RETURN \
    printf("RETURN\n");

/**
 * @brief Macro printing constant string
 *
 * @param str String value
 */
#define GEN_WRITE_CONST_STR(str_val) \
    gen_write_const(STRING_T, str_val, 0);

/**
 * @brief Macro printing constant int
 *
 * @param num Int value
 */
#define GEN_WRITE_CONST_INT(num_val) \
    gen_write_const(INT_T, NULL, num_val);

/**
 * @brief Macro printing constant float
 *
 * @param num Float value
 */
#define GEN_WRITE_CONST_FLOAT(num_val) \
    gen_write_const(FLOAT_T, NULL, num_val);

/**
 * @brief Generates label of a function with no arguments and creates local frame
 *
 * @param id Identificator of the function
 */
#define GEN_FUNC_DEF_HEADER_NOARGS(id)           \
    _GEN_SKIP(id)                                \
    printf("# Definition of function %s\n", id); \
    printf("LABEL $%s\n", id);                   \
    CREATEFRAME;                                 \
    PUSHFRAME;

#define _GEN_SKIP(id)                                                                                   \
    printf("# Skip of definition of function %s,\n# if defined in the middle of the IFJ22 code\n", id); \
    printf("JUMP $_SKIP_DEF_%s\n", id);
/**
 * @brief Generates MOVE instruction to a local variable from a string constant
 *
 * @param id Identificator of the variable
 * @param str_val String value
 */
#define GEN_VAR_MOVE_CONST_STR(id, str_val) \
    gen_var_move_const(id, STRING_T, 0, str_val);

/**
 * @brief Generates MOVE instruction to a local variable from a int constant
 *
 * @param id Identificator of the variable
 * @param num_val Int value
 */
#define GEN_VAR_MOVE_CONST_INT(id, num_val) \
    gen_var_move_const(id, INT_T, num_val, NULL);

/**
 * @brief Generates MOVE instruction to a local variable from a float constant
 *
 * @param id Identificator of the variable
 * @param num_val Int value
 */
#define GEN_VAR_MOVE_CONST_FLOAT(id, num_val) \
    gen_var_move_const(id, FLOAT_T, num_val, NULL);

/**
 * @brief Generates PUSHS with variable
 *
 * @param id Identificator of the variable
 */
#define PUSHS_VAR(id) \
    printf("PUSHS LF@%s\n", id);

/**
 * @brief Generates POPS instruction to a variable
 *
 * @param id Identificator of the variable
 */
#define POPS(id) \
    printf("POPS LF@%s\n", id);

/**
 * @brief Generates ADDS function
 *
 */
#define ADDS \
    printf("ADDS\n");

/**
 * @brief Generates SUBS function
 *
 */
#define SUBS \
    printf("SUBS\n");

/**
 * @brief Generates MULS function
 *
 */
#define MULS \
    printf("MULS\n");

/**
 * @brief Generates DIVS function
 *
 */
#define DIVS \
    printf("DIVS\n");

/**
 * @brief Generates IDIVS function
 *
 */
#define IDIVS \
    printf("IDIVS\n");

/**
 * @brief Generates CLEARS function
 *
 */
#define CLEARS \
    printf("CLEARS\n");

/**
 * @brief Generates LTS function (<)
 *
 */
#define LTS \
    printf("LTS\n");

/**
 * @brief Generates GTS function (>)
 *
 */
#define GTS \
    printf("GTS\n");

/**
 * @brief Generates EQS function (==)
 *
 */
#define EQS \
    printf("EQS\n");

/**
 * @brief Generates EQS function (==)
 *
 * @param id LABEL name
 */
#define JUMP(id) \
    printf("JUMP %s\n", id);

/**
 * @brief Generates JUMPIFEQS function (jump if equal)
 *
 */
#define JUMPIFEQS \
    printf("JUMPIFEQS\n");

/**
 * @brief Generates JUMPIFNEQS function (jump if not equal)
 *
 */
#define JUMPIFNEQS \
    printf("JUMPIFNEQS\n");

#define STRLEN(dest, src) \
    printf("STRLEN %s %s\n", dest, src);

#define GEN_READS(id) \
    printf("READ LF@%s string\n", id);

#define GEN_READI(id) \
    printf("READ LF@%s int\n", id);

#define GEN_READF(id) \
    printf("READ LF@%s float\n", id);

#define GEN_EXIT(code) \
    printf("EXIT int@%d\n", code);

#define GEN_BUILT_IN_ORD                                          \
    printf("# Definition of built in function ord(string $c)\n"); \
    printf("LABEL $ord\n");                                       \
    printf("PUSHFRAME\n");                                        \
    printf("DEFVAR LF@$c\n");                                     \
    printf("MOVE LF@$c LF@_1\n");                                 \
    printf("DEFVAR LF@_retval\n");                                \
    printf("MOVE LF@_retval LF@$c\n");                            \
    printf("JUMPIFEQ $_ord_empty LF@_retval string@\n");          \
    printf("STRI2INT LF@_retval LF@_retval int@0\n");             \
    printf("POPFRAME\n");                                         \
    printf("RETURN\n");                                           \
    printf("LABEL $_ord_empty\n");                                \
    printf("MOVE LF@_retval int@0\n");                            \
    printf("POPFRAME\n");                                         \
    printf("RETURN\n");                                           \
    printf("# End of definition of built in function ord(string $c)\n\n");

#define GEN_BUILT_IN_CHR                                       \
    printf("# Definition of built in function chr(int $i)\n"); \
    printf("LABEL $chr\n");                                    \
    printf("PUSHFRAME\n");                                     \
    printf("DEFVAR LF@$i\n");                                  \
    printf("MOVE LF@$i LF@_1\n");                              \
    printf("DEFVAR LF@_retval\n");                             \
    printf("INT2CHAR LF@_retval LF@$i\n");                     \
    printf("POPFRAME\n");                                      \
    printf("RETURN\n");                                        \
    printf("# End of definition of built in function chr(int $i)\n\n");

#define GEN_BUILT_IN_STRLEN                                          \
    printf("# Definition of built in function strlen(string $s)\n"); \
    printf("LABEL $strlen\n");                                       \
    printf("PUSHFRAME\n");                                           \
    printf("DEFVAR LF@$s\n");                                        \
    printf("MOVE LF@$s LF@_1\n");                                    \
    printf("DEFVAR LF@_retval\n");                                   \
    printf("STRLEN LF@_retval LF@$s\n");                             \
    printf("POPFRAME\n");                                            \
    printf("RETURN\n");                                              \
    printf("# End of definition of built in function strlen(string $s)\n\n");

#define GEN_BUILT_IN_SUBSTRING                                                          \
    printf("# Definition of built in function substring(string $s, int $i, int $j)\n"); \
    printf("LABEL $substring\n");                                                       \
    printf("PUSHFRAME\n");                                                              \
    printf("DEFVAR LF@$s\n");                                                           \
    printf("MOVE LF@$s LF@_1\n");                                                       \
    printf("DEFVAR LF@$i\n");                                                           \
    printf("MOVE LF@$i LF@_2\n");                                                       \
    printf("DEFVAR LF@$j\n");                                                           \
    printf("MOVE LF@$j LF@_3\n");                                                       \
    printf("DEFVAR LF@_retval\n");                                                      \
    printf("MOVE LF@_retval string@\n");                                                \
    printf("DEFVAR LF@_tmp\n");                                                         \
    printf("MOVE LF@_tmp string@\n");                                                   \
    printf("LABEL $_substring_while\n");                                                \
    printf("JUMPIFEQ $_substr_end LF@$i LF@$j\n");                                      \
    printf("GETCHAR LF@_tmp LF@$s LF@$i\n");                                            \
    printf("CONCAT LF@_retval LF@_retval LF@_tmp\n");                                   \
    printf("ADD LF@$i LF@$i int@1\n");                                                  \
    printf("JUMP $_substring_while\n");                                                 \
    printf("LABEL  $_substr_end\n");                                                    \
    printf("POPFRAME\n");                                                               \
    printf("RETURN\n");                                                                 \
    printf("# End of definition of built in function strlen(string $s)\n\n");

void gen_header();

void gen_write_var(char *);

void gen_write_const(Data_type, char *, float);

void gen_func_call(char *);

void gen_func_def_header_args(tab_item_data, char *);

void gen_var_move_var(char *, char *, char *, char *);

void gen_func_return();

void gen_var_def(char *);

void gen_var_move_const(char *, Data_type, float, char *);

void gen_params(tab_item_data, List *);

#endif