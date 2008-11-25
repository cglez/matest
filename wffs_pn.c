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


#include "MaTest.h"


/***
  Function symbol_type:
   Returns symbol type of given character. Possible symbol types are:
   VAR (variable), UCON (unary connective), BCON (binary connective) or NONE.
***/
SymbolType symbol_type (char symbol, Logic logic)
{
  if (islower (symbol))
    return VAR;
  else if (is_unary_connective (symbol, &logic->unyConns))
    return UCON;
  else if (is_binary_connective (symbol, &logic->binConns))
    return BCON;
  else
    return NONE;
}


/***
  Function check_string:
   Checks the string and assures that only contains alpha characters and then
   returns true, otherwise returns false.
***/
bool check_string (char formula[])
{
  int i;
  
  for (i = 0; i < strlen (formula); i++)
    {
      if (!isalpha (formula[i]))
        {
          printf ("Given formula has not-implemented characters.\n");
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
  
  // First check the formula for not implemented characters
  if (check_string (formula))
    {
      for (i = 0; i < strlen (formula); i++)
        {
          // Variables decrease the formula deep
          if (islower (formula[i]))
            deep--;
          
          else if (isupper (formula[i]))
            {
              // Unary connectives don't change the formula deep
              if (is_unary_connective (formula[i], &logic->unyConns))
                deep = deep;
              // Binary connectives increase the formula deep
              else if (is_binary_connective (formula[i], &logic->binConns))
                deep++;
              // Else, if connective isn't unary or binary, it doesn't exists
              else
                {
                  printf ("The connective %c is not defined.\n", formula[i]);
                  return false;
                }
            }
          // If all connectives have arguments now but still rests arguments:
          // deep exceeded
          if (deep == 0 && formula[i + 1] != 0)
            {
              printf (" Formula \"%s\" isn't a WFF!\n", formula);
              printf (" Deep exceeded, too much variables. Check the formula.\n");
              return false;
            }
        }
      // If, after all, there are connectives without arguments: deep 
      // insufficient.
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

