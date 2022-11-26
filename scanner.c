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
#include "returncodes.h"
#include "scanner.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// EOP stands for END OF PROGRAM.
// Global variables
bool IS_PROLOG = false; // true when input file contains prolog
int size = 1;           // is size of token.value.str so we can dynamicly double it if we have long string


// TO DO comma , - DONE
//  TO DO !== && === -DONE
//  TO DO coments - DONE
//  TO DO prolog represnted in 2 tokens -- In the end is represented in 1 and is checked here. - DONE
//  TO DO end of program - DONE only for ?>
//  TO DO end of program without ?>
//  TO DO. Dynamicly alocate strings (String only size up mby size down would be good to add) - DONE
//  TO DO. Repair Strings - (,) - missing from automat - DONE
//  TO DO. Gabika is redoing automat schema so need to do aditional tokens (cca 4 new) 30 mins. -DONE
States Automat(States current_state, int transition)
{
  switch (current_state)
  {
  case Start:
    if (isalpha(transition) || transition == '$' || transition == '_' || transition == '?')
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
    else if (transition == '!')
    {
      current_state = Notequ1;
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
      current_state = String;
    }
    else if (transition == ' ')
    {
      current_state = Start;
    }
    else if (transition == ';')
    {
      current_state = Sem;
    }
    else if (transition == ',')
    {
      current_state = Col;
    }
    else
    {
      current_state = ERROR;
    }

    break;

  case ID:
    if (isalnum(transition) || transition == '_')
    {
      current_state = ID;
    }
    else if(transition == '>') 
    {
      current_state = EOP2;
    }else{
      current_state = ERROR;
    }
    break;

  case Int:
    if (transition >= '0' && transition <= '9')
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

  case Sem:
    current_state = ERROR;
    break;

  case Col:
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

  case Notequ:
    if (transition == '=')
    {
      current_state = Notequ1;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Notequ1:
    if (transition == '=')
    {
      current_state = Notequ2;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case Notequ2:
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
    if (transition == '=')
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

    if (transition >= 32 && transition <= 126 && transition != '"' && transition != 92)
    {
      current_state = String;
    }
    else if (transition == '"')
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
  case EOP:
    if (transition == '>')
    {
      current_state = EOP2;
    }
    else
    {
      current_state = ERROR;
    }
    break;

  case EOP2:
    current_state = ERROR;
    break;

  default:
    current_state = ERROR;
    break;
  }

  return current_state;
}

char *init_Str(char *Str, int size)
{

  Str = (char *)calloc(size, sizeof(char));

  if (Str == NULL)
  {
    fprintf(stderr, "Could not allocate required memory\n");

    exit(LEX_ANALYSIS_ERR);
  }

  return Str;
}

void clen_Str(char *Str)
{
  free(Str);
}

int check_for_keyword(char *value)
{
  if (strcmp(value, "if") == 0 || strcmp(value, "else") == 0 || strcmp(value, "float") == 0 || strcmp(value, "function") == 0 || strcmp(value, "int") == 0 || strcmp(value, "null") == 0 || strcmp(value, "return") == 0 || strcmp(value, "string") == 0 || strcmp(value, "void") == 0 || strcmp(value, "while") == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

char *resize_Str(char *Str, int size)
{
  char *Str2;
  Str2 = realloc(Str, size * sizeof(int));
  return Str2;
}

End_States determin_EndState(States Final_sate, char *value)
{
  End_States end_states;
  switch (Final_sate)
  {
  case ID:
    if (check_for_keyword(value))
    {
      end_states = ES_KEY_WORD;
    }
    else
    {
      end_states = ES_ID;
    }
    break;
  case Int:
    end_states = ES_Int;
    break;
  case Float:
    end_states = ES_Float;
    break;
  case Exp2:
    end_states = ES_Exp2;
    break;
  case Add:
    end_states = ES_Add;
    break;
  case Sub:
    end_states = ES_Sub;
    break;
  case Mul:
    end_states = ES_Mul;
    break;
  case Div:
    end_states = ES_Div;
    break;
  case Conc:
    end_states = ES_Conc;
    break;
  case Rol:
    end_states = ES_Rol;
    break;
  case Ror:
    end_states = ES_Ror;
    break;
  case Cul:
    end_states = ES_Cul;
    break;
  case Cur:
    end_states = ES_Cur;
    break;
  case Equ:
    end_states = ES_Equ;
    break;
  case Equ1:
    end_states = ES_Equ1;
    break;
  case Equ2:
    end_states = ES_Equ2;
    break;
  case Notequ:
    end_states = ES_Notequ;
    break;
  case Notequ1:
    end_states = ES_Notequ1;
    break;
  case Notequ2:
    end_states = ES_Notequ2;
    break;
  case Less:
    end_states = ES_Less;
    break;
  case Less1:
    end_states = ES_Less1;
    break;
  case Great:
    end_states = ES_Great;
    break;
  case Great1:
    end_states = ES_Great1;
    break;
  case Com1:
    end_states = ES_Com1;
    break;
  case Com4:
    end_states = ES_Com4;
    break;
  case String1:
    end_states = ES_String1;
    break;
  case EOP2:
    end_states = ES_EOP2;
    break;
  default:
    end_states = ES_ERROR;
    break;
  }

  return end_states;
}

bool check_prolog()
{
  bool result = false;
  int transition;
  char In_stream;
  char Prolog[32] = "<?php declare(strict_types=556);";
  do
  {
    transition = getchar();
  } while (transition == ' ');

  // give back char posible < already loaded
  ungetc(transition, stdin);

  for (int i = 0; i < 31; i++)
  {
    In_stream = getchar();
    while (In_stream == '\n')
    {
      In_stream = getchar();
    }
    if (Prolog[i] != In_stream)
    {
      return result;
    }
  }

  result = true;

  return result;
}

struct TOKEN generate_token()
{

  // declare structures
  TOKEN token;
  States previus_state;
  States current_state = Start;
  // define variables
  int i = 0;

  char *Str = NULL;

  Str = init_Str(Str, size);

  // prolog check
  // will be done only once when first getToken is called
  // wrong prolog will result in exit of program
  if (!IS_PROLOG)
  {

    if (!check_prolog())
    {
      fprintf(stderr, "Error ocured in class scanner.c Prolog\n");
      exit(LEX_ANALYSIS_ERR);
    }
    else
    {
      IS_PROLOG = true;
      char Prolog[32] = "<?php declare(strict_types=556);";
      token.Value.Str = Prolog;
      token.end_state = ES_Prolog;
      return token;
    }
  }
  else
  {

    do
    {
      int transition = getchar();
      // We take another character if we took EOL-\n
      while (transition == '\n')
      {
        transition = getchar();
      }

      previus_state = current_state;
      current_state = Automat(current_state, transition);

      if (current_state != ERROR)
      {
        if (transition != ' ' || transition != '\n')
        {
          Str[i] = transition;
        }
        int a = i + 1;
        // if array containing value of token runes out of space doubles in size
        if (a > size - 1)
        {
          size = 2 * size;
          Str = resize_Str(Str, size);
        }
        // increment index of an Array containing value of token
        i++;
      }
      else
      {
        // unused char is puted back on strem w/o empty space
        if (transition != ' ')
        {
          ungetc(transition, stdin);
        }
      }
    } while (current_state != ERROR);

    // checks if we didn t end in state whitch is not END STATE
    if (previus_state == Exp || previus_state == Exp1 || previus_state == Com || previus_state == Com2 || previus_state == Com3 || previus_state == Equ1 || previus_state == Notequ1)
    {
      fprintf(stderr, "Error ocured in class scanner.c\n");
      exit(LEX_ANALYSIS_ERR);
    }
    else
    {
      token.end_state = determin_EndState(previus_state, Str);
    }

    if (token.end_state == ES_Int)
    {
      int i;
      sscanf(Str, "%d", &i);
      token.Value.intiger = i;
    }
    else if (token.end_state == ES_Float)
    {
      token.Value.floating = *Str;
    }
    else
    {
      token.Value.Str = Str;
    }

    return token;
  }
}
