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
*    connectives, others for the binary connectives (ones for vectors, other
*    for marices).
*/    


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
   Searchs an unary connective in the list of unary connectives. Returns a
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
   Searchs a binary connective in the list of binary connectives. Returns a
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
   Generic function for adding an unary connective with an especific name and
   matrix into a list of unary connectives.
     Returns 0: succesfull.
     Returns 2: error, connective is in the list.
***/
int add_unary_connective (char con_name, int *mtx, unyConList *list, int dimmension)
{
  int i;
  unyCon new, aux;
  
  new = (unyCon) malloc (sizeof (unyConType));
  
  /* If the list is empty, the new node is now the first element. */
  if (*list == NULL)
    {
      new->next = *list;
      *list = new;
    }
  else
    {
      aux = *list;
      /* We search the last element */
      while (aux->next)
        {
          /* We dischard the elements already in the list */
          if (aux->name == con_name || aux->next->name == con_name)
            return 2;
          else
            aux = aux->next;
        }
        new->next = aux->next;
        aux->next = new;
    }
  new->name = con_name;
  new->matrix = (int*) calloc (dimmension, sizeof (int));
  for (i = 0; i < dimmension; i++)
    {
      new->matrix[i] = mtx[i];
    }
  
  return 0;
}


/***
  Function add_binary_connective:
   Generic function for adding a binary connective with an especific name and
   matrix into a list of binary connectives.
     Returns 0: succesfull.
     Returns 2: error, connective is in the list.
***/
int add_binary_connective (char con_name, int **mtx, binConList *list, int dimmension)
{
  int i, j;
  binCon new, aux;
  
  new = (binCon) malloc (sizeof (binConType));
  
  if (*list == NULL)
    {
      new->next = *list;
      *list = new;
    }
  else
    {
      aux = *list;
      /* We search the last element */
      while (aux->next)
        {
          /* We dischard the elements already in the list */
          if (aux->name == con_name || aux->next->name == con_name)
            return 2;
          else
            aux = aux->next;
        }
        new->next = aux->next;
        aux->next = new;
    }
  
  new->name = con_name;
  new->matrix = (int**) calloc (dimmension, sizeof (int*));
  for (i = 0; i < dimmension; i++)
    {
      new->matrix[i] = (int*) calloc (dimmension, sizeof (int));
    }
  for (i = 0; i < dimmension; i++)
    {
      for (j = 0; j < dimmension; j++)
        {
          new->matrix[i][j] = mtx[i][j];
        }
    }
  return 0;
}


/***
  Procedure add_custom_uny_conn:
***/
void add_custom_uny_conn (char con_name, unyConList *list, int dimmension)
{
  int i;
  int *mtx;
  char value[10];
  
  mtx = (int*) calloc (dimmension, sizeof (int));
  
  /* Initialice all the values to -1 for error control */
  for (i = 0; i < dimmension; i++)
    mtx[i] = -1;
  
  for (i = 0; i < dimmension; i++)
    {
      do
        {
          printf ("%c %i: ", con_name, i);
          fgets (value, 9, stdin);
          fflush (stdin);
          fflush (stdout);
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
      while (mtx[i] < 0 || mtx[i] >= dimmension);
    }
  
  add_unary_connective (con_name, mtx, list, dimmension);
  free (mtx);
}


/***
  Procedure add_custom_bin_conn:
***/
void add_custom_bin_conn (char con_name, binConList *list, int dimmension)
{
  int i, j;
  int **mtx;
  char value[10];
  
  mtx = (int**) calloc (dimmension, sizeof (int*));
  for (i = 0; i < dimmension; i++)
    mtx[i] = calloc (dimmension, sizeof (int));
  
  /* Initialice all the values to -1 for error control */
  for (i = 0; i < dimmension; i++)
    {
      for (j = 0; j < dimmension; j++)
        mtx[i][j] = -1;
    }
  
  for (i = 0; i < dimmension; i++)
    {
      for (j = 0; j < dimmension; j++)
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
                      mtx[i][j] = mtx[i - 1][dimmension - 1];
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
          while (mtx[i][j] < 0 || mtx[i][j] >= dimmension);
        }
    }
  
  add_binary_connective (con_name, mtx, list, dimmension);
  free (mtx);
}


/***
  Procedure set_default_uny_conns:
   Sets the unary connectives at default; default is as in Łuckassiewicz
   multivaluate logics definition. There is only a default unary connective,
   the negation connective (N).
***/
void set_default_uny_conns (Logic logic)
{
  int i;
  int *mtx;
  
  mtx = (int*) calloc (logic->dimmension, sizeof (int));
  
  /* Setting the negation connective (N):
       Nx [¬x]  =df  (1 - x) */
  for (i = 0; i < logic->dimmension; i++)
    mtx[i] = logic->dimmension - 1 - i;
  add_unary_connective ('N', mtx, &logic->unyConns, logic->dimmension);
  
  free (mtx);
}


/***
  Procedure set_default_bin_conns:
   Sets the binary connectives at default. Default is as in Łuckassiewicz
   multivaluate logics definition. The default binary connectives are the
   implication connective (C), the conjunction connective (K) and the
   disyunction connective (A).
***/
void set_default_bin_conns (Logic logic)
{
  int i, j;
  int **mtx;
  
  mtx = (int**) calloc (logic->dimmension, sizeof (int*));
  for (i = 0; i < logic->dimmension; i++)
    mtx[i] = calloc (logic->dimmension, sizeof (int));

  /* Setting the implication connective (C):
       Cxy [x->y]  =df  min {1, (1 - x) + y} */  
  for (i = 0; i < logic->dimmension; i++)
    {
      for (j = 0; j < logic->dimmension; j++)
        {
          if ((logic->dimmension - 1) < ((logic->dimmension - 1) - i + j))
            mtx[i][j] = (logic->dimmension - 1);
          else
            mtx[i][j] = ((logic->dimmension - 1) - i + j);
        }
    }
  add_binary_connective ('C', mtx, &logic->binConns, logic->dimmension);
  
  /* Setting the conjunction connective (K):
       Kxy [x & y]  =df  min {x, y} */
  for (i = 0; i < logic->dimmension; i++)
    {
      for (j = 0; j < logic->dimmension; j++)
        {
          if (i < j)
            mtx[i][j] = i;
          else
            mtx[i][j] = j;
        }
    }
  add_binary_connective ('K', mtx, &logic->binConns, logic->dimmension);
  
  /* Setting the disyunction connective (A):
       Axy [x v y]  =df  Max {x, y} */
  for (i = 0; i < logic->dimmension; i++)
    {
      for (j = 0; j < logic->dimmension; j++)
        {
          if (i > j)
            mtx[i][j] = i;
          else
            mtx[i][j] = j;
        }
    }
  add_binary_connective ('A', mtx, &logic->binConns, logic->dimmension);
  
  free (mtx);
}


/***
***/
void del_connective (char con_name, Logic logic)
{
  int i;
  unyCon unyprev, unyaux;
  binCon binprev, binaux;
  
  if (is_unary_connective (con_name, &logic->unyConns))
    {
      if (logic->unyConns->name == con_name)
        {
          unyprev = logic->unyConns;
          unyaux = logic->unyConns->next;
          free (unyprev->matrix);
          free (unyprev);
          logic->unyConns = unyaux;
        }
      else
        {
          unyaux = logic->unyConns;
          while (unyaux->name != con_name)
            {
              unyprev = unyaux;
              unyaux = unyaux->next;
            }
          unyprev->next = unyaux->next;
          free (unyaux->matrix);
          free (unyaux);
        }
    }
  
  else if (is_binary_connective (con_name, &logic->binConns))
    {
      if (logic->binConns->name == con_name)
        {
          binprev = logic->binConns;
          binaux = logic->binConns->next;
          for (i = 0; i < logic->dimmension; i++)
            free (binprev->matrix[i]);
          free (binprev->matrix);
          free (binprev);
          logic->binConns = binaux;
        }
      else
        {
          binaux = logic->binConns;
          while (binaux->name != con_name)
            {
              binprev = binaux;
              binaux = binaux->next;
            }
          binprev->next = binaux->next;
          for (i = 0; i < logic->dimmension; i++)
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


/*
int list_length (struct list)
{
  int count = 0;
  
  while (list->next)
    count++;
  
  return count;
}
*/


/***
  Procedure print_uny_matrix:
   Prints the matrix values of a given unary connective in table format.
***/
void print_uny_matrix (unyCon connective, int dimmension, int mdv)
{
  int i;
  
  printf ("\n");
  
  printf ("  %c |", connective->name);
  for (i = 0; i < dimmension; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < dimmension; i++)
    printf ("---");
  
  printf ("-\n"
          "    |");
  for (i = 0; i < dimmension; i++)
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
   Prints the matrix values of a given binary connective in table format.
***/
void print_bin_matrix (binCon connective, int dimmension, int mdv)
{
  int i,j;
  
  printf ("\n");
  
  printf ("  %c |", connective->name);
  for (i = 0; i < dimmension; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < dimmension; i++)
    printf ("---");
  
  printf ("-\n");
  for (i = 0; i < dimmension; i++)
    {
      if (i >= mdv)
        printf (" *%i |", i);
      else
        printf ("  %i |", i);
      
      for (j = 0; j < dimmension; j++)
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
  Procedure show_matrices:
***/
void show_matrices (Logic logic)
{
  unyCon unyaux;
  binCon binaux;
  
  unyaux = logic->unyConns;
  while (unyaux)
    {
      print_uny_matrix (unyaux, logic->dimmension, logic->mdv);
      unyaux = unyaux->next;
    }
  
  binaux = logic->binConns;
  while (binaux)
    {
      print_bin_matrix (binaux, logic->dimmension, logic->mdv);
      binaux = binaux->next;
    }
}


/***
  Function write_uny_matrix:
***/
int write_uny_matrix (unyCon connective, FILE *file, int dimmension)
{
  int i;
  
  if (file)
    {
      fprintf (file, "%c\n", connective->name);
      for (i = 0; i < dimmension; i++)
        fprintf (file, "%i ", connective->matrix[i]);
      fprintf (file, "\n\n");
      
      return 0;
    }
  else
    return 1;
}


/***
   Function write_bin_matrix:
***/
int write_bin_matrix (binCon connective, FILE *file, int dimmension)
{
  int i, j;
  
  if (file)
    {
      fprintf (file, "%c\n", connective->name);
      for (i = 0; i < dimmension; i++)
        {
          for (j = 0; j < dimmension; j++)
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
***/
void write_matrices (Logic logic, FILE *file)
{
  unyCon unyaux;
  binCon binaux;
  
  unyaux = logic->unyConns;
  while (unyaux)
    {
      write_uny_matrix (unyaux, file, logic->dimmension);
      unyaux = unyaux->next;
    }
  
  binaux = logic->binConns;
  while (binaux)
    {
      write_bin_matrix (binaux, file, logic->dimmension);
      binaux = binaux->next;
    }
}


/***
  Function read_matrix:
    Returns 3: error, wrong format file.
    Returns 4: error, dimmension is set and isn't the same as in the file.
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
          else if (the_logic->dimmension && (dim =! the_logic->dimmension))
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

