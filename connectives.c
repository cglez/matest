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

#include "MaTest.h"


/***
  Function is_unary_connective:
   Returns true if the name of the connective in the list of unary
   connectives, otherwise returns false.
***/
bool is_unary_connective (char con_name, unyConList *list)
{
  unyCon aux = *list;
  
  while (aux)
    {
      if (aux->name == con_name)
        return true;
      else
        aux = aux->next;
    }
  
  return false;
}


/***
  Function is_binary_connective:
   Returns true if the name of the connective in the list of dyadic
   connectives, otherwise returns false.
***/
bool is_binary_connective (char con_name, binConList *list)
{
  binCon aux = *list;
  
  while (aux)
    {
      if (aux->name == con_name)
        return true;
      else
        aux = aux->next;
    }
  
  return false;
}


/***
  Function search_uny_con:
   Searches an unary connective in the list of unary connectives. Returns a
   pointer to it if exists, or a null pointer if don't.
***/
unyCon search_uny_con (char con_name, unyConList list)
{
  unyCon aux;
  aux = list;
  
  while (aux)
    {
      if (aux->name == con_name)
        return aux;
      else
        aux = aux->next;
    }
  
  return aux;
}


/***
  Function search_bin_con:
   Searches a binary connective in the list of binary connectives. Returns a
   pointer to it if exists, or a null pointer if don't.
***/
binCon search_bin_con (char con_name, binConList list)
{
  binCon aux;
  aux = list;
  
  while (aux)
    {
      if (aux->name == con_name)
        return aux;
      else
        aux = aux->next;
    }
  
  return aux;
}


/***
  Function add_unary_connective:
   Generic function for adding an unary connective with an specific name and
   matrix into a list of unary connectives.
     Returns 0: successful.
     Returns 2: error, connective is in the list.
***/
int add_unary_connective (char con_name, int *mtx, unyConList *list, int dimension)
{
  int i;
  unyCon new, aux;
  
  new = (unyCon) malloc (sizeof (unyConType));
  
  // If list is empty, new node is the first element now
  if (*list == NULL)
    {
      new->next = *list;
      *list = new;
    }
  else
    {
      aux = *list;
      // Search the last element
      while (aux->next)
        {
          // Discard elements in the list
          if (aux->name == con_name || aux->next->name == con_name)
            return 2;
          else
            aux = aux->next;
        }
        new->next = aux->next;
        aux->next = new;
    }
  
  // Define the connective elements
  new->name = con_name;
  new->matrix = (int*) calloc (dimension, sizeof (int));
  for (i = 0; i < dimension; i++)
    new->matrix[i] = mtx[i];
  
  return 0;
}


/***
  Function add_binary_connective:
   Generic function for adding a binary connective with an specific name and
   matrix into a list of binary connectives.
     Returns 0: successful.
     Returns 2: error, connective is in the list.
***/
int add_binary_connective (char con_name, int **mtx, binConList *list, int dimension)
{
  int i, j;
  binCon new, aux;
  
  new = (binCon) malloc (sizeof (binConType));
  
  // If list is empty, new node is the first element now
  if (*list == NULL)
    {
      new->next = *list;
      *list = new;
    }
  else
    {
      aux = *list;
      // Search the last element
      while (aux->next)
        {
          // Discard elements in the list
          if (aux->name == con_name || aux->next->name == con_name)
            return 2;
          else
            aux = aux->next;
        }
        new->next = aux->next;
        aux->next = new;
    }
  
  // Define the connective elements
  new->name = con_name;
  new->matrix = (int**) calloc (dimension, sizeof (int*));
  for (i = 0; i < dimension; i++)
    {
      new->matrix[i] = (int*) calloc (dimension, sizeof (int));
    }
  for (i = 0; i < dimension; i++)
    {
      for (j = 0; j < dimension; j++)
        {
          new->matrix[i][j] = mtx[i][j];
        }
    }
  return 0;
}


/***
  Procedure add_custom_uny_conn:
   Adds a new, custom unary connective into the unary connective list. Asks
   user for new values. If user press return whit no value, this position is
   set with the last prompted value.
***/
void add_custom_uny_conn (char con_name, unyConList *list, int dimension)
{
  int i;
  int *mtx;
  char value[10];
  
  mtx = (int*) calloc (dimension, sizeof (int));
  
  // Initialize all values to -1 for error control
  for (i = 0; i < dimension; i++)
    mtx[i] = -1;
  
  // Ask for new values
  for (i = 0; i < dimension; i++)
    {
      do
        {
          printf ("%c %i: ", con_name, i);
          fgets (value, 9, stdin);
          if (value[0] == '\n')
            {
              if (i == 0)
                mtx[i] = -1;
              else
                {
                  mtx[i] = mtx[i - 1];
                  printf ("%c %i: %i\n", con_name, i, mtx[i]);
                }
            }
          else
            sscanf (value, "%i", &mtx[i]);
        }
      while (mtx[i] < 0 || mtx[i] >= dimension);
    }
  
  add_unary_connective (con_name, mtx, list, dimension);
  free (mtx);
}


/***
  Procedure add_custom_bin_conn:
   Adds a new, custom binary connective into the binary connective list. Asks
   user for new values. If user press return with no value, this position is
   set with the last prompted value.
***/
void add_custom_bin_conn (char con_name, binConList *list, int dimension)
{
  int i, j;
  int **mtx;
  char value[10];
  
  mtx = (int**) calloc (dimension, sizeof (int*));
  for (i = 0; i < dimension; i++)
    mtx[i] = calloc (dimension, sizeof (int));
  
  // Initialize all the values to -1 for error control
  for (i = 0; i < dimension; i++)
    {
      for (j = 0; j < dimension; j++)
        mtx[i][j] = -1;
    }
  
  // Ask for new values
  for (i = 0; i < dimension; i++)
    {
      for (j = 0; j < dimension; j++)
        {
          do
            {
              printf ("%c %i %i: ", con_name, i, j);
              fgets (value, 8, stdin);
              if (value[0] == '\n')
                {
                  if (j == 0 && i == 0)
                    mtx[i][j] = -1;
                  else if (j == 0)
                    {
                      mtx[i][j] = mtx[i - 1][dimension - 1];
                      printf ("%c %i %i: %i\n", con_name, i, j, mtx[i][j]);
                    }
                  else
                    {
                      mtx[i][j] = mtx[i][j - 1];
                      printf ("%c %i %i: %i\n", con_name, i, j, mtx[i][j]);
                    }
                }
              else
                sscanf (value, "%i", &mtx[i][j]);
            }
          while (mtx[i][j] < 0 || mtx[i][j] >= dimension);
        }
    }
  
  add_binary_connective (con_name, mtx, list, dimension);
  free (mtx);
}


/***
  Procedure set_default_uny_conns:
   Sets the unary connectives at default; default is as in Łukasiewicz
   multivaluated logics definition. There is only a default unary connective,
   the negation connective (N).
***/
void set_default_uny_conns (Logic logic)
{
  int i;
  int *mtx;
  
  mtx = (int*) calloc (logic->dimension, sizeof (int));
  
  /* Setting the negation connective (N):
       Nx [¬x]  =df  (1 - x) */
  for (i = 0; i < logic->dimension; i++)
    mtx[i] = logic->dimension - 1 - i;
  add_unary_connective ('N', mtx, &logic->unyConns, logic->dimension);
  
  free (mtx);
}


/***
  Procedure set_default_bin_conns:
   Sets the binary connectives at default. Default is as in Łukasiewicz
   multivaluated logics definition. The default binary connectives are the
   implication connective (C), the conjunction connective (K) and the
   disjunction connective (A).
***/
void set_default_bin_conns (Logic logic)
{
  int i, j;
  int **mtx;
  
  mtx = (int**) calloc (logic->dimension, sizeof (int*));
  for (i = 0; i < logic->dimension; i++)
    mtx[i] = calloc (logic->dimension, sizeof (int));

  /* Setting the implication connective (C):
       Cxy [x->y]  =df  min {1, (1 - x) + y} */  
  for (i = 0; i < logic->dimension; i++)
    {
      for (j = 0; j < logic->dimension; j++)
        {
          if ((logic->dimension - 1) < ((logic->dimension - 1) - i + j))
            mtx[i][j] = (logic->dimension - 1);
          else
            mtx[i][j] = ((logic->dimension - 1) - i + j);
        }
    }
  add_binary_connective ('C', mtx, &logic->binConns, logic->dimension);
  
  /* Setting the conjunction connective (K):
       Kxy [x & y]  =df  min {x, y} */
  for (i = 0; i < logic->dimension; i++)
    {
      for (j = 0; j < logic->dimension; j++)
        {
          if (i < j)
            mtx[i][j] = i;
          else
            mtx[i][j] = j;
        }
    }
  add_binary_connective ('K', mtx, &logic->binConns, logic->dimension);
  
  /* Setting the disjunction connective (A):
       Axy [x v y]  =df  Max {x, y} */
  for (i = 0; i < logic->dimension; i++)
    {
      for (j = 0; j < logic->dimension; j++)
        {
          if (i > j)
            mtx[i][j] = i;
          else
            mtx[i][j] = j;
        }
    }
  add_binary_connective ('A', mtx, &logic->binConns, logic->dimension);
  
  free (mtx);
}


/***
  Procedure del_connective:
   Deletes, if exists, an unary or binary connective given by its name.
***/
void del_connective (char con_name, Logic logic)
{
  int i;
  unyCon unyprev, unyaux;
  binCon binprev, binaux;
  
  if (is_unary_connective (con_name, &logic->unyConns))
    {
      unyprev = NULL;
      unyaux = logic->unyConns;
      
      while (unyaux && unyaux->name != con_name)
        {
          unyprev = unyaux;
          unyaux = unyaux->next;
        }
      if (!unyaux || unyaux->name != con_name)
        {
          printf ("Unexpected error!\n");
          make_pause ();
          return;
        }
      else
        {
          if (!unyprev)
            logic->unyConns = unyaux->next;
          else
            unyprev->next = unyaux->next;
          
          free (unyaux->matrix);
          free (unyaux);
        }
    }
  
  else if (is_binary_connective (con_name, &logic->binConns))
    {
      binprev = NULL;
      binaux = logic->binConns;
      
      while (binaux && binaux->name != con_name)
        {
          binprev = binaux;
          binaux = binaux->next;
        }
      if (!binaux || binaux->name != con_name)
        {
          printf ("Unexpected error!\n");
          make_pause ();
          return;
        }
      else
        {
          if (!binprev)
            logic->binConns = binaux->next;
          else
            binprev->next = binaux->next;
          
          for (i = 0; i < logic->dimension; i++)
            free (binaux->matrix[i]);
          free (binaux->matrix);
          free (binaux);
        }
    }
  
  else
    {
      printf ("Connective %c doesn't exist.\n", con_name);
      make_pause ();
    }
}


/***
  Procedure print_uny_matrix:
   Prints matrix values of given unary connective in table format.
***/
void print_uny_matrix (unyCon connective, int dimension, int mdv)
{
  int i;
  
  printf ("\n");
  
  printf ("  %c |", connective->name);
  for (i = 0; i < dimension; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < dimension; i++)
    printf ("---");
  
  printf ("-\n"
          "    |");
  for (i = 0; i < dimension; i++)
    {
      if (connective->matrix[i] >= mdv)
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
void print_bin_matrix (binCon connective, int dimension, int mdv)
{
  int i,j;
  
  printf ("\n");
  
  printf ("  %c |", connective->name);
  for (i = 0; i < dimension; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < dimension; i++)
    printf ("---");
  
  printf ("-\n");
  for (i = 0; i < dimension; i++)
    {
      if (i >= mdv)
        printf (" *%i |", i);
      else
        printf ("  %i |", i);
      
      for (j = 0; j < dimension; j++)
        {
          if (connective->matrix[i][j] >= mdv)
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
  unyCon unyaux;
  binCon binaux;
  
  unyaux = logic->unyConns;
  while (unyaux)
    {
      print_uny_matrix (unyaux, logic->dimension, logic->mdv);
      unyaux = unyaux->next;
    }
  
  binaux = logic->binConns;
  while (binaux)
    {
      print_bin_matrix (binaux, logic->dimension, logic->mdv);
      binaux = binaux->next;
    }
}


/***
  Function write_uny_matrix:
   Writes given unary connective matrix into external file. Format here is
   simpler than table format, pretends to be human and machine readable.
***/
int write_uny_matrix (unyCon connective, FILE *file, int dimension)
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
int write_bin_matrix (binCon connective, FILE *file, int dimension)
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
  unyCon unyaux;
  binCon binaux;
  
  unyaux = logic->unyConns;
  while (unyaux)
    {
      write_uny_matrix (unyaux, file, logic->dimension);
      unyaux = unyaux->next;
    }
  
  binaux = logic->binConns;
  while (binaux)
    {
      write_bin_matrix (binaux, file, logic->dimension);
      binaux = binaux->next;
    }
}


/***
  Function read_matrix:
    Returns 3: error, wrong format file.
    Returns 4: error, dimension is set and isn't the same as in the file.
***/
/*
int read_matrix (FILE *file)
{
  char string[BUFSIZ];
  char con;
  int vector[256];
  int i, j, val, dim;
  
  if (file)
    {
      fgets (string, BUFSIZ, file);
      sscanf (string, "%c", &con);
      if (con > 'A' || con < 'Z')
        {
          do
            fgets (string, BUFSIZ, file);
          while (string[0] == '\n');
          for (i = 0; string[i] =! 0; i++);
            {
              sscanf (string, "%i", &val);
              vector[i] = val;
              dim++;
            }
          if (dim < 2)
            return 3;
          else if (the_logic->dimension && (dim =! the_logic->dimension))
            return 4;
          do
            {
              fgets (string, BUFSIZ, file);
            }
          while (string[0] == '\n');
        }
    }
}
*/

