/******************************************************************************
*                                                                             *
*                           ---   MaTest 1.0   ---                            *
*                     Matrix Tester for logical matrices.                     *
*       Copyright (C) 2008, César González Gutiérrez <ceguel@gmail.com>       *
*                                                                             *
*    This file is part of MaTest.                                             *
*                                                                             *
*    MaTest is free software: you can redistribute it and/or modify           *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation, either version 3 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    MaTest is distributed in the hope that it will be useful,                *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with MaTest.  If not, see <http://www.gnu.org/licenses/>.          *
*                                                                             *
*                                                                             *
 *****************************************************************************/


/*                               variables.c
*
*    This file contains the code of functions that deals with lists of
*    variables and variables itself.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "MaTest.h"


/***
  Function is_empty_var_list:
***/
bool is_empty_var_list (LogicVarList list)
{
  return (list == NULL);
}


/***
  Procedure del_var_list:
   Deletes given variable list freeing the memory.
***/
void del_var_list (LogicVarList *list)
{
  LogicVar aux;
  
  while (*list)
    {
      aux = *list;
      *list = aux->next;
      free (aux);
    }
}


/***
  Function search_var:
   Returns a pointer to the variable given by name, otherwise returns a null
   pointer.
***/
LogicVar search_var (LogicVarList list, char variable)
{
  LogicVar aux = list;
  
  while (aux)
    {
      if (aux->name == variable)
        return aux;
      else
        aux = aux->next;
    }
  
  return aux;
}


/***
  Procedure add_var:
   Adds a variable, if it doesn't exist, in the variable list in alphabetical
   order.
***/
void add_var (LogicVarList *list, char variable)
{
  LogicVar new, aux;
  
  /* Discard  elements in the list */
  if (search_var (*list, variable))
    return;
  else
    {
      /* Create a new node */
      new = (LogicVar) malloc (sizeof (LogicVarType));
      new->name = variable;
      new->value = 0;
      
      /* If list is empty or given character is previous than the first
         element, now it is the first */
      if ((*list == NULL) || (variable < (*list)->name))
        {
          /* Add list after new node */
          new->next = *list;
          /* Now, list begins at new node */
          *list = new;
        }
      /* Else, search the last element or the node with a previous character */
      else
        {
          aux = *list;
          while (aux->next && aux->name < variable)
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
void register_vars (Logic logic, char formula[])
{
  int i;
  
  for (i = 0; i < (int) strlen (formula); i++)
    {
      if (symbol_kind_pn (formula[i], logic) == VAR)
        add_var (&logic->Vars, formula[i]);
    }
}


/***
  Function num_vars:
   Returns the number of elements in given variable list.
***/
int num_vars (LogicVarList list)
{
  int count = 0;
  LogicVarList aux = list;
  
  if (is_empty_var_list (list))
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
   Sets given value for a variable.
***/
void set_var_value (LogicVar var, int n)
{
  if (var)
    var->value = n;
  else
    {
      perror ("Variable is not in the list.");
      return;
    }
}


/***
  Function var_value:
   Returns the value of a variable in the given list of variables. If variable
   doesn't exist, returns -1.
***/
int var_value (LogicVar var)
{
  if (var)
    return var->value;
  else
    {
      perror ("Variable doesn't exists.");
      return -1;
    }
}
