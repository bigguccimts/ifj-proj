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

States Automat(States current_state, int transition)
{

  switch (current_state)
  {
  case Start:
    if (isalnum(transition) || transition == '$' || transition == '_' || transition == '?')
    {
      current_state = ID;
    }
    else if ((transition >= '0' && transition <= '9'))
    {
      current_state = Int;
    }
    else if (transition == '+')
    {
      current_state = Add;
    }
    else if (transition == '-')
    {
      current_state = Sub;
    }
    else if (transition == '*')
    {
      current_state = Mul;
    }
    else if (transition == '/')
    {
      current_state = Div;
    }
    else if (transition == '.')
    {
      current_state = Conc;
    }
    else if (transition == '(')
    {
      current_state = Rol;
    }
    else if (transition == ')')
    {
      current_state = Ror;
    }
    else if (transition == '{')
    {
      current_state = Cul;
    }
    else if (transition == '}')
    {
      current_state = Cur;
    }
    else if (transition == '=')
    {
      current_state = Equ;
    }
    else if (transition == '<')
    {
      current_state = Less;
    }
    else if (transition == '>')
    {
      current_state = Great;
    }
    else if (transition == '"')
    {
      current_state = Equ;
    }
    else if (transition == ' ')
    {
      current_state = Start;
    }
    else
    {
      current_state = ERROR;
    }

    break;

  case ID:
    if (isalnum(transition))
    {
      current_state = ID;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Int:
    if (transition >= '0' || transition <= '9')
    {
      current_state = Int;
    }
    else if (transition == '.')
    {
      current_state = Float;
    }
    else if (transition == 'E' || transition == 'e')
    {
      current_state = Exp;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Float:
    if (transition >= '0' || transition <= '9')
    {
      current_state = Float;
    }
    else if (transition == 'E' || transition == 'e')
    {
      current_state = Exp;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Exp:
    if (transition >= '0' || transition <= '9')
    {
      current_state = Exp2;
    }
    else if (transition == '+' || transition == '-')
    {
      current_state = Exp1;
    }
    else
    {
      current_state = ERROR;
    }
    break;
  case Exp1:
    if (transition >= '0' || transition <= '9')
    {
      current_state = Exp2;
    }
    else
    {
      current_state = ERROR;
    }
    break;
  case Exp2:
    if (transition >= '0' || transition <= '9')
    {
      current_state = Exp2;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Add:
    current_state = ERROR;
    break;

  case Sub:
    current_state = ERROR;
    break;

  case Mul:
    current_state = ERROR;
    break;

  case Div:
    if (transition == '/')
    {
      current_state = Com;
    }
    else if (transition == '*')
    {
      current_state = Com2;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Com:
    if (transition >= 32 && transition <= 126)
    {
      current_state = Com;
    }
    else if (transition >= '\n')
    {
      current_state = Com1;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Com1:
    current_state = ERROR;
    break;

  case Com2:
    if (transition >= 32 && transition <= 126 && transition != 42)
    {
      current_state = Com2;
    }
    else if (transition >= '*')
    {
      current_state = Com3;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Com3:

    if (transition >= 32 && transition <= 126 && transition != 47)
    {
      current_state = Com2;
    }
    else if (transition >= '/')
    {
      current_state = Com4;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Com4:
    current_state = ERROR;
    break;

  case Conc:
    current_state = ERROR;
    break;

  case Rol:
    current_state = ERROR;
    break;

  case Ror:
    current_state = ERROR;
    break;

  case Cul:
    current_state = ERROR;
    break;

  case Cur:
    current_state = ERROR;
    break;

  case Equ:
    if (transition == '=')
    {
      current_state = Equ1;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Equ1:
    if (transition == '=' || transition == '!')
    {
      current_state = Equ2;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Equ2:
    current_state = ERROR;
    break;

  case Less:
    if (transition == '=')
    {
      current_state = Less1;
    }
    break;

  case Less1:
    current_state = ERROR;
    break;

  case Great:
    if (transition == '=')
    {
      current_state = Great1;
    }
    break;

  case Great1:
    current_state = ERROR;
    break;

  case String:
    if (transition >= 32 && transition <= 126 && transition != 34 && transition != 92)
    {
      current_state = String;
    }
    else if (transition == 34)
    {
      current_state = String1;
    }
    else if (transition == 92)
    {
      current_state = Esc;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case String1:
    current_state = ERROR;
    break;

  case Esc:
    if (transition == 34 || transition == 92 || transition == 'n' || transition == 't' || transition == '$')
    {
      current_state = String;
    }
    else if (transition == 'x')
    {
      current_state = Esc1;
    }
    else if ((transition >= '0' && transition <= '9'))
    {
      current_state = Esc3;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Esc1:
    if ((transition >= '0' && transition <= '9') || (65 <= transition && transition <= 70) || (97 <= transition && transition <= 102))
    {
      current_state = Esc2;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Esc2:
    if ((transition >= '0' && transition <= '9') || (65 <= transition && transition <= 70) || (97 <= transition && transition <= 102))
    {
      current_state = String;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Esc3:
    if ((transition >= '0' && transition <= '9'))
    {
      current_state = Esc4;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Esc4:
    if ((transition >= '0' && transition <= '9'))
    {
      current_state = String;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  default:
    current_state = ERROR;
    break;
  }

  return current_state;
}

struct TOKEN generate_token()
{
  States current_state = Start;
  // enum End_states end_state = ES_ERROR;
  int transition = getchar();
  // TO DO crete dinamicli alocated array for value for token
  while (1)
  {
    current_state = Automat(current_state, transition);
  }
  // struct TOKEN token;

  // return token;
}
