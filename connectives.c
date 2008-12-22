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


/*                               connectives.c
*
*    This file contains the code that deals with connectives, its creation,
*    definition, etc.
*    The most functions in this file comes in pairs, ones for the unary
*    connectives, others for the binary connectives (ones for vectors, others
*    for matrices).
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "MaTest.h"


/***
  Function search_UCon:
   Searches an unary connective in the list of unary connectives. Returns a
   pointer to it if exists, or a null pointer if don't.
***/
LogicUCon search_UCon (LogicUConList list, char name)
{
  LogicUCon aux;
  aux = list;

  while (aux)
    {
      if (aux->name == name)
        return aux;
      else
        aux = aux->next;
    }

  return aux;
}


/***
  Function search_BCon:
   Searches a binary connective in the list of binary connectives. Returns a
   pointer to it if exists, or a null pointer if don't.
***/
LogicBCon search_BCon (LogicBConList list, char name)
{
  LogicBCon aux;
  aux = list;
  
  while (aux)
    {
      if (aux->name == name)
        return aux;
      else
        aux = aux->next;
    }
  
  return aux;
}


/***
  Function add_UCon:
   Generic function for adding an unary connective with an specific name and
   matrix into a list of unary connectives.
     Returns 0: successful.
     Returns 2: error, connective is in the list.
***/
int add_UCon (LogicUConList *list, char name, int *mtx, int dimension)
{
  int i;
  LogicUCon new, aux;
  
  new = (LogicUCon) malloc (sizeof (LogicUConType));
  
  /* If list is empty, now new node is the first element */
  if (*list == NULL)
    {
      new->next = *list;
      *list = new;
    }
  else
    {
      aux = *list;
      /* Search the last element */
      while (aux->next)
        {
          /* Discard elements in the list */
          if (aux->name == name || aux->next->name == name)
            return 2;
          else
            aux = aux->next;
        }
        new->next = aux->next;
        aux->next = new;
    }
  
  /* Define the connective elements */
  new->name = name;
  new->matrix = (int*) calloc (dimension, sizeof (int));
  for (i = 0; i < dimension; i++)
    new->matrix[i] = mtx[i];
  
  return 0;
}


/***
  Function add_BCon:
   Generic function for adding a binary connective with an specific name and
   matrix into a list of binary connectives.
     Returns 0: successful.
     Returns 2: error, connective is in the list.
***/
int add_BCon (LogicBConList *list, char name, int **mtx, int dimension)
{
  int i, j;
  LogicBCon new, aux;
  
  new = (LogicBCon) malloc (sizeof (LogicBConType));
  
  /* If list is empty, new node is the first element now */
  if (*list == NULL)
    {
      new->next = *list;
      *list = new;
    }
  else
    {
      aux = *list;
      /* Search the last element */
      while (aux->next)
        {
          /* Discard elements in the list */
          if (aux->name == name || aux->next->name == name)
            return 2;
          else
            aux = aux->next;
        }
        new->next = aux->next;
        aux->next = new;
    }
  
  /* Define the connective elements */
  new->name = name;
  new->matrix = (int**) calloc (dimension, sizeof (int*));
  for (i = 0; i < dimension; i++)
    new->matrix[i] = (int*) calloc (dimension, sizeof (int));
  for (i = 0; i < dimension; i++)
    {
      for (j = 0; j < dimension; j++)
        new->matrix[i][j] = mtx[i][j];
    }
  return 0;
}


/***
  Procedure add_custom_UCon:
   Adds a new, custom unary connective into the unary connective list. Asks
   user for new values. If user press return whit no value, this position is
   set with the last prompted value.
***/
void add_custom_UCon (LogicUConList *list, char name, int dimension)
{
  int i;
  int *mtx;
  char value[10];
  
  mtx = (int*) calloc (dimension, sizeof (int));
  
  /* Initialize all values to -1 for error control */
  for (i = 0; i < dimension; i++)
    mtx[i] = -1;
  
  /* Ask for new values */
  for (i = 0; i < dimension; i++)
    {
      do
        {
          printf ("%c %i: ", name, i);
          fgets (value, 9, stdin);
          if (value[0] == '\n')
            {
              if (i == 0)
                mtx[i] = -1;
              else
                {
                  mtx[i] = mtx[i - 1];
                  printf ("%c %i: %i\n", name, i, mtx[i]);
                }
            }
          else
            sscanf (value, "%i", &mtx[i]);
        }
      while (mtx[i] < 0 || mtx[i] >= dimension);
    }
  
  add_UCon (list, name, mtx, dimension);
  free (mtx);
}


/***
  Procedure add_custom_BCon:
   Adds a new, custom binary connective into binary connective list. Asks
   user for new values. If user press return with no value, this position is
   set with the last prompted value.
***/
void add_custom_BCon (Logic logic, char connective)
{
  int i, j;
  int **mtx;
  char value[10];

  mtx = (int**) calloc (DIM, sizeof (int*));
  for (i = 0; i < DIM; i++)
    mtx[i] = calloc (DIM, sizeof (int));

  /* Initialize all the values to -1 for error control */
  for (i = 0; i < DIM; i++)
    {
      for (j = 0; j < DIM; j++)
        mtx[i][j] = -1;
    }

  /* Ask for new values */
  for (i = 0; i < DIM; i++)
    {
      for (j = 0; j < DIM; j++)
        {
          do
            {
              printf (" %c %i %i: ", connective, i, j);
              fgets (value, 8, stdin);
              if (value[0] == '\n')
                {
                  if (j == 0 && i == 0)
                    mtx[i][j] = -1;
                  else if (j == 0)
                    {
                      mtx[i][j] = mtx[i - 1][logic->dim - 1];
                      printf (" %c %i %i: %i\n", connective, i, j, mtx[i][j]);
                    }
                  else
                    {
                      mtx[i][j] = mtx[i][j - 1];
                      printf (" %c %i %i: %i\n", connective, i, j, mtx[i][j]);
                    }
                }
              else
                sscanf (value, "%i", &mtx[i][j]);
            }
          while (mtx[i][j] < 0 || mtx[i][j] >= DIM);
        }
    }

  add_BCon (&logic->BCons, connective, mtx, logic->dim);
  free (mtx);
}


/***
  Procedure set_default_UCons:
   Sets the unary connectives at default; default is as in Łukasiewicz
   multivaluated logics definition. There is only a default unary connective,
   the negation connective (N).
***/
void set_default_UCons (Logic logic)
{
  int i;
  int *mtx;

  mtx = (int*) calloc (DIM, sizeof (int));

  /* Setting the negation connective (N):
       Nx [¬x]  =df  (1 - x) */
  for (i = 0; i < DIM; i++)
    mtx[i] = logic->dim - 1 - i;
  add_UCon (&logic->UCons, 'N', mtx, logic->dim);

  free (mtx);
}


/***
  Procedure set_default_BCons:
   Sets the binary connectives at default. Default is as in Łukasiewicz
   multivaluated logics definition. The default binary connectives are the
   implication connective (C), the conjunction connective (K) and the
   disjunction connective (A).
***/
void set_default_BCons (Logic logic)
{
  int i, j;
  int **mtx;
  
  mtx = (int**) calloc (DIM, sizeof (int*));
  for (i = 0; i < DIM; i++)
    mtx[i] = calloc (DIM, sizeof (int));

  /* Setting the implication connective (C):
       Cxy [x->y]  =df  min {n, (n - x) + y} */
  for (i = 0; i < DIM; i++)
    {
      for (j = 0; j < DIM; j++)
        mtx[i][j] = MIN ((DIM - 1), (DIM - 1 - i + j));
    }
  add_BCon (&logic->BCons, 'C', mtx, logic->dim);

  /* Setting the conjunction connective (K):
       Kxy [x & y]  =df  min {x, y} */
  for (i = 0; i < DIM; i++)
    {
      for (j = 0; j < DIM; j++)
        mtx[i][j] = MIN (i, j);
    }
  add_BCon (&logic->BCons, 'K', mtx, logic->dim);

  /* Setting the disjunction connective (A):
       Axy [x v y]  =df  Max {x, y} */
  for (i = 0; i < DIM; i++)
    {
      for (j = 0; j < DIM; j++)
        mtx[i][j] = MAX (i, j);
    }
  add_BCon (&logic->BCons, 'A', mtx, logic->dim);
  
  free (mtx);
}


/***
  Procedure del_connective:
   Deletes, if exists, an unary or binary connective given by its name.
***/
void del_connective (Logic logic, char connective)
{
  int i;
  LogicUCon unyprev, unyaux;
  LogicBCon binprev, binaux;
  
  if (search_UCon (logic->UCons, connective))
    {
      unyprev = NULL;
      unyaux = logic->UCons;
      
      while (unyaux && unyaux->name != connective)
        {
          unyprev = unyaux;
          unyaux = unyaux->next;
        }
      if (!unyaux || unyaux->name != connective)
        {
          printf ("Unexpected error!\n");
          make_pause ();
          return;
        }
      else
        {
          if (!unyprev)
            logic->UCons = unyaux->next;
          else
            unyprev->next = unyaux->next;
          
          free (unyaux->matrix);
          free (unyaux);
        }
    }
  
  else if (search_BCon (logic->BCons, connective))
    {
      binprev = NULL;
      binaux = logic->BCons;
      
      while (binaux && binaux->name != connective)
        {
          binprev = binaux;
          binaux = binaux->next;
        }
      if (!binaux || binaux->name != connective)
        {
          printf ("Unexpected error!\n");
          make_pause ();
          return;
        }
      else
        {
          if (!binprev)
            logic->BCons = binaux->next;
          else
            binprev->next = binaux->next;
          
          for (i = 0; i < DIM; i++)
            free (binaux->matrix[i]);
          free (binaux->matrix);
          free (binaux);
        }
    }
  
  else
    {
      printf ("Connective %c doesn't exist.\n", connective);
      make_pause ();
    }
}


/***
  Procedure print_uny_matrix:
   Prints matrix values of given unary connective in table format.
***/
void print_uny_matrix (LogicUCon connective, Logic logic)
{
  int i;
  
  printf ("\n");
  
  printf ("  %c |", connective->name);
  for (i = 0; i < DIM; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < DIM; i++)
    printf ("---");
  
  printf ("-\n"
          "    |");
  for (i = 0; i < DIM; i++)
    {
      if (connective->matrix[i] >= logic->mdv)
        printf (" *%i", connective->matrix[i]);
      else
        printf ("  %i", connective->matrix[i]);
    }
  printf ("\n");
}


/***
  Procedure print_bin_matrix:
   Prints matrix values of given binary connective in table format.
***/
void print_bin_matrix (LogicBCon connective, Logic logic)
{
  int i, j;
  
  printf ("\n");
  
  printf ("  %c |", connective->name);
  for (i = 0; i < DIM; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < DIM; i++)
    printf ("---");
  
  printf ("-\n");
  for (i = 0; i < DIM; i++)
    {
      if (i >= logic->mdv)
        printf (" *%i |", i);
      else
        printf ("  %i |", i);

      for (j = 0; j < DIM; j++)
        {
          if (connective->matrix[i][j] >= logic->mdv)
            printf (" *%i", connective->matrix[i][j]);
          else
            printf ("  %i", connective->matrix[i][j]);
        }
      printf ("\n");
    }
}


/***
  Procedure show_matrices
   Prints all the connective matrices into the screen, first unary connectives,
   second binary ones.
***/
void show_matrices (Logic logic)
{
  LogicUCon unyaux;
  LogicBCon binaux;
  
  unyaux = logic->UCons;
  while (unyaux)
    {
      print_uny_matrix (unyaux, logic);
      unyaux = unyaux->next;
    }
  
  binaux = logic->BCons;
  while (binaux)
    {
      print_bin_matrix (binaux, logic);
      binaux = binaux->next;
    }
}


/***
  Function write_uny_matrix:
   Writes given unary connective matrix into external file. Format here is
   simpler than table format, pretends to be human and machine readable.
***/
int write_uny_matrix (FILE *file, LogicUCon connective, int dimension)
{
  int i;
  
  if (file)
    {
      fprintf (file, "%c\n", connective->name);
      for (i = 0; i < dimension; i++)
        fprintf (file, "%i ", connective->matrix[i]);
      fprintf (file, "\n\n");
      
      return 0;
    }
  else
    return 1;
}


/***
  Function write_bin_matrix:
   Writes given binary connective matrix into external file. Format here is
   simpler than table format, pretends to be human and machine readable.
***/
int write_bin_matrix (FILE *file, LogicBCon connective, int dimension)
{
  int i, j;
  
  if (file)
    {
      fprintf (file, "%c\n", connective->name);
      for (i = 0; i < dimension; i++)
        {
          for (j = 0; j < dimension; j++)
            fprintf (file, "%i ", connective->matrix[i][j]);
          fprintf (file, "\n");
        }
      fprintf (file, "\n");
      
      return 0;
    }
  else
    return 1;
}


/***
  Procedure write_matrices:
   Writes all matrices into external file in simple format, first unary
   connectives, second binary ones.
***/
void write_matrices (Logic logic, FILE *file)
{
  LogicUCon unyaux;
  LogicBCon binaux;
  
  unyaux = logic->UCons;
  while (unyaux)
    {
      write_uny_matrix (file, unyaux, DIM);
      unyaux = unyaux->next;
    }
  
  binaux = logic->BCons;
  while (binaux)
    {
      write_bin_matrix (file, binaux, DIM);
      binaux = binaux->next;
    }
}
