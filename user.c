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


/*                               user.c
*
*    This file contains the functions that do the user interactuation, that is
*    functions that show information in the screen and others that get input
*    information.
*/


#include "MaTest.h"


/***
  Function readin:
   Returns the first character from a given string that is in the given pattern.
***/
char readin (char *str, char *pattern)
{
  char ch;
  int i;

  fflush (stdout);
  for (;;)
    {
      fgets (str, BUFSIZ, stdin);
      ch = 0;
      for (i = 0; str[i]; i++)
        {
          if (isupper (ch = str[i]))
            ch = tolower (ch);
          if (strchr (pattern, ch))
            return ch;
        }
    }
}


/***
  Procedure clear_scr():
   Cleans the screen depending on the system defined by the preprocessor.
***/
void clear_scr (void)
{
  int i;
  
  #ifdef UNIX
    system ("clear");
  #elif WIN
    system ("cls");
  #else
    for (i = 0; i < 24; i++)
      printf ("\n");
  #endif
}


/***
  Procedure make_pause:
   Prints a message and waits until user press enter.
***/
void make_pause (void)
{
  printf ("\nPress Enter to continue... ");
  fflush (stdout);
  while (getchar () != '\n');
}


/***
  Procedure menu_header:
   Prints a header with the name of the programm, version and a short
   description.
***/
void menu_header (void)
{
  clear_scr();
  printf ("\n"
          "                            --- MaTest %s ---\n"
          "                     Matrix Tester for logical matrices\n"
          "\n", VERSION);
}


/***
  Procedure menu_info:
   Prints information about the state of the configurable elements.
***/
void menu_info (Logic logic)
{
  unyCon unyaux;
  binCon binaux;
  
  printf ("   Matrices dimmension:    ");
  if (logic->dimmension)
    printf ("%ix%i", logic->dimmension, logic->dimmension);
  else
    printf ("Not defined!");
  printf ("\n   Min. Designated Value:  ");
  if (logic->mdv)
    printf ("%i", logic->mdv);
  else
    printf ("Not defined!");
  
  printf ("\n   Unary  connectives:     ");
  unyaux = logic->unyConns;
  while (unyaux)
    {
      printf ("%c ", unyaux->name);
      unyaux = unyaux->next;
    }
  printf ("\n   Binary connectives:     ");
  binaux = logic->binConns;
  while (binaux)
    {
      printf ("%c ", binaux->name);
      binaux = binaux->next;
    }
  if (logic->formula[0])
    printf ("\n   Formula:                %s", logic->formula);
  else
    printf ("\n   Formula:                Not defined!");
  printf ("\n\n\n");
}


/***
  Procedure menu_options:
   Prints the options available.
***/
void menu_options (void)
{  
  printf (" Options:\n"
          "   V: Redefine the Minimun Designated Value.\n"
          "\n"
          "   P: Print matrices into the screen.         N: Add a new connective.\n"
          "   W: Write matrices to external file.        D: Delete a connective.\n"
          "\n"
          "   F: Introduce a new formula.\n"
          "   E: Evaluate formula.\n"
          "                                           A: About    H: Help    Q: Quit\n"
          "\n"
          " Select your option: ");
}


/***
  Procedure menu_init:
   Prints a simple message asking for a dimmension of the matrices.
   It's designed to appears at the beginning.
***/
void menu_init (void)
{
  menu_header();
  printf ("  Copyright (C) 2008  César González Gutiérrez.\n"
          "  License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
          "  This is free software: you are free to change and redistribute it.\n"
          "  There is NO WARRANTY, to the extent permitted by law.\n"
          "  For more details type 'a' in the main menu.\n\n\n"
          "To start working you need to define the matrices dimmension and then the\n"
          "program will define the most common connectives like in Łuckassiewicz\n"
          "multivaluate logics.\n\n"
          "Matrix dimmension: ");
}


/***
  Procedure menu_index:
   Prints the global menu.
***/
void menu_index (Logic logic)
{
  menu_header();
  menu_info (logic);
  menu_options();
}

