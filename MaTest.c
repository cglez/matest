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


/*                               MaTest.c
*
*    This file contains the main function and manages the general operations of
*    the program.
*/


#include "MaTest.h"


int main (void)
{
  char opt, answer[BUFSIZ];
  Logic the_logic;
  
  the_logic = (Logic) malloc (sizeof (logicType));
  
  menu_init();
  
  do
    {
      fgets (answer, BUFSIZ, stdin);
      sscanf (answer, "%i", &the_logic -> dimmension);
    }
  while (the_logic -> dimmension < 2);
  
  the_logic -> mdv = the_logic -> dimmension - 1;
  
  set_default_unycons (the_logic);
  set_default_diacons (the_logic);
  
  /* Manage options */
  for (;;)
    {
      menu_index (the_logic);
      opt = readin (answer, "vrwadfehq\n");
      
      switch (opt)
        {
          case 'v':
            clear_scr ();
            menu_header ();
            menu_info (the_logic);
            do
              {
                printf ("Nuevo minimo valor designado: ");
                fgets (answer, BUFSIZ, stdin);
                sscanf (answer, "%i", &the_logic -> mdv);
              }
            while ((the_logic -> mdv < 0) || ((the_logic -> mdv) > (the_logic -> dimmension - 1)));
            break;
          
          /*
          case 'p':
            show_matrix ();
            break;
          */
          
          case 'f':
            do
              {
                printf ("Write a formula in polk notation: ");
                fgets (answer, BUFSIZ, stdin);
                sscanf (answer, "%s", the_logic -> formula);
              }
            while (!is_wff_pk (the_logic -> formula, the_logic));
            break;
          
          case 'e':
            if (the_logic -> formula[0] == 0)
              printf ("\nPrimero debe introducir una formula!\n");
            /*
            else
              {
                evaluate (formula);
                printf ("\nPulse ENTER para continuar");
                getchar ();
              }
            */
            break;
          
          /*
          case 'r':
            read_matrix ();
            break;
          
          case 'w':
            write_matrix ();
            break;
          */
          
          case 'q':
            return 0;
        }
    }
}

