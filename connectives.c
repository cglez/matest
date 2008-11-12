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
bool is_unary_connective (char ch, unyConList *list)
{
  unyCon aux = *list;
  
  while (aux)
    {
      if (aux -> name == ch)
        return true;
      else
        aux = aux -> next;
    }
  
  return false;
}


/***
  Function is_dyadic_connective:
   Returns true if the name of the connective in the list of dyadic
   connectives, otherwise returns false.
***/
bool is_dyadic_connective (char ch, dyaConList *list)
{
  dyaCon aux = *list;
  
  while (aux)
    {
      if (aux -> name == ch)
        return true;
      else
        aux = aux -> next;
    }
  
  return false;
}


/***
  Function search_unycon:
   Searchs an unary connective in the list of unary connectives. Returns a
   pointer to it if exists, or a null pointer if don't.
***/
unyCon search_unycon (unyConList unylist, char con)
{
  unyCon aux;
  aux = unylist;
  
  while (aux)
    {
      if (aux -> name == con)
        return aux;
      else
        aux = aux -> next;
    }
  
  return aux;
}


/***
  Function search_dyacon:
   Searchs a binary connective in the list of binary connectives. Returns a
   pointer to it if exists, or a null pointer if don't.
***/
dyaCon search_dyacon (dyaConList dyalist, char con)
{
  dyaCon aux;
  aux = dyalist;
  
  while (aux)
    {
      if (aux -> name == con)
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
int add_unary_connective (char ch, unyConList *list, int *mtx)
{
  unyCon new, prev;
  int i;
  
  new = (unyCon) malloc (sizeof (unyConType));
  
  /* If the list is empty, the new node is now the first element. */
  if (*list == NULL)
    {
      new -> next = *list;
      *list = new;
    }
  else
    {
      prev = *list;
      /* We search the last element */
      while (prev -> next)
        {
          /* We dischard the elements already in the list */
          if (prev -> name == ch || prev -> next -> name == ch)
            return 2;
          else
            prev = prev -> next;
        }
        new -> next = prev -> next;
        prev -> next = new;
    }
  new -> name = ch;
  new -> matrix = (int*) calloc (the_logic -> dimmension, sizeof (int));
  for (i = 0; i < the_logic -> dimmension; i++)
    {
      new -> matrix[i] = mtx[i];
    }
  
  return 0;
}


/***
  Function add_dyadic_connective:
   Generic function for adding a dyadic connective with an especific name and
   matrix into a list of dyadic connectives.
     Returns 0: succesfull.
     Returns 2: error, connective is in the list.
***/
int add_dyadic_connective (char ch, dyaConList *list, int **mtx)
{
  dyaCon new, prev;
  int i, j;
  
  new = (dyaCon) malloc (sizeof (dyaConType));
  
  if (*list == NULL)
    {
      new -> next = *list;
      *list = new;
    }
  else
    {
      prev = *list;
      /* We search the last element */
      while (prev -> next)
        {
          /* We dischard the elements already in the list */
          if (prev -> name == ch || prev -> next -> name == ch)
            return 2;
          else
            prev = prev -> next;
        }
        new -> next = prev -> next;
        prev -> next = new;
    }
  
  new -> name = ch;
  new -> matrix = (int**) calloc (the_logic -> dimmension, sizeof (int*));
  for (i = 0; i < the_logic -> dimmension; i++)
    {
      new -> matrix[i] = (int*) calloc (the_logic -> dimmension, sizeof (int));
    }
  for (i = 0; i < the_logic -> dimmension; i++)
    {
      for (j = 0; j < the_logic -> dimmension; j++)
        {
          new -> matrix[i][j] = mtx[i][j];
        }
    }
  return 0;
}


/***
  Procedure set_default_unycons:
   Sets the unary connectives at default; default is as in Łuckassiewicz
   multivaluate logics definition. There is only a default unary connective,
   the negation connective (N).
***/
void set_default_unycons (Logic logic)
{
  int i;
  int *mtx;
  
  mtx = (int*) calloc (the_logic -> dimmension, sizeof (int));
  
  /* Setting the negation connective (N):
       Nx [¬x]  =df  (1 - x) */
  for (i = 0; i < the_logic -> dimmension; i++)
    mtx[i] = the_logic -> dimmension - 1 - i;
  add_unary_connective ('N', &the_logic -> unyconns, mtx);
}


/***
  Procedure set_default_dyacons:
   Sets the dyadic connectives at default. Default is as in Łuckassiewicz
   multivaluate logics definition. The default binary connectives are the
   implication connective (C), the conjunction connective (K) and the
   disyunction connective (A).
***/
void set_default_dyacons (Logic logic)
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
  add_dyadic_connective ('C', &logic -> dyaconns, mtx);
  
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
  add_dyadic_connective ('K', &logic -> dyaconns, mtx);
  
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
  add_dyadic_connective ('A', &logic -> dyaconns, mtx);
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
void print_uny_matrix (unyCon connective)
{
  int i;
  
  printf ("\n");
  
  printf ("  %c |", connective -> name);
  for (i = 0; i < the_logic -> dimmension; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < the_logic -> dimmension; i++)
    printf ("---");
  
  printf ("-\n"
          "    |");
  for (i = 0; i < the_logic -> dimmension; i++)
    {
      if (connective -> matrix[i] >= the_logic -> mdv)
        printf (" *%i", connective -> matrix[i]);
      else
        printf ("  %i", connective -> matrix[i]);
    }
  printf ("\n");
}


/***
  Procedure print_dya_matrix:
   Prints the matrix values of a given dyadic connective in so pretty
   table like format.
***/
void print_dya_matrix (dyaCon connective)
{
  int i,j;
  
  printf ("\n");
  
  printf ("  %c |", connective -> name);
  for (i = 0; i < the_logic -> dimmension; i++)
    printf ("  %i", i);
  
  printf ("\n----+");
  for (i = 0; i < the_logic -> dimmension; i++)
    printf ("---");
  
  printf ("-\n");
  for (i = 0; i < the_logic -> dimmension; i++)
    {
      if (i >= the_logic -> mdv)
        printf (" *%i |", i);
      else
        printf ("  %i |", i);
      
      for (j = 0; j < the_logic -> dimmension; j++)
        {
          if (connective -> matrix[i][j] >= the_logic -> mdv)
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
int write_uny_matrix (unyCon connective, char filename[])
{
  int i;
  
  FILE *outfile;
  outfile = fopen (filename, "a");
  
  if (outfile)
    {
      fprintf (outfile, "%c\n", connective -> name);
      for (i = 0; i < the_logic -> dimmension; i++)
        fprintf (outfile, "%i ", connective -> matrix[i]);
      fprintf (outfile, "\n\n");
      
      fclose (outfile);
      return 0;
    }
  else
    return 1;
}


/***
   Function write_dya_matrix:
   
***/
int write_dya_matrix (dyaCon connective, char filename[])
{
  int i, j;
  
  FILE *outfile;
  outfile = fopen (filename, "a");
  
  if (outfile)
    {
      fprintf (outfile, "%c\n", connective -> name);
      for (i = 0; i < the_logic -> dimmension; i++)
        {
          for (j = 0; j < the_logic -> dimmension; j++)
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
int read_matrix (FILE *file)
{
  char string[BUFSIZ];
  char conn;
  int vector[256];
  int i, j, val, dim;
  
  if (file)
    {
      fgets (string, BUFSIZ, file);
      sscanf (string, "%c", &conn);
      if (conn > 'A' || conn < 'Z')
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

/*
int main (void)
{
  char conn;
  the_logic = (Logic) malloc (sizeof (logicType));
  
  the_logic -> dimmension = 5;
  the_logic -> mdv = the_logic -> dimmension - 1;
  
  set_default_unycons (the_logic);
  set_default_diacons (the_logic);
  
  printf ("Qué conectiva muestro: ");
  scanf ("%c", &conn);
  conn = toupper (conn);
  if (search_unycon (the_logic -> unyconnectives, conn))
    print_uny_matrix (search_unycon (the_logic -> unyconnectives, conn));
  else if (search_diacon (the_logic -> diaconnectives, conn))
    print_dia_matrix (search_diacon (the_logic -> diaconnectives, conn));
  else
    printf ("La conectiva no existe.\n");
  return 0;
}
*/

