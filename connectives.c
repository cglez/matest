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
      if (aux -> name == con_name)
        return true;
      else
        aux = aux -> next;
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
      if (aux -> name == con_name)
        return true;
      else
        aux = aux -> next;
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
      if (aux -> name == con_name)
        return aux;
      else
        aux = aux -> next;
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
      if (aux -> name == con_name)
        return aux;
      else
        aux = aux -> next;
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
      new -> next = *list;
      *list = new;
    }
  else
    {
      aux = *list;
      /* We search the last element */
      while (aux -> next)
        {
          /* We dischard the elements already in the list */
          if (aux -> name == con_name || aux -> next -> name == con_name)
            return 2;
          else
            aux = aux -> next;
        }
        new -> next = aux -> next;
        aux -> next = new;
    }
  new -> name = con_name;
  new -> matrix = (int*) calloc (dimmension, sizeof (int));
  for (i = 0; i < dimmension; i++)
    {
      new -> matrix[i] = mtx[i];
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
      new -> next = *list;
      *list = new;
    }
  else
    {
      aux = *list;
      /* We search the last element */
      while (aux -> next)
        {
          /* We dischard the elements already in the list */
          if (aux -> name == con_name || aux -> next -> name == con_name)
            return 2;
          else
            aux = aux -> next;
        }
        new -> next = aux -> next;
        aux -> next = new;
    }
  
  new -> name = con_name;
  new -> matrix = (int**) calloc (dimmension, sizeof (int*));
  for (i = 0; i < dimmension; i++)
    {
      new -> matrix[i] = (int*) calloc (dimmension, sizeof (int));
    }
  for (i = 0; i < dimmension; i++)
    {
      for (j = 0; j < dimmension; j++)
        {
          new -> matrix[i][j] = mtx[i][j];
        }
    }
  return 0;
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
  
  mtx = (int*) calloc (logic -> dimmension, sizeof (int));
  
  /* Setting the negation connective (N):
       Nx [¬x]  =df  (1 - x) */
  for (i = 0; i < logic -> dimmension; i++)
    mtx[i] = logic -> dimmension - 1 - i;
  add_unary_connective ('N', mtx, &logic -> unyConns, logic -> dimmension);
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
  
  mtx = (int**) calloc (logic -> dimmension, sizeof (int*));
  for (i = 0; i < logic -> dimmension; i++)
    mtx[i] = calloc (logic -> dimmension, sizeof (int));

  /* Setting the implication connective (C):
       Cxy [x -> y]  =df  min {1, (1 - x) + y} */  
  for (i = 0; i < logic -> dimmension; i++)
    {
      for (j = 0; j < logic -> dimmension; j++)
        {
          if ((logic -> dimmension - 1) < ((logic -> dimmension - 1) - i + j))
            mtx[i][j] = (logic -> dimmension - 1);
          else
            mtx[i][j] = ((logic -> dimmension - 1) - i + j);
        }
    }
  add_binary_connective ('C', mtx, &logic -> binConns, logic -> dimmension);
  
  /* Setting the conjunction connective (K):
       Kxy [x & y]  =df  min {x, y} */
  for (i = 0; i < logic -> dimmension; i++)
    {
      for (j = 0; j < logic -> dimmension; j++)
        {
          if (i < j)
            mtx[i][j] = i;
          else
            mtx[i][j] = j;
        }
    }
  add_binary_connective ('K', mtx, &logic -> binConns, logic -> dimmension);
  
  /* Setting the disyunction connective (A):
       Axy [x v y]  =df  Max {x, y} */
  for (i = 0; i < logic -> dimmension; i++)
    {
      for (j = 0; j < logic -> dimmension; j++)
        {
          if (i > j)
            mtx[i][j] = i;
          else
            mtx[i][j] = j;
        }
    }
  add_binary_connective ('A', mtx, &logic -> binConns, logic -> dimmension);
}


/*
int list_length (struct list)
{
  int count = 0;
  
  while (list -> next)
    count++;
  
  return count;
}
*/


/***
  Procedure print_uny_matrix:
   Prints the matrix values of a given unary connective in so pretty
   table like format.
***/
void print_uny_matrix (unyCon connective, int dimmension, int mdv)
{
  int i;
  
  printf ("\n");
  
  printf ("  %c |", connective -> name);
  for (i = 0; i < dimmension; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < dimmension; i++)
    printf ("---");
  
  printf ("-\n"
          "    |");
  for (i = 0; i < dimmension; i++)
    {
      if (connective -> matrix[i] >= mdv)
        printf (" *%i", connective -> matrix[i]);
      else
        printf ("  %i", connective -> matrix[i]);
    }
  printf ("\n");
}


/***
  Procedure print_bin_matrix:
   Prints the matrix values of a given binary connective in so pretty
   table like format.
***/
void print_bin_matrix (binCon connective, int dimmension, int mdv)
{
  int i,j;
  
  printf ("\n");
  
  printf ("  %c |", connective -> name);
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
          if (connective -> matrix[i][j] >= mdv)
            printf (" *%i", connective -> matrix[i][j]);
          else
            printf ("  %i", connective -> matrix[i][j]);
        }
      printf ("\n");
    }
}


/***
   Function write_uny_matrix:
***/
int write_uny_matrix (unyCon connective, char filename[], int dimmension)
{
  int i;
  
  FILE *outfile;
  outfile = fopen (filename, "a");
  
  if (outfile)
    {
      fprintf (outfile, "%c\n", connective -> name);
      for (i = 0; i < dimmension; i++)
        fprintf (outfile, "%i ", connective -> matrix[i]);
      fprintf (outfile, "\n\n");
      
      fclose (outfile);
      return 0;
    }
  else
    return 1;
}


/***
   Function write_bin_matrix:
***/
int write_bin_matrix (binCon connective, char filename[], int dimmension)
{
  int i, j;
  
  FILE *outfile;
  outfile = fopen (filename, "a");
  
  if (outfile)
    {
      fprintf (outfile, "%c\n", connective -> name);
      for (i = 0; i < dimmension; i++)
        {
          for (j = 0; j < dimmension; j++)
            fprintf (outfile, "%i ", connective -> matrix[i][j]);
          fprintf (outfile, "\n");
        }
      fprintf (outfile, "\n");
      
      fclose (outfile);
      return 0;
    }
  else
    return 1;
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
          else if (the_logic -> dimmension && (dim =! the_logic -> dimmension))
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

