/*
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file main.c
 * @brief File containing main function of the compiler
 *
 * @author Ivan Mahút (xmahut01)
 */

#include "returncodes.h"
#include "symtable.h"
#include "parser.h"
#include "generator.h"

/*
 * @brief Main function of the compiler
 * @return 0 if no errors occured
 */
int main()
{
    int err;
    Symtab globalT;
    symt_init(globalT, &err);
    if (err != ALL_GOOD)
        return err;
    Symtab localT;
    symt_init(localT, &err);
    if (err != ALL_GOOD)
        return err;
    err = parse(globalT, localT);
    GEN_EXIT(err);
    symt_free(globalT, &err);
    symt_free(localT, &err);

    return err;
}
