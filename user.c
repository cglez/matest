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
            ch = tolower(ch);
          if (strchr (pattern, ch))
            return ch;
        }
    }
}


/***
  Procedure clear_scr():
   Cleans the screan, pretends to be multisystem compatible.
***/
void clear_scr (void)
{
  int i;
  
  if (system ("clear"))
    system ("clear");
  else if (system ("cls"))
    system ("cls");
  else
    {
      for (i = 0; i < 24; i++)
        printf ("\n");
    }
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
void menu_info (void)
{
  unyCon unyaux;
  diaCon diaaux;
  
  printf ("Info:");
  printf ("\n   Matrices dimmension:      ");
  if (the_logic -> dimmension)
    printf ("%ix%i", the_logic -> dimmension, the_logic -> dimmension);
  else
    printf ("Not defined!");
  printf ("\n   Minimun Designated Value: ");
  if (the_logic -> mdv)
    printf ("%i", the_logic -> mdv);
  else
    printf ("Not defined!");
  
  printf ("\n   Unary  connectives:       ");
  unyaux = the_logic -> unyconnectives;
  while (unyaux)
    {
      printf ("%c ", unyaux -> name);
      unyaux = unyaux -> next;
    }
  printf ("\n   Binary connectives:       ");
  diaaux = the_logic -> diaconnectives;
  while (diaaux)
    {
      printf ("%c ", diaaux -> name);
      diaaux = diaaux -> next;
    }
  if (formula[0])
    printf ("\n   Formula:                  %s", formula);
  else
    printf ("\n   Formula:                  Not defined!", formula);
  printf ("\n\n\n");
}


/***
  Procedure menu_options:
   Prints the options available.
***/
void menu_options (void)
{  
  printf ("Options:\n"
          "   V: Redefine the Minimun Designated Value.\n"
          "\n"
          "   R: Read matrices from external file.       A: Add a new connective.\n"
          "   W: Write matrices to external file.        D: Delete a connective.\n"
          "\n"
          "   F: Introduce a new formula.\n"
          "   E: Evaluate formula.\n"
          "                                                       H: Help    Q: Quit\n"
          "\n"
          "Select your option: ");
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
void menu_index (void)
{
  menu_header();
  menu_info();
  menu_options();
}


/*
void menu (void)
{
  clear_scr();
  printf ("\n"
          "                             --- MaTest ---\n"
          "                     Matrix Tester for logical matrices\n"
          "\n"
          "\n"
          "  V: Redefine the Minimun Designated Value.\n"
          "\n"
          "  R: Read matrices from external file.\n"
          "  W: Write matrices in to external file.\n"
          "\n"
          "\n"
          "  Redefine matrices manually:\n"
          "\n"
          "       I: Implication        C: Conjunction           D: Disjunction\n"
          "       N: Negation           L: Necessity             M: Possibility\n"
          "\n"
          "\n"
          "  F: Introduce new formula.\n" 
          "  E: Evaluate formula.\n"
          "                                                                  Q: Quit\n"
          "\n"
          " Select your option: ");
}
*/

/*
{
  printf ("\n\n");
  printf ("V: Redefinir minimo valor designado\n\n");
  printf ("R: Leer matrices desde archivo\n");
  printf ("W: Guardar las matrices a un archivo\n");
  printf ("P: Ver matrices\n");
  printf ("Redefinir matrices:\n");
  printf ("  I)mplicacion  C)onjuncion  D)isyuncion\n");
  printf ("  N)egacion     (L) Necesidad   (M) Posibilidad\n\n");
  printf ("F: Introducir formula\n");
  printf ("E: Evaluar formula\n\n");
  printf ("Q: Salir\n\n");
  printf ("Elija su opcion: ");
}
*/

/*
int main (void)
{
  the_logic = (Logic) malloc (sizeof (logicType));
  the_logic -> dimmension = 10;
  the_logic -> mdv = 7;
  formula[0] = 0;
  set_default_unycons (the_logic);
  set_default_diacons (the_logic);
  
  menu_index();
  printf ("\n");
}
*/

