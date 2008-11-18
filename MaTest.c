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
  char opt, answer[BUFSIZ], namefile[BUFSIZ];
  FILE *outfile;
  Logic the_logic;
  
  the_logic = (Logic) malloc (sizeof (logicType));
  
  menu_init();
  
  do
    {
      printf ("Matrices dimmension (<1): ");
      fgets (answer, BUFSIZ, stdin);
      sscanf (answer, "%i", &the_logic->dimmension);
    }
  while (the_logic->dimmension < 2);
  
  the_logic->mdv = the_logic->dimmension - 1;
  
  set_default_uny_conns (the_logic);
  set_default_bin_conns (the_logic);
  
  /* Manage options */
  for (;;)
    {
      menu_index (the_logic);
      opt = readin (answer, "vpwndfeahq");
      
      switch (opt)
        {
          case 'v':
            clear_scr ();
            menu_header ();
            menu_info (the_logic);
            do
              {
                printf ("New Minimun Designated Value [0 - %i]: ", the_logic->dimmension - 1);
                fgets (answer, BUFSIZ, stdin);
                sscanf (answer, "%i", &the_logic->mdv);
              }
            while ((the_logic->mdv < 0) || ((the_logic->mdv) > (the_logic->dimmension - 1)));
            break;
          
          
          case 'p':
            clear_scr ();
            show_matrices (the_logic);
            make_pause ();
            break;
          
          
          case 'w':
            printf ("Type the name of the file to write to: ");
            fgets (answer, BUFSIZ, stdin);
            sscanf (answer, "%s", namefile);
            outfile = fopen (namefile, "r");
            
            if (outfile)
              {
                printf ("File exists, do you want to replace it? (y/[n]): ");
                opt = readin (answer, "y\n");
                if (opt == 'y')
                  {
                    outfile = freopen (namefile, "w", outfile);
                    if (outfile)
                      {
                        write_matrices (the_logic, outfile);
                        fclose (outfile);
                      }
                    else
                      {
                        printf ("\nYou haven't got write permissions for this file!\n");
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
                    write_matrices (the_logic, outfile);
                    fclose (outfile);
                  }
                else
                  {
                    printf ("\nCouldn't write the file!\n");
                    make_pause ();
                  }
              }
            break;
          
          
          case 'n':
            clear_scr ();
            menu_header ();
            menu_info (the_logic);
            printf ("Unary or Binary connective? (u/b): ");
            
            opt = readin (answer, "bu");
            switch (opt)
              {
                case 'u':
                  printf ("Unary connective name: ");
                  opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
                  if (is_unary_connective (opt, &the_logic->unyConns) ||
                      is_binary_connective (opt, &the_logic->binConns) )
                    {
                      printf ("Connective is already defined.\n");
                      break;
                    }
                  else
                    add_custom_uny_conn (opt, &the_logic->unyConns, the_logic->dimmension);
                  break;
                
                case 'b':
                  printf ("Binary connective name: ");
                  opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
                  if (is_unary_connective (opt, &the_logic->unyConns) ||
                      is_binary_connective (opt, &the_logic->binConns) )
                    {
                      printf ("Connective is already defined.\n");
                      break;
                    }
                  else
                    add_custom_bin_conn (opt, &the_logic->binConns, the_logic->dimmension);
                  break;
              }
            break;
          
          
          case 'd':
            clear_scr ();
            menu_header ();
            menu_info (the_logic);
            
            printf ("Delete connective: ");
            opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
            del_connective (opt, the_logic);
            break;
          
          case 'f':
            do
              {
                printf ("Write a formula in polk notation: ");
                fgets (answer, BUFSIZ, stdin);
                sscanf (answer, "%s", the_logic->formula);
              }
            while (!is_wff_pk (the_logic->formula, the_logic));
            break;
          
          
          case 'e':
            if (the_logic->formula[0] == 0)
              printf ("\nPrimero debe introducir una formula!\n");
            /*
            else
              {
                evaluate (formula);
                make_pause ();
              }
            */
            break;
          
          
          case 'a':
            break;
          
          
          case 'h':
            break;
          
          
          case 'q':
            return 0;
        }
    }
}

