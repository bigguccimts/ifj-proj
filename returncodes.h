/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file returncodes.h
 * @brief File containing return codes for the compiler
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

// Inclusion guard
#ifndef RETURNCODES_H
#define RETURNCODES_H

#define ALL_GOOD 0            // No errors
#define LEX_ANALYSIS_ERR 1    // Lexical analysis error
#define SYNTAX_ANALYSIS_ERR 2 // Wrong code syntax, missing header etc.
#define UNDEF_FUNC_ERR 3      // Undefined function, redefinition of a function
#define UNDEF_PARAM_ERR 4     // Wrong amount/type of parameters in function call
#define UNDEF_RETURN_ERR 4    // Wrong type of return value
#define UNDEF_VAR_ERR 5       // Undefined variable
#define WRONG_RET_ERR 6       // Missing/redundant expression in return
#define TYPE_COMP_ERR 7       // Error in type compatibility in arithmetic, string and relation expressions
#define SEMANTIC_OTHER_ERR 8  // Other semantic errors
#define INTERNAL_ERR 99       // Internal error ie. memory allocation error

#endif // RETURNCODES_H
