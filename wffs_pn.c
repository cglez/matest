/*****************************************************************************\
#                                                                             #
#                           ---   MaTest 1.0   ---                            #
#                     Matrix Tester for logical matrices.                     #
#       Copyright (C) 2008, César González Gutiérrez <ceguel@gmail.com>       #
#                                                                             #
#    This file is part of MaTest.                                             #
#                                                                             #
#    MaTest is free software: you can redistribute it and/or modify           #
#    it under the terms of the GNU General Public License as published by     #
#    the Free Software Foundation, either version 3 of the License, or        #
#    (at your option) any later version.                                      #
#                                                                             #
#    MaTest is distributed in the hope that it will be useful,                #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#    GNU General Public License for more details.                             #
#                                                                             #
#    You should have received a copy of the GNU General Public License        #
#    along with MaTest.  If not, see <http://www.gnu.org/licenses/>.          #
#                                                                             #
#                                                                             #
\*****************************************************************************/


/*                               wffs_pn.c
*
*    This file contains the functions needed to evaluate a well formed formula
*    in polish notation.
*    We use the next convention for formulas in polish notation: all upper case
*    characters are connectives, all lower case characters are variables.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "MaTest.h"


/***
  Function symbol_type_pn:
   Returns symbol type of given character. Possible symbol types are:
   VAR (variable), UCON (unary connective), BCON (binary connective) or NONE.
***/
LogicSymbKind symbol_kind_pn (char symbol, Logic logic)
{
  if (islower (symbol))
    return VAR;
  else if (search_UCon (logic->UCons, symbol))
    return UCON;
  else if (search_BCon (logic->BCons, symbol))
    return BCON;
  else
    return NONE;
}


/***
  Function check_string:
   Checks the string and assures that only contains alpha characters, then
   returns true, otherwise returns false.
***/
bool check_string (char formula[])
{
  int i;
  
  for (i = 0; i < (int) strlen (formula); i++)
    {
      if (!isalpha (formula[i]))
        {
          perror ("Given formula has not-implemented characters.\n");
          return false;
        }
    }
  return true;
}


/***
  Function is_wff_pn:
   Checks if the given formula is a well formed formula in polish notation.
   If it's a WFF returns true, otherwise returns false.
***/
bool is_wff_pn (char formula[], Logic logic)
{
  int i, deep = 1;
  
  /* First check the formula for not implemented characters */
  if (check_string (formula))
    {
      for (i = 0; i < (int) strlen (formula); i++)
        {
          /* Variables decrease formula deep */
          if (islower (formula[i]))
            deep--;
          
          else if (isupper (formula[i]))
            {
              /* Unary connectives don't change the formula deep */
              if (search_UCon (logic->UCons, formula[i]))
                deep = deep;
              /* Binary connectives increase the formula deep */
              else if (search_BCon (logic->BCons, formula[i]))
                deep++;
              /* Else, if connective isn't unary or binary, it doesn't exists */
              else
                {
                  printf ("The connective %c is not defined.\n", formula[i]);
                  return false;
                }
            }
          /* If all connectives have arguments now but still rests arguments:
             deep exceeded. */
          if (deep == 0 && formula[i + 1] != 0)
            {
              printf (" Formula \"%s\" isn't a WFF!\n", formula);
              printf (" Deep exceeded, too much variables. Check the formula.\n");
              return false;
            }
        }
      /* If, after all, there are connectives without arguments: deep
         insufficient. */
      if (deep != 0)
        {
          printf (" Formula \"%s\" isn't a WFF!\n", formula);
          printf (" Deep insufficient, too few variables. Check the formula.\n");
          return false;
        }
      else
        return true;
    }
  else
    return false;
}


/***
  Procedure parse_polish:
   Parses a well formed formula in polish notation.
   Here, we read the formula from left to right, setting atoms one by one, that
   makes a correct well formed formula tree, that is a characteristic of
   prefixed notations and polish notation is a prefixed notation.
***/
void parse_polish (LogicWFF *tree, char formula[], Logic logic)
{
  LogicVar var;
  int i;
  
  for (i = 0; i < (int) strlen (formula); i++)
    {
      if (symbol_kind_pn (formula[i], logic) == VAR)
        {
          var = (LogicVar) search_var (logic->Vars, formula[i]);
          set_atom (tree, VAR, formula[i], &var->value);
        }
      else if (symbol_kind_pn (formula[i], logic) == UCON)
        set_atom (tree, UCON, formula[i], NULL);
      else if (symbol_kind_pn (formula[i], logic) == BCON)
        set_atom (tree, BCON, formula[i], NULL);
      else
        {
          printf ("Parsing... Unexpected error!\n");
          make_pause();
          return;
        }
    }
}
