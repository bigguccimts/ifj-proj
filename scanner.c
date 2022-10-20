/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file subor.c
 * @brief kratky popis daneho suboru
 *
 * @author Matúš Ďurica (xduric06)
 * @author Ivan Mahút (xmahut01)
 * @author Dušan Slúka (xsluka00)
 * @author Gabriela Paganíková (xpagan00)
 */

#include "scanner.h"
#include <ctype.h>
#include <stdio.h>

States Automat(States actual_state, int transition) {

  switch (actual_state) {
  case Start:
    if (isalpha(transition) || isalnum(transition)) {
        actual_state =  ID;
    }
    /*if (isalpha(transition) || isalnum(transition)) {
        actual_state =  ID;
    }
    if (isalpha(transition) || isalnum(transition)) {
        actual_state =  ID;
    }
    if (isalpha(transition) || isalnum(transition)) {
        actual_state =  ID;
    }*/

    break;
  case ID:
    if (isalpha(transition) || isalnum(transition)) {
        actual_state =  ID;
    } 
    
    else {
        //TO DO Create token 
    }

    break;
  case Int:

    break;

  default:
    break;
  }

  return actual_state;
}

struct TOKEN generate_token() {
  States actual_state = Start;
  // enum End_states end_state = ES_ERROR;
  int transition = getchar();
  //TO DO crete dinamicli alocated array for value for token
  while (1) {
    actual_state = Automat(actual_state, transition);
  }
  // struct TOKEN token;

  // return token;
}
