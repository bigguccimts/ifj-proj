/**
 * Implementace překladače imperativního jazyka IFJ22
 *
 * @file scanner.c
 * @brief Lexical analysis
 *
 * @author Dušan Slúka (xsluka00)
 */

#include "returncodes.h"
#include "scanner.h"
#include "dyn_str.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Global variables

bool IS_PROLOG = false;	 // true when input file contains prolog
int size = DYN_STR_SIZE; // is size of token.value.str so we can dynamically resize it if we have long string

/**
 * @brief KA automat for diferent tokens.
 *
 * @param current_state state in which is automat at the moment
 * @param transition	is character which will determin next state.
 * @return States after transition
 */
States Automat(States current_state, int transition)
{
	switch (current_state)
	{
	case Start:
		if (isalpha(transition) || transition == '_' || transition == '?')
		{
			current_state = FID;
		}
		else if (transition == EOF)
		{
			current_state = EEOF;
		}
		else if (transition == '$')
		{
			current_state = VID1;
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
			current_state = Notequ;
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
		else if (transition == ' ' || transition == '\n')
		{
			current_state = Start;
		}
		else if (transition == ';')
		{
			current_state = Sem;
		}
		else if (transition == ',')
		{
			current_state = Comm;
		}
		else if (transition == ':')
		{
			current_state = Col;
		}
		else
		{
			current_state = ERROR;
		}

		break;
	case FID:
		if (transition == '>')
		{
			current_state = EOP2;
		}
		else if (isalnum(transition) || transition == '_')
		{
			current_state = FID;
		}
		else
		{
			current_state = ERROR;
		}
		break;

	case VID1:
		if (isalnum(transition) || transition == '_')
		{
			current_state = VID2;
		}
		else
		{
			current_state = ERROR;
		}
		break;

	case VID2:
		if (isalnum(transition) || transition == '_')
		{
			current_state = VID2;
		}
		else
		{
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
		if (transition >= '0' && transition <= '9')
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
		if (transition >= '0' && transition <= '9')
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
		if (transition >= '0' && transition <= '9')
		{
			current_state = Exp2;
		}
		else
		{
			current_state = ERROR;
		}
		break;
	case Exp2:
		if (transition >= '0' && transition <= '9')
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

	case Comm:
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
		else if (transition >= '\n' || transition >= EOF)
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
		else if (transition == 10)
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
		else if (transition == 10)
		{
			current_state = Com3;
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
		else
		{
			current_state = ERROR;
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
		else
		{
			current_state = ERROR;
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

	case EOP2:
		if (transition != EOF)
		{
			current_state = EOP3;
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
/**
 * @brief checks if string is keyword
 *
 * @param value is string which we check.
 * @return 	int 1 if it is keyword
 * 			int 2 if it is not keyword
 */
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
/**
 * @brief checks if string is build in function
 *
 * @param value is string which we check.
 * @return 	int 1 if it is build in function
 * 			int 2 if it is build in function
 */
int check_for_BIF(char *value)
{
	if (strcmp(value, "reads") == 0 || strcmp(value, "readi") == 0 || strcmp(value, "readf") == 0 || strcmp(value, "write") == 0 || strcmp(value, "ord") == 0 || strcmp(value, "chr") == 0 || strcmp(value, "strlen") == 0 || strcmp(value, "substring") == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * @brief Choses end state for token
 *
 * @param Final_sate is state in whitch automat ended
 * @param value		 is String which was collected along the transitions in automat
 * @return End_States
 */
End_States determin_EndState(States Final_sate, char *value)
{
	End_States end_states;
	switch (Final_sate)
	{
	case FID:
		if (check_for_keyword(value))
		{
			end_states = ES_KEY_WORD;
		}
		else if (check_for_BIF(value))
		{

			end_states = ES_BIF;
		}
		else
		{
			end_states = ES_FID;
		}
		break;
	case VID2:
		end_states = ES_VID2;
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
	case Comm:
		end_states = ES_Comm;
		break;
	case Col:
		end_states = ES_Col;
		break;
	case Sem:
		end_states = ES_Sem;
		break;
	default:
		end_states = ES_ERROR;
		break;
	}

	return end_states;
}
/**
 * @brief Checks if program file contains prolog.
 * 		  Saved in global variable.
 * @return true if prolog was corect in source file
 * @return false if prolog was not corect in source file
 */
bool check_prolog()
{
	bool result1 = false;
	bool result2 = false;
	int transition;
	int num = 0;
	char In_stream;
	char Prolog1[5] = "<?php";
	char Prolog2[24] = "declare(strict_types=1);";

	for (int i = 0; i < 5; i++)
	{
		In_stream = getchar();

		if (Prolog1[i] != In_stream)
		{
			return result1;
		}
	}

	result1 = true;
	transition = getchar();
	while (transition == ' ' || transition == '\n')
	{
		num += 1;
		transition = getchar();
	}

	if (num == 0)
	{
		return false;
	}

	// give back char posible (d) already loaded

	ungetc(transition, stdin);

	for (int i = 0; i < 24; i++)
	{
		In_stream = getchar();

		if (Prolog2[i] != In_stream)
		{
			return result2;
		}
	}

	transition = getchar();

	ungetc(transition, stdin);

	result2 = true;

	return result2;
}
/**
 * @brief generate token for errors
 * 
 * t1.Value.intiger = 1 - Eror in prolog
 * t1.Value.intiger = 2 - Eror in closing
 * t1.Value.intiger = 3 - Wrong ES
 *
 * @return struct TOKEN returns custom token used for representing error in class
 */
struct TOKEN generate_Eror_token(int er)
{
	TOKEN t1;
	t1.end_state = ES_ERROR;

	if (er == 1)
	{
		t1.Value.intiger = 1;
	}
	else if (er == 2)
	{
		t1.Value.intiger = 2;
	}
	else
	{
		t1.Value.intiger = 3;
	}
	return t1;
}

/**
 * @brief generates new token from source file.
 *
 * @return struct TOKEN returns next token based on source file
 */
struct TOKEN generate_token()
{

	// declare structures
	TOKEN token;
	States previus_state;
	States current_state = Start;
	// define variables
	size = DYN_STR_SIZE;
	char *Str = NULL;

	// var for ""
	int u = 1;

	dyn_str_init(&Str);
	dyn_str_alloc(&Str);

	// prolog check
	// will be done only once when first getToken is called
	// wrong prolog will result in exit of program
	if (!IS_PROLOG)
	{
		if (!check_prolog())
		{
			fprintf(stderr, "Error ocured in class scanner.c Prolog\n");
			TOKEN t1 = generate_Eror_token(2);
			free(Str);
			return t1;
		}
		else
		{
			IS_PROLOG = true;
			char Prolog[32] = "<?php declare(strict_types=556);";
			token.Value.Str = Prolog;
			token.end_state = ES_Prolog;
			free(Str);
			return token;
		}
	}
	else
	{

		do
		{
			int transition = getchar();

			previus_state = current_state;
			current_state = Automat(current_state, transition);

			if (current_state == EEOF)
			{
				token.end_state = ES_EOF;
				token.Value.Str = "EOF";
				free(Str);
				return token;
			}

			if (current_state != ERROR)
			{
				// Value of token cant contain whitespace and EOL

				if (transition == 32 || transition == 10 || transition == EOF)
				{
					if (current_state == String && transition == 32)
					{
						Str = dyn_str_addc(Str, transition);
					}
				}
				else
				{
					if (current_state == Com || current_state == Com2 || current_state == Com3)
					{
					}
					else
					{
						if (transition == '"')
						{
							u = u - 1;
						}

						if (u == 0)
						{
							u = u - 1;
						}
						else
						{

							if ((transition != '"' || current_state != String1))
							{

								Str = dyn_str_addc(Str, transition);
							}
						}
					}
				}
			}
			else
			{
				// unused char is puted back on strem w/o empty space
				if (transition == ' ' || transition == '\n' || transition == EOF)
				{
				}
				else
				{
					ungetc(transition, stdin);
				}
			}
		} while (current_state != ERROR);

		if (previus_state == EOP3)
		{
			fprintf(stderr, "Error ocured in class scanner.c character after closing statement\n");
			TOKEN t1 = generate_Eror_token(2);
			free(Str);
			return t1;
		}

		// checks if we didn t end in state which is not END STATE
		if (previus_state == Exp || previus_state == Exp1 || previus_state == Com || previus_state == Com2 || previus_state == Com3 || previus_state == Equ1 || previus_state == Notequ1 || previus_state == VID1)
		{

			TOKEN t1 = generate_Eror_token(3);
			free(Str);
			return t1;
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
			free(Str);
		}
		else if (token.end_state == ES_Float)
		{
			float x;
			x = atof(Str);
			token.Value.floating = x;
			free(Str);
		}
		else
		{

			token.Value.Str = malloc(sizeof(Str));

			for (int i = 0; i < (int)strlen(Str) + 1; i++)
			{
				token.Value.Str[i] = Str[i];
			}

			token.Value.Str[(int)strlen(Str) + 1] = '\0';

			free(Str);
		}

		return token;
	}
}