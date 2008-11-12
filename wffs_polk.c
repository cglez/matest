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
SymbolType symbol_type (char symbol)
{
  if (islower (symbol))
    return VAR;
  else if (is_unary_connective (symbol, &the_logic -> unyconns))
    return UCON;
  else if (is_dyadic_connective (symbol, &the_logic -> dyaconns))
    return DCON;
  else
    return NONE;
}


/***
  Function check_string:
   Checks the string and asures that only contains alpha characters and then
   returns true, otherwise returns false.
***/
bool check_string (char formul[])
{
  int i;
  
  for (i = 0; i < strlen (formul); i++)
    {
      if (!isalpha (formul[i]))
        {
          printf ("Given formula have not-implemented characters.\n");
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
bool is_wff_pk (char formul[])
{
  int i, crtl = 1;
  
  if (check_string (formul))
    {
      for (i = 0; i < strlen (formul); i++)
        {
          if (islower (formul[i]))
            crtl--;
          
          else if (isupper (formul[i]))
            {
              if (is_unary_connective (formul[i], &the_logic -> unyconns))
                crtl = crtl;
              else if (is_dyadic_connective (formul[i], &the_logic -> dyaconns))
                crtl++;
              else
                {
                  printf ("The connective %c is not defined.\n", formul[i]);
                  return false;
                }
            }
          
          if (crtl == 0 && formul[i + 1] != 0)
            {
              printf ("Formula \"%s\" isn't a WFF!\n", formul);
              return false;
            }
        }
      if (crtl != 0)
        {
          printf ("Formula \"%s\" isn't a WFF!\n", formul);
          return false;
        }
      else
        return true;
    }
  else
    return false;
}


/*
int main (void)
{
  the_logic = (Logic) malloc (sizeof (logicType));
  
  the_logic -> dimmension = 5;
  the_logic -> mdv = the_logic -> dimmension - 1;
  
  set_default_unycons (the_logic);
  set_default_diacons (the_logic);
  
  printf ("Givme a formula: ");
  fgets (answer, BUFSIZ, stdin);
  sscanf (answer, "%s", formula);
  
  if (check_string (formula))
    {
      if (is_wff_pk (formula))
        {
          printf ("OK.\n", formula);
          return 0;
        }
    }
  else
    return 1;
}
*/

