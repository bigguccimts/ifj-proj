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

States Automat(States actual_state, int transition)
{

  switch (actual_state)
  {
  case Start:
    if (isalnum(transition) || transition == '$' || transition == '_' || transition == '?')
    {
      actual_state = ID;
    }
    else if ((transition >= '0' && transition <= '9'))
    {
      actual_state = Int;
    }
    else if (transition == '+')
    {
      actual_state = Add;
    }
    else if (transition == '-')
    {
      actual_state = Sub;
    }
    else if (transition == '*')
    {
      actual_state = Mul;
    }
    else if (transition == '/')
    {
      actual_state = Div;
    }
    else if (transition == '.')
    {
      actual_state = Conc;
    }
    else if (transition == '(')
    {
      actual_state = Rol;
    }
    else if (transition == ')')
    {
      actual_state = Ror;
    }
    else if (transition == '{')
    {
      actual_state = Cul;
    }
    else if (transition == '}')
    {
      actual_state = Cur;
    }
    else if (transition == '=')
    {
      actual_state = Equ;
    }
    else if (transition == '<')
    {
      actual_state = Less;
    }
    else if (transition == '>')
    {
      actual_state = Great;
    }
    else if (transition == '"')
    {
      actual_state = Equ;
    }
    else if (transition == ' ')
    {
      actual_state = Start;
    }
    else
    {
      actual_state = ERROR;
    }

    break;

  case ID:
    if (isalnum(transition))
    {
      actual_state = ID;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Int:
    if (transition >= '0' || transition <= '9')
    {
      actual_state = Int;
    }
    else if (transition == '.')
    {
      actual_state = Float;
    }
    else if (transition == 'E' || transition == 'e')
    {
      actual_state = Exp;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Float:
    if (transition >= '0' || transition <= '9')
    {
      actual_state = Float;
    }
    else if (transition == 'E' || transition == 'e')
    {
      actual_state = Exp;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Exp:
    if (transition >= '0' || transition <= '9')
    {
      actual_state = Exp2;
    }
    else if (transition == '+' || transition == '-')
    {
      actual_state = Exp1;
    }
    else
    {
      actual_state = ERROR;
    }

  case Exp1:
    if (transition >= '0' || transition <= '9')
    {
      actual_state = Exp2;
    }
    else
    {
      actual_state = ERROR;
    }

  case Exp2:
    if (transition >= '0' || transition <= '9')
    {
      actual_state = Exp2;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Add:
    actual_state = ERROR;
    break;

  case Sub:
    actual_state = ERROR;
    break;

  case Mul:
    actual_state = ERROR;
    break;

  case Div:
    if (transition == '/')
    {
      actual_state = Com;
    }
    else if (transition == '*')
    {
      actual_state = Com2;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Com:
    if (transition >= '32' && transition <= '126')
    {
      actual_state = Com;
    }
    else if (transition >= '\n')
    {
      actual_state = Com1;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Com1:
    actual_state = ERROR;
    break;

  case Com2:
    if (32 <= transition > 42 && 42 < transition <= 126)
    {
      actual_state = Com2;
    }
    else if (transition >= '*')
    {
      actual_state = Com3;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Com3:

    if (32 <= transition > 47 && 47 < transition <= 126)
    {
      actual_state = Com2;
    }
    else if (transition >= '/')
    {
      actual_state = Com4;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Com4:
    actual_state = ERROR;
    break;

  case Conc:
    actual_state = ERROR;
    break;

  case Rol:
    actual_state = ERROR;
    break;

  case Ror:
    actual_state = ERROR;
    break;

  case Cul:
    actual_state = ERROR;
    break;

  case Cur:
    actual_state = ERROR;
    break;

  case Equ:
    if (transition == '=')
    {
      actual_state = Equ1;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Equ1:
    if (transition == '=' || transition == '!')
    {
      actual_state = Equ2;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Equ2:
    actual_state = ERROR;
    break;

  case Less:
    if (transition == '=')
    {
      actual_state = Less1;
    }
    break;

  case Less1:
    actual_state = ERROR;
    break;

  case Great:
    if (transition == '=')
    {
      actual_state = Great1;
    }
    break;

  case Great1:
    actual_state = ERROR;
    break;

  case String:
    if (32 <= transition > 34 && 34 < transition > 92 && 92 < transition <= 126)
    {
      actual_state = String;
    }
    else if (transition = 34)
    {
      actual_state = String1;
    }
    else if (transition = 92)
    {
      actual_state = Esc;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case String1:
    actual_state = ERROR;
    break;

  case Esc:
    if (transition == 34 || transition == 92 || transition == 'n' || transition == 't' || transition == '$')
    {
      actual_state = String;
    }
    else if (transition = 'x')
    {
      actual_state = Esc1;
    }
    else if ((transition >= '0' && transition <= '9'))
    {
      actual_state = Esc3;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Esc1:
    if ((transition >= '0' && transition <= '9') || 65 <= transition <= 70 || 97 <= transition <= 102)
    {
      actual_state = Esc2;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Esc2:
    if ((transition >= '0' && transition <= '9') || 65 <= transition <= 70 || 97 <= transition <= 102)
    {
      actual_state = String;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Esc3:
    if ((transition >= '0' && transition <= '9'))
    {
      actual_state = Esc4;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  case Esc4:
    if ((transition >= '0' && transition <= '9'))
    {
      actual_state = String;
    }
    else
    {
      actual_state = ERROR;
    }
    break;

  default:
    actual_state = ERROR;
    break;
  }

  return actual_state;
}

struct TOKEN generate_token()
{
  States actual_state = Start;
  // enum End_states end_state = ES_ERROR;
  int transition = getchar();
  // TO DO crete dinamicli alocated array for value for token
  while (1)
  {
    actual_state = Automat(actual_state, transition);
  }
  // struct TOKEN token;

  // return token;
}
