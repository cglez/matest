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


/*                               variables.c
*
*    This file contains the code of functions that deals with the lists of
*    variables and its elements.
*/


#include "MaTest.h"


/***
  Function is_empty_list:
***/
int is_empty_list (varList ls)
{
  return (ls == NULL);
}


/***
  Procedure del_list:
   Frees the memory of the whole list.
***/
void del_list (varList *ls)
{
  variable aux;
  
  while (*ls)
    {
      aux = *ls;
      *ls = aux -> next;
      free (aux);
    }
}


/***
  Function is_in_list:
   True if the variable is in the list, false otherwise.
***/
bool is_in_list (varList *ls, char ch)
{
  variable aux = *ls;
  
  /* If the list is empty obviously the variable isn't in the list */
  /*
  if (is_empty_list (*ls))
    return 0;
  */
  
  /* Else we cross the whole list */
  /*
  else
    {
      while (aux)
      {
        if (aux -> name == var)
          return 1;
        else
          aux = aux -> next;
      }
    }
  */
  
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
  Procedure add_var:
   Adds a variable, if it doesn't exist, in the variable list in alphabetical
   order.
***/
void add_var (varList *ls, char var)
{
  variable new, aux;
  
  /* We dischard the elements already in the list */
  printf ("Comprobamos que no esta en la lista.\n");
  if (!is_in_list (ls, var)) {
    printf ("No esta.\n");
    return;
  }
  else
    {
      /* Create a new node */
      printf ("Ahora la añadimos.\n");
      new = (variable) malloc (sizeof (typeVar));
      new -> name = var;
  
      /* If the list is empty or the given character is previous than the
         first element, now this is the first */
      if ((is_empty_list (*ls)) || (var < (*ls) -> name))
        {
          /* We add the list after the new node */
          new -> next = *ls;
          /* Now, list begins at the new node */
          *ls = new;
        }
      /* Else we search the last element or the node with a previous character */
      else
        {
          aux = *ls;
          while ((aux -> next) && (aux -> next -> name < var))
            aux = aux -> next;
          
          new -> next = aux -> next;
          aux -> next = new;
        }
    }
}


/***
  Procedure register_vars:
   Adds all the variables present in a formula into a variable list.
***/
void register_vars (varList *list, char formula[])
{
  int i;
  
  for (i = 0; i < strlen (formula); i++)
    {
      printf ("Vamos a comprobar el símbolo %c.\n", formula[i]);
      if (symbol_type (formula[i]) == VAR)
        {
          printf ("Añadiendo variable %c.\n", formula[i]);
          add_var (list, formula[i]);
        }
      else
        printf ("%c no es una variable.\n", formula[i]);
    }
}


/***
  Function num_elements:
   Returns the number of elements.
***/
int num_elements (varList ls)
{
  int count = 0;
  varList aux = ls;
  
  if (is_empty_list (ls))
    return count;
  else
    {
      while (aux)
        {
          count++;
          aux = aux -> next;
        }
    }
  return count;
}


/***
  Function var_value:
***/
int var_value (char var, varList list)
{
  variable aux = list;
  
  if (aux) {
  
  while (aux)
    {
      if (aux -> name == var)
        return aux -> value;
      else
        aux = aux -> next;
    }
  }
  else
    return -1;
}


void del_var (varList *ls, char var)
{
  variable aux, node;
  
  node = *ls;
  aux = NULL;
  while (node && node -> name < var)
    {
      aux = node; 
      node = node -> next;
    }
    if (!node || node -> name != var)
      return;
    else
      { /* Borrar el node */
        if (!aux) /* Primer elemento */
          *ls = node -> next;
        else  /* un elemento cualquiera */
          aux -> next = node -> next;
        free (node);
      }
}


void print_list (varList ls)
{
  variable aux = ls;

  if (is_empty_list (ls))
    printf ("List vacía.\n");
  else
    {
      while (aux)
        {
          printf("%c -> ", aux -> name);
          aux = aux -> next;
        }
      printf("||\n");
    }
}

/*
int main()
{
   varList list = NULL;
   char input[BUFSIZ];
   char ch;
   int counter;
  
   do
     {
       printf ("Dame variables, hasta darme un 0: ");
       fgets (input, BUFSIZ, stdin);
       sscanf (input, "%c", &ch);
       add_var (&list, ch);
     }
   while (ch != '0');
   
   print_list (list);
   
   counter = num_elements (list);
   printf ("There is %i elements in the list.\n", counter);
   
   if (is_in_list (&list, 'p'))
     printf ("p is in list.\n");
   else
     printf ("p isn't in the list!\n");
   
   del_list (&list);

   return 0;
}
*/

