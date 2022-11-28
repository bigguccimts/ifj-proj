/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file generator.h
 * @brief File containing definitions of functions for code generations
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

void gen_header();

void gen_main_label();

void gen_jump_to_main();

void gen_write_var(tab_item_data);

void gen_func_def_header_args(tab_item_data, char *);

void gen_var(char *);

void gen_params(tab_item_data, List *);

#endif