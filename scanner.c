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
#include <stdio.h>
#include <stdlib.h>

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
  //TO DO write other keywords
  if (strcmp(value, "IF") == 0 || strcmp(value, "ELSE") == 0){
    return 1;
  }else{
    return 0;
  }
}

//TO DO 
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

  default:
    break;
  }

  return end_states;
}

struct TOKEN generate_token()
{

  TOKEN token;
  States previus_state;
  States current_state = Start;

  End_States end_state;

  int i = 0;
  int size = 10;
  char *Str = NULL;
  Str = init_Str(Str, 5);
  // TO DO crete dinamicli alocated array for value for token
  do
  {
    int transition = getchar();
    previus_state = current_state;
    current_state = Automat(current_state, transition);

    if (current_state != ERROR)
    {
      if (transition != ' ')
      {
        Str[i] = transition;
      }

      // TO DO realoc array make it bigger
      int a = i + 1;
      if (a > size - 1)
      {
        fprintf(stderr, "Too big\n");
        exit(LEX_ANALYSIS_ERR);
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
  // checks the tokens value delete later
  printf("Tokens value : ");
  for (int i = 0; i < size; i++)
  {

    if (Str[i] != NULL)
    {
      printf("%c", Str[i]);
    }
    else
    {
      break;
    }
  }
  printf("\n");
  // checks if we didn t end in state whitch is not END STATE
  if (previus_state == Exp || previus_state == Exp1 || previus_state == Com || previus_state == Com2 || previus_state == Com3)
  {
    fprintf(stderr, "Error ocured in class scanner.c\n");
    exit(LEX_ANALYSIS_ERR);
  }
  else
  {

    token.End_States = determin_EndState(previus_state, Str);
  }

  // Delete later
  // token.End_States = ES_Add;
  token.Value.intiger = *Str;

  clen_Str(Str);
  return token;
}
void generate_table_of_tokens()
{
  TOKEN token;
  token = generate_token();
  printf("Token %d", token.Value.intiger);
}

int main()
{
  generate_table_of_tokens();
}