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
*    This file contains the code of functions that deals with lists of
*    variables and variables itself.
*/


#include "MaTest.h"


/***
  Function is_empty_list:
***/
bool is_empty_list (VarList list)
{
  return (list == NULL);
}


/***
  Procedure del_var_list:
   Deletes given variable list freeing the memory.
***/
void del_var_list (VarList *list)
{
  Var aux;
  
  while (*list)
    {
      aux = *list;
      *list = aux->next;
      free (aux);
    }
}


/***
  Function is_in_list:
   True if the variable is in the list, false otherwise.
***/
bool is_in_list (char var_name, VarList *list)
{
  Var aux = *list;
  
  while (aux)
    {
      if (aux->name == var_name)
        return true;
      else
        aux = aux->next;
    }
  
  return false;
}


/***
  Function search_var:
   Returns a pointer to the variable given by name, otherwise returns a null
   pointer.
***/
Var search_var (char var_name, VarList list)
{
  Var aux = list;
  
  while (aux)
    {
      if (aux->name == var_name)
        return aux;
      else
        aux = aux->next;
    }
  
  return NULL;
}


/***
  Function last_var:
   Returns a pointer to the last variable into given variable list, otherwise
   returns a null pointer.
***/
Var last_var (VarList list)
{
  Var aux = list;
  
  while (aux)
    {
      if (aux->next == NULL)
        return aux;
      else
        aux = aux->next;
    }
  
  return NULL;
}


/***
  Procedure add_var:
   Adds a variable, if it doesn't exist, in the variable list in alphabetical
   order.
***/
void add_var (char var_name, VarList *list)
{
  Var new, aux;
  
  // Discard  elements in the list
  if (is_in_list (var_name, list))
    return;
  else
    {
      // Create a new node
      new = (Var) malloc (sizeof (varType));
      new->name = var_name;
      new->value = 0;
      
      /* If list is empty or given character is previous than the first
         element, now it is the first */
      if ((*list == NULL) || (var_name < (*list)->name))
        {
          // Add list after new node
          new->next = *list;
          // Now, list begins at new node
          *list = new;
        }
      // Else, search the last element or the node with a previous character
      else
        {
          aux = *list;
          while (aux->next && aux->name < var_name)
            aux = aux->next;
          
          new->next = aux->next;
          aux->next = new;
        }
    }
}


/***
  Procedure register_vars:
   Adds all the variables present in a formula into a variable list.
***/
void register_vars (Work work)
{
  int i;
  
  for (i = 0; i < strlen (work->formula_pn); i++)
    {
      if (symbol_type (work->formula_pn[i], work->logic) == VAR)
        add_var (work->formula_pn[i], &work->logic->Vars);
    }
}


/***
  Function num_vars:
   Returns the number of elements in given variable list.
***/
int num_vars (VarList list)
{
  int count = 0;
  VarList aux = list;
  
  if (is_empty_list (list))
    return count;
  else
    {
      while (aux)
        {
          count++;
          aux = aux->next;
        }
    }
  return count;
}


/***
  Procedure set_var_value:
   Sets given value for a variable, given by name, in a list of variables.
***/
void set_var_value (char var_name, int n, VarList list)
{
  Var aux = list;
  
  while (aux)
    {
      if (aux->name == var_name)
        {
          aux->value = n;
          return;
        }
      else
        aux = aux->next;
    }
  
  printf ("Variable %c is not in the list.\n", var_name);
}


/***
  Function get_var_value:
   Returns the value of a variable in the given list of variables. If variable
   doesn't exist, returns -1.
***/
int get_var_value (char var_name, VarList list)
{
  Var aux = list;
  
  while (aux)
    {
      if (aux->name == var_name)
        return aux->value;
      else
        aux = aux->next;
    }
  
  return -1;
}


/*
void del_var (varList *ls, char var)
{
  variable aux, node;
  
  node = *ls;
  aux = NULL;
  while (node && node->name < var)
    {
      aux = node; 
      node = node->next;
    }
    if (!node || node->name != var)
      return;
    else
      {
        if (!aux)
          *ls = node->next;
        else
          aux->next = node->next;
        free (node);
      }
}
*/

/*
void print_var_list (VarList list)
{
  Var aux = list;

  if (is_empty_list (list))
    printf ("Empty list.\n");
  else
    {
      while (aux)
        {
          printf(", %c", aux->name);
          aux = aux->next;
        }
      printf(".\n");
    }
}
*/

