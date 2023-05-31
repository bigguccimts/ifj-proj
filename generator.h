/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file generator.h
 * @brief File containing definitions of functions and macros for code generations
 *
 * @author Matúš Ďurica (xduric06)
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
 * @brief Macro for generating JUMPIFNEQ instruction
 *
 * @param jump_n Number of label
 */
#define GEN_IF_COND(jump_n)               \
    printf("# Start of if %d\n", jump_n); \
    printf("JUMPIFNEQ $_IF_FALSE_%d LF@_BOOL_IF_RET bool@true\n", jump_n);

#define GEN_WHILE_COND(jump_n)                         \
    printf("# Start of while condition %d\n", jump_n); \
    printf("JUMPIFNEQ $_WHILE_FALSE_%d LF@_BOOL_IF_RET bool@true\n", jump_n);

/**
 * @brief Macro for generating LABEL so if statement can jump somewhere
 *
 * @param jump_n Number of label
 */
#define GEN_IF_FALSE_LABEL(jump_n)                       \
    printf("# Label for skipping over if %d\n", jump_n); \
    printf("LABEL $_IF_FALSE_%d\n", jump_n);             \
    printf("# End of if %d\n", jump_n);

#define GEN_WHILE_FALSE_LABEL(jump_n)                       \
    printf("# Label for skipping over while %d\n", jump_n); \
    printf("LABEL $_WHILE_FALSE_%d\n", jump_n);             \
    printf("# End of while %d\n", jump_n);

/**
 * @brief Macro for generating JUMP instruction so else can be skipped
 *
 * @param jump_n Number of label
 */
#define GEN_JUMP_OVER_ELSE(jump_n) \
    printf("JUMP $_SKIP_ELSE_%d\n", jump_n);

/**
 * @brief Macro for generating LABEL so else can be skipped
 *
 * @param jump_n Number of label
 */
#define GEN_JUMP_OVER_ELSE_LABEL(jump_n)                   \
    printf("# Label for skipping over else %d\n", jump_n); \
    printf("LABEL $_SKIP_ELSE_%d\n", jump_n);              \
    printf("# End of else %d\n", jump_n);

/**
 * @brief Macro for generating JUMP for while cycle
 *
 * @param jump_n Number of label
 */
#define GEN_WHILE(jump_n)                            \
    printf("# Jump to start of while %d\n", jump_n); \
    printf("JUMP $_WHILE_%d\n", jump_n);             \
    printf("# End of while %d\n", jump_n);

/**
 * @brief Macro for generating LABEL for while cycle
 *
 * @param jump_n Number of label
 */
#define GEN_WHILE_LABEL(jump_n)              \
    printf("# Start of while %d\n", jump_n); \
    printf("LABEL $_WHILE_%d\n", jump_n);

/**
 * @brief Generating code to compare single int in statement
 *
 */
#define GEN_GET_BOOL_INT_SINGLE                           \
    printf("EQ LF@_BOOL_IF_RET LF@_BOOL_IF_RET int@0\n"); \
    printf("NOT LF@_BOOL_IF_RET LF@_BOOL_IF_RET\n");

/**
 * @brief Generating code to compare single float in statement
 *
 */
#define GEN_GET_BOOL_FLOAT_SINGLE                                \
    printf("EQ LF@_BOOL_IF_RET LF@_BOOL_IF_RET float@0x0p+0\n"); \
    printf("NOT LF@_BOOL_IF_RET LF@_BOOL_IF_RET\n");

/**
 * @brief Generating code to compare single string in statement
 *
 */
#define GEN_GET_BOOL_STRING_SINGLE                                 \
    GEN_MOVE_LOCAL_VARS("_BOOL_IF_RET", "_STRING_OP_TEMP_RETURN"); \
    printf("EQ LF@_BOOL_IF_RET LF@_BOOL_IF_RET string@\n");        \
    printf("NOT LF@_BOOL_IF_RET LF@_BOOL_IF_RET\n");

/**
 * @brief Generating code to compare 2 strings with EQ instruction
 *
 */
#define GEN_EQ_STR                                                                         \
    printf("EQ LF@_STRING_OP_TEMP_RETURN LF@_STRING_OP_TEMP_RETURN LF@_STRING_OP_TEMP\n"); \
    GEN_MOVE_LOCAL_VARS("_BOOL_IF_RET", "_STRING_OP_TEMP_RETURN");

/**
 * @brief Generating code to compare 2 strings with NEQ instruction
 *
 */
#define GEN_NEQ_STR                                                                        \
    printf("EQ LF@_STRING_OP_TEMP_RETURN LF@_STRING_OP_TEMP_RETURN LF@_STRING_OP_TEMP\n"); \
    printf("NOT LF@_STRING_OP_TEMP_RETURN LF@_STRING_OP_TEMP_RETURN\n");                   \
    GEN_MOVE_LOCAL_VARS("_BOOL_IF_RET", "_STRING_OP_TEMP_RETURN");

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

#define GEN_WRITE_NULL \
    printf("WRITE nil@nil\n");

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
    GEN_SKIP(id)                                 \
    printf("# Definition of function %s\n", id); \
    printf("LABEL $%s\n", id);                   \
    CREATEFRAME;                                 \
    PUSHFRAME;                                   \
    gen_var_def("_BOOL_IF_RET");                 \
    gen_var_def("_STRING_OP_TEMP_RETURN");       \
    gen_var_def("_STRING_OP_TEMP");

#define GEN_SKIP(id)                                                                                    \
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

#define PUSHS_INT(id) \
    printf("PUSHS float@%a\n", (float)id);

#define PUSHS_FLOAT(id) \
    printf("PUSHS float@%a\n", id);

/**
 * @brief Generates POPS instruction to a variable
 *
 * @param id Identificator of the variable
 */
#define POPS(id) \
    printf("POPS LF@%s\n", id);

/**
 * @brief Generates ADDS instruction
 *
 */
#define ADDS \
    printf("ADDS\n");

/**
 * @brief Generates SUBS instruction
 *
 */
#define SUBS \
    printf("SUBS\n");

/**
 * @brief Generates MULS instruction
 *
 */
#define MULS \
    printf("MULS\n");

/**
 * @brief Generates DIVS instruction
 *
 */
#define DIVS \
    printf("DIVS\n");

/**
 * @brief Generates IDIVS instruction
 *
 */
#define IDIVS \
    printf("IDIVS\n");

/**
 * @brief Generates CLEARS instruction
 *
 */
#define CLEARS \
    printf("CLEARS\n");

/**
 * @brief Generates LTS instruction (<)
 *
 */
#define LTS \
    printf("LTS\n");

/**
 * @brief Generates GTS instruction (>)
 *
 */
#define GTS \
    printf("GTS\n");

/**
 * @brief Generates NOTS instruction (!)
 *
 */
#define NOTS \
    printf("NOTS\n");

/**
 * @brief Generates EQS instruction (==)
 *
 */
#define EQS \
    printf("EQS\n");

/**
 * @brief Generates FLOAT2INTS instruction
 *
 */
#define FLOAT2INTS \
    printf("FLOAT2INTS\n");

/**
 * @brief Generates INT2FLOATS instruction
 *
 */
#define INT2FLOATS \
    printf("INT2FLOATS\n");

/**
 * @brief Generates CONCAT instruction between 2 strings
 *
 */
#define GEN_CONCAT \
    printf("CONCAT LF@_STRING_OP_TEMP_RETURN LF@_STRING_OP_TEMP_RETURN LF@_STRING_OP_TEMP\n");

/**
 * @brief Generates EQS instruction (==)
 *
 * @param id LABEL name
 */
#define JUMP(id) \
    printf("JUMP %s\n", id);

/**
 * @brief Generates JUMPIFEQS instruction (jump if equal)
 *
 */
#define JUMPIFEQS \
    printf("JUMPIFEQS\n");

/**
 * @brief Generates JUMPIFNEQS instruction (jump if not equal)
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

#define GEN_BUILT_IN_FLOATVAL                                     \
    printf("# Definition of built in function floatval(term)\n"); \
    printf("LABEL $floatval\n");                                  \
    printf("PUSHFRAME\n");                                        \
    printf("DEFVAR LF@term \n");                                  \
    printf(" MOVE LF@term LF@_1  \n");                            \
    printf("DEFVAR LF@_retval\n");                                \
    printf("DEFVAR LF@$_TYPE\n");                                 \
    printf("TYPE LF@$_TYPE LF@term\n");                           \
    printf("JUMPIFEQ $_TERM_FLOAT LF@$_TYPE string@float\n");     \
    printf("INT2FLOAT LF@_retval LF@term\n");                     \
    printf("POPFRAME\n");                                         \
    printf("RETURN\n");                                           \
    printf("LABEL $_TERM_FLOAT\n");                               \
    printf("MOVE LF@_retval LF@term\n");                          \
    printf("POPFRAME\n");                                         \
    printf("RETURN\n");                                           \
    printf("# End of definition of built in function floatval(term)\n\n");

#define GEN_BUILT_IN_INTVAL                                     \
    printf("# Definition of built in function intval(term)\n"); \
    printf("LABEL $intval\n");                                  \
    printf("PUSHFRAME\n");                                      \
    printf("DEFVAR LF@term\n");                                 \
    printf("MOVE LF@term LF@_1\n");                             \
    printf("DEFVAR LF@_retval\n");                              \
    printf("DEFVAR LF@$_TYPE\n");                               \
    printf("TYPE LF@$_TYPE LF@term\n");                         \
    printf("JUMPIFEQ $_TERM_INT LF@$_TYPE string@int\n");       \
    printf("FLOAT2INT LF@_retval LF@term\n");                   \
    printf("POPFRAME\n");                                       \
    printf("RETURN\n");                                         \
    printf("LABEL $_TERM_INT\n");                               \
    printf("MOVE LF@_retval LF@term\n");                        \
    printf("POPFRAME\n");                                       \
    printf("RETURN\n");                                         \
    printf("# End of definition of built in function intval(term)\n\n");

#define GEN_BUILT_IN_STRVAL                                      \
    printf("# Definition of built in function strval(term)\n");  \
    printf("LABEL $strval\n");                                   \
    printf("PUSHFRAME\n");                                       \
    printf("DEFVAR LF@term\n");                                  \
    printf("MOVE LF@term LF@_1\n");                              \
    printf("DEFVAR LF@_retval\n");                               \
    printf("DEFVAR LF@$_TYPE\n");                                \
    printf("TYPE LF@$_TYPE LF@term\n");                          \
    printf("JUMPIFNEQ $_TERM_STRING LF@$_TYPE string@string\n"); \
    printf("MOVE LF@_retval LF@term\n");                         \
    printf("POPFRAME\n");                                        \
    printf("RETURN\n");                                          \
    printf("LABEL $_TERM_STRING\n");                             \
    printf("POPFRAME\n");                                        \
    printf("RETURN\n");                                          \
    printf("# End of definition of built in function strval(term)\n\n");

void gen_header();

void gen_write_var(char *);

void gen_const(Data_type, char *, float);

void gen_write_const(Data_type, char *, float);

void gen_func_call(char *);

void gen_func_def_header_args(tab_item_data, char *);

void gen_var_move_var(char *, char *, char *, char *);

void gen_func_return();

void gen_var_def(char *);

void gen_var_move_const(char *, Data_type, float, char *);

void gen_params(tab_item_data, List *);

#endif