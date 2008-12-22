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
*    This file contains functions that interactuate with the user, others that
*    show information into the screen and others that get input information.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "MaTest.h"


/***
  Function readin:
   Returns the first character from a given string that is in the given pattern.
***/
char readin (char str[], char pattern[])
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
  #ifdef POSIX
    system ("clear");
  #elif WIN
    system ("cls");
  #else
    int i;
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
   Prints a header with the name of the program, version and a short
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
void menu_info (Work work)
{
  LogicUCon unyaux;
  LogicBCon binaux;
  
  printf ("   Matrices dimension:    ");
  if (work->DIM)
    printf ("%ix%i", work->DIM, work->DIM);
  else
    printf ("Not defined!");
  
  printf ("\n   Min. Desig. Value:     ");
  if (work->MDV)
    printf ("%i", work->MDV);
  else
    printf ("Not defined!");
  
  printf ("\n   Unary  connectives:    ");
  unyaux = work->logic->UCons;
  while (unyaux)
    {
      printf ("%c ", unyaux->name);
      unyaux = unyaux->next;
    }
  
  printf ("\n   Binary connectives:    ");
  binaux = work->logic->BCons;
  while (binaux)
    {
      printf ("%c ", binaux->name);
      binaux = binaux->next;
    }
  
  if (work->formula_pn[0])
    printf ("\n   Formula:               %s", work->formula_pn);
  else
    printf ("\n   Formula:               Not defined");
  
  if (work->eval_values == ALL)
    printf ("\n   Evaluate values:       [All]   Designated   Not designated");
  if (work->eval_values == DESIGNATED)
    printf ("\n   Evaluate values:       All   [Designated]   Not designated");
  if (work->eval_values == NOTDESIGNATED)
    printf ("\n   Evaluate values:       All   Designated   [Not designated]");
  
  printf ("\n\n");
}


/***
  Procedure menu_options:
   Prints the menu of options available.
***/
void menu_options (void)
{
  printf (" Options:\n"
          "   V: Values evaluated.\n"
          "   M: Redefine the Minimum Designated Value.\n"
          "\n"
          "   N: New connective.           P: Print matrices into the screen.\n"
          "   D: Delete a connective.      W: Write matrices to external file.\n"
          "\n"
          "   F: Introduce a new Formula.\n"
          "   E: Evaluate formula.\n"
          "                                           A: About    H: Help    Q: Quit\n"
          "\n"
          " Select your option: ");
}


/***
  Procedure menu_init:
   Prints a short credits message and legal notice.
   Designed to appear at the beginning.
***/
void menu_init (void)
{
  menu_header();
  printf ("\n"
          "      Copyright (C) 2008  César González Gutiérrez.\n"
          "      License GPLv3+: GNU General Public Licence version 3 or later,\n"
          "      see <http://gnu.org/licenses/gpl.html>.\n"
          "      This is free software: you are free to change and redistribute it.\n"
          "      There is NO WARRANTY, to the extent permitted by law.\n"
          "      For more details type 'a' in the main menu.\n\n\n");
}


/***
  Procedure menu_dimension:
   Message before asking for dimension matrices.
***/
void menu_dimension (void)
{
  printf (" To start working you have to define the matrices dimension.\n"
          " Then, the program will define for you the most common connectives like in\n"
          " Łukasiewicz multivaluated logics model.\n\n");
}


/***
  Procedure menu_about:
   Message showing short description, version, credits and long legal notice.
***/
void menu_about (void)
{
  clear_scr();
  printf ("\n"
          " This is MaTest version %s\n"
          "\n"
          " AUTHOR: César González Gutiérrez <ceguel@gmail.com>\n"
          "\n"
          " DESCRIPTION: MaTest (Matrix Tester) is a free, cross-platform logical matrix\n"
          "  tester. Defined a couple of connectives as matrices, a minimun designated\n"
          "  value and given a formula, it gets all possible values for the formula and\n"
          "  evaluate if they are designated or not.\n"
          "  Sucessor of Matrigüity by J. M. Méndez & B. García Noriega (1982).\n"
          "\n", VERSION);
  printf (" COPYRIGHT (C) 2008, César González Gutiérrez <ceguel@gmail.com>.\n"
          "  MaTest is free software: you can redistribute it and/or modify\n"
          "  it under the terms of the GNU General Public License as published by\n"
          "  the Free Software Foundation, either version 3 of the License, or\n"
          "  (at your option) any later version.\n"
          "\n");
  printf ("  MaTest is distributed in the hope that it will be useful, but\n"
          "  without any warranty; without even the implied warranty of\n"
          "  merchantability or fitness for a particular purpose.\n"
          "  See the GNU General Public License for more details:\n"
          "  <http://gnu.org/licenses/gpl.html>.\n");
}


/***
  Procedure menu_help:
   Will show the inline help. Not implemented by now.
***/
void menu_help (void)
{
  menu_header();
  printf ("\n"
          "There isn't inline help available by now.\n"
          "See the Readme.txt file that come with this program.\n");
}


/***
  Procedure menu_index:
   Prints the global menu.
***/
void menu_index (Work work)
{
  menu_header();
  menu_info (work);
  menu_options();
}
