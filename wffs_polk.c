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


/*                               wffs_polk.c
*
*    This file contains the functions that evalues if given formula is a well
*    formed formula in polk notation. For more information about polk notation
*    see the readme file.
*/


#include "MaTest.h"


/***
  Function symbol_type:
***/
SymbolType symbol_type (char symbol, Logic logic)
{
  if (islower (symbol))
    return VAR;
  else if (is_unary_connective (symbol, &logic -> unyConns))
    return UCON;
  else if (is_binary_connective (symbol, &logic -> binConns))
    return BCON;
  else
    return NONE;
}


/***
  Function check_string:
   Checks the string and asures that only contains alpha characters and then
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
  Function is_wff_pk:
   Checks if the given formula is a well formed formula in polk notation. If
   it's a WFF returns true, otherwise returns false.
***/
bool is_wff_pk (char formula[], Logic logic)
{
  int i, crtl = 1;
  
  if (check_string (formula))
    {
      for (i = 0; i < strlen (formula); i++)
        {
          if (islower (formula[i]))
            crtl--;
          
          else if (isupper (formula[i]))
            {
              if (is_unary_connective (formula[i], &logic -> unyConns))
                crtl = crtl;
              else if (is_binary_connective (formula[i], &logic -> binConns))
                crtl++;
              else
                {
                  printf ("The connective %c is not defined.\n", formula[i]);
                  return false;
                }
            }
          
          if (crtl == 0 && formula[i + 1] != 0)
            {
              printf ("Formula \"%s\" isn't a WFF!\n", formula);
              return false;
            }
        }
      if (crtl != 0)
        {
          printf ("Formula \"%s\" isn't a WFF!\n", formula);
          return false;
        }
      else
        return true;
    }
  else
    return false;
}

