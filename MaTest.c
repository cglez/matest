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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MaTest.h"


int main (void)
{
  char opt, answer[BUFSIZ], namefile[BUFSIZ];
  FILE *outfile;
  Work the_work;
  
  the_work = (Work) malloc (sizeof (workType));
  the_work->logic = (Logic) malloc (sizeof (logicType));
  the_work->formula_pn[0] = 0;
  the_work->wff = NULL;
  
  menu_init();
  
  // Ask for the matrices dimension
  menu_dimension ();
  do
    {
      printf (" Matrices dimension ( <1 ): ");
      fgets (answer, BUFSIZ, stdin);
      sscanf (answer, "%i", &the_work->logic->dimension);
    }
  while (the_work->logic->dimension < 2);
  
  // Define default Minimum Designated Value like in Łukasiewicz model
  the_work->logic->mdv = the_work->logic->dimension - 1;
  
  // Define the default connectives
  set_default_uny_conns (the_work->logic);
  set_default_bin_conns (the_work->logic);
  
  // Show all evaluated values by default
  the_work->eval_values = ALL;
  
  /* Manage options */
  for (;;)
    {
      menu_index (the_work);
      opt = readin (answer, "vmpwndfeahq");
      
      switch (opt)
        {
          // Change the evaluated values showed
          case 'v':
            clear_scr ();
            menu_header ();
            menu_info (the_work);
            printf (" What values do you want to be evaluated?\n"
                    " All, Designated, Not designated. (a/d/n): ");
            opt = readin (answer, "adn\n");
            switch (opt)
              {
                case 'a':
                  the_work->eval_values = ALL;
                  break;
                case 'd':
                  the_work->eval_values = DESIGNATED;
                  break;
                case 'n':
                  the_work->eval_values = NOTDESIGNATED;
                  break;
              }
            break;
          
          
          // Change the Minimum Designated Value
          case 'm':
            clear_scr ();
            menu_header ();
            menu_info (the_work);
            do
              {
                printf ("New Minimum Designated Value [1 - %i]: ", the_work->logic->dimension - 1);
                fgets (answer, BUFSIZ, stdin);
                sscanf (answer, "%i", &the_work->logic->mdv);
              }
            while ((the_work->logic->mdv < 1) || ((the_work->logic->mdv) > (the_work->logic->dimension - 1)));
            break;
          
          
          // Print the matrices into the screen
          case 'p':
            clear_scr ();
            show_matrices (the_work->logic);
            make_pause ();
            break;
          
          
          // Write matrices to an external file
          case 'w':
            clear_scr ();
            menu_header ();
            menu_info (the_work);
            printf (" Type the name of the file to write to: ");
            fgets (answer, BUFSIZ, stdin);
            sscanf (answer, "%s", namefile);
            
            // Look if file exists
            outfile = fopen (namefile, "r");
            if (outfile)
              {
                printf (" File \'%s\' exists, do you want to replace it? (y/N): ", namefile);
                opt = readin (answer, "y\n");
                if (opt == 'y')
                  {
                    outfile = freopen (namefile, "w", outfile);
                    if (outfile)
                      {
                        write_matrices (the_work->logic, outfile);
                        fclose (outfile);
                      }
                    else
                      {
                        printf ("\n You haven't got write permissions!\n");
                        make_pause ();
                      }
                  }
                else
                  fclose (outfile);
              }
            else
              {
                outfile = fopen (namefile, "w");
                if (outfile)
                  {
                    write_matrices (the_work->logic, outfile);
                    fclose (outfile);
                  }
                else
                  {
                    printf ("\n Couldn't write the file!\n");
                    make_pause ();
                  }
              }
            break;
          
          
          // Define a new, custom connective
          case 'n':
            clear_scr ();
            menu_header ();
            menu_info (the_work);
            printf (" Unary or Binary connective? (u/b): ");
            
            opt = readin (answer, "ub");
            switch (opt)
              {
                case 'u':
                  printf (" Unary connective name: ");
                  opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
                  if (is_unary_connective (opt, &the_work->logic->unyConns) ||
                      is_binary_connective (opt, &the_work->logic->binConns) )
                    {
                      printf (" Connective is already defined.\n");
                      break;
                    }
                  else
                    add_custom_uny_conn (opt, &the_work->logic->unyConns, the_work->logic->dimension);
                  break;
                
                case 'b':
                  printf (" Binary connective name: ");
                  opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
                  if (is_unary_connective (opt, &the_work->logic->unyConns) ||
                      is_binary_connective (opt, &the_work->logic->binConns) )
                    {
                      printf (" Connective is already defined.\n");
                      break;
                    }
                  else
                    add_custom_bin_conn (opt, &the_work->logic->binConns, the_work->logic->dimension);
                  break;
              }
            break;
          
          
          // Delete an existing connective
          case 'd':
            clear_scr ();
            menu_header ();
            menu_info (the_work);
            
            printf (" Delete connective: ");
            opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
            del_connective (opt, the_work->logic);
            break;
          
          
          // Prompt for new well formed formula and parse it
          case 'f':
            clear_scr ();
            menu_header ();
            menu_info (the_work);
            do
              {
                printf ("\n Write a formula in polish notation: ");
                fgets (answer, BUFSIZ, stdin);
                sscanf (answer, "%s", the_work->formula_pn);
              }
            while (!is_wff_pn (the_work->formula_pn, the_work->logic));
            
            if (the_work->logic->Vars)
              del_var_list (&the_work->logic->Vars);
            register_vars (the_work);
            
            if (the_work->wff)
              {
                del_wff (&the_work->wff);
                the_work->wff = NULL;
              }
            parse_polish (the_work->formula_pn, &the_work->wff, the_work->logic);
            
            break;
          
          
          // Make evaluation
          case 'e':
            if (the_work->formula_pn[0] == 0)
              printf ("\n First type a Well Formed Formula.\n");
            else
              {
                clear_scr ();
                evaluation (the_work);
              }
            make_pause ();
            break;
          
          
          // Show about page
          case 'a':
            menu_about ();
            make_pause ();
            break;
          
          
          // Show help menu
          case 'h':
            menu_help ();
            make_pause ();
            break;
          
          // Quit asking for confirmation
          case 'q':
            printf (" Are you sure? (y/N): ");
            opt = readin (answer, "y\n");
            if (opt == 'y')
              return 0;
            else
              break;
        }
    }
}

