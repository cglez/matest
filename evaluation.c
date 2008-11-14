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


#include "MaTest.h"


int polk_eval (char formula[], Logic logic)
{ 
  int i, pos = 0, val1, val2;
  int *lifo;
  unyCon ucon;
  binCon bcon;
  
  lifo = calloc (strlen (formula), sizeof (int));
  
  for (i = strlen (formula) - 1; i >= 0; i--)
    {
      switch (symbol_type (formula[i]))
        {
          case VAR:
            printf ("Ponemos la variable en la pila.\n");
            lifo[pos] = var_value (formula[i]);
            pos++;
            printf ("Puesta.\n");
            break;
          case UCON:
            val1 = lifo[--pos];
            ucon = (unyCon) search_unycon (logic -> unyConns, formula[i]);
            lifo[pos++] = ucon -> matrix [val1];
          case BCON:
            val1 = lifo[--pos];
            val2 = lifo[--pos];
            bcon = (binCon) search_dyacon (logic -> binConns, formula[i]);
            lifo[pos++] = bcon -> matrix [val1][val2];
        }
    }
  
  return lifo[pos];
}

/*
FIFO create_fifo (int size)
{
  FIFO *fifo;
  fifo = (FIFO) malloc (sizeof (FIFO);
  fifo -> position = -1;
  fifo -> vector = calloc (size, sizeof (int));
  return fifo;
}
*/

/*
void del_fifo (FIFO fifo)
{
  free (fifo -> vector);
  free (fifo);
  return;
}
*/

/*
void fifo_push (int *fifo, int pos, int value)
{
  fifo[pos] = value;
}

int fifo_push (int *fifo, int pos)
{
  return fifo[pos];
}
*/

/*
void evaluate (int base, int orden, char formula[])
{
  int i, j;
  variable varnode;
  */
  /* Inicialize all the values to 0 */
  /*
  for (i = 0; i <= orden; i++)
    auxvec[i] = 0;
  
  while (varnode -> next != NULL && node -> value <= base)
    {
      for (i = 0; i < base; i++)
        {
          varnode -> value = i;
          
          for (j = orden - 1; j >= 0; j--)
            printf ("%i", auxvec[j]);
          printf ("\n");
        }
      
      for (i = 0; i < orden; i++)
        {
          if ((auxvec[i] == base - 1) && (auxvec[i + 1] < base - 1))
            {
              auxvec[i + 1]++;
              for (j = i; j >= 0; j--)
                auxvec[j] = 0;
              break;
            }
        }
    }
}
*/

/*
int main (void)
{
  int val;
  char the_formula[100];
  Logic the_logic;
  
  the_logic = (Logic) malloc (sizeof (logicType));
  
  the_logic -> dimmension = 5;
  the_logic -> mdv = the_logic -> dimmension - 1;
  the_logic -> vars = (VarList) malloc (sizeof (varType));
  // the_logic -> vars = NULL;
  
  set_default_unycons (the_logic);
  set_default_dyacons (the_logic);
  
  do
    {
      printf ("Dame una formula: ");
      scanf ("%s", the_formula);
    }
  while (!is_wff_pk (the_formula));
  printf ("OK. %i elements.\n", strlen (the_formula));
  
  register_vars (the_logic -> vars, the_formula);
  print_var_list (the_logic -> vars);
  
  val = polk_eval (the_formula);
  printf ("%i\n", val);
}
*/

