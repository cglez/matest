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


/*                               evaluation.c
*
*    This file contains the global evaluation job. That contains the function
*    . Here is also the
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "MaTest.h"


/***
  Function set_atom:
   Searches, recursively, the first free connective argument into given well
   formed formula tree. Then sets type, name, and a pointer to value if it's
   a variable.
***/
bool set_atom (LogicWFF *tree, LogicSymbKind kind, char name, int *value)
{
  LogicAtom father = NULL;
  LogicAtom node = *tree;
  
  while (node)
    {
      /* Unary connective nodes has only one argument. Trivial. */
      if (node->kind == UCON)
        {
          father = node;
          node = node->postarg;
        }
      /* With binary connectives, search recursively. */
      else if (node->kind == BCON)
        {
          if (set_atom (&node->prearg, kind, name, value))
            return true;
          else if (set_atom (&node->postarg, kind, name, value))
            return true;
          else
            return false;
        }
      /* Variables hasn't got arguments. */
      else if (node->kind == VAR)
        return false;
    }

  /* If tree is empty, allocate memory and set given values. */
  if (father == NULL)
    {
      (*tree) = (LogicWFF) malloc (sizeof (LogicAtomType));
      (*tree)->kind = kind;
      (*tree)->name = name;
      (*tree)->value = value;
      (*tree)->prearg = (*tree)->postarg = NULL;
      return true;
    }
  else if (father->kind == UCON)
    {
      node = (LogicAtom) malloc (sizeof (LogicAtomType));
      father->postarg = node;
      node->kind = kind;
      node->name = name;
      node->value = value;
      node->prearg = node->postarg = NULL;
      return true;
    }
  else if (father->kind == BCON)
    {
      if (!father->prearg)
        {
          node = (LogicAtom) malloc (sizeof (LogicAtomType));
          father->prearg = node;
          node->kind = kind;
          node->name = name;
          node->value = value;
          node->prearg = node->postarg = NULL;
          return true;
        }
      else
        {
          node = (LogicAtom) malloc (sizeof (LogicAtomType));
          father->postarg = node;
          node->kind = kind;
          node->name = name;
          node->value = value;
          node->prearg = node->postarg = NULL;
          return true;
        }
    }
  else
    {
      perror ("Setting wff atom... Unexpected error!\n");
      make_pause ();
      return false;
    }
}


/***
  Procedure del_wff:
   Deletes given well formed formula tree freeing its memory.
***/
void del_wff (LogicWFF *wff)
{
  if (!(*wff))
    return;
  
  del_wff (&(*wff)->prearg);
  del_wff (&(*wff)->postarg);
  free (*wff);
}


/***
  Function eval_formula:
   Calculates, recursively, the value of given formula based on its current
   values and returns it. If well formed formula tree is empty, returns -1.
***/
int eval_formula (LogicWFF wff, Logic logic)
{
  LogicUCon ucon;
  LogicBCon bcon;
  
  if (!wff)
    {
      perror ("WFF tree is empty.\n");
      return -1;
    }
  else if (wff->kind == VAR)
    return *wff->value;
  else if (wff->kind == UCON)
    {
      ucon = (LogicUCon) search_UCon (logic->UCons, wff->name);
      return ucon->matrix[eval_formula (wff->postarg, logic)];
    }
  else if (wff->kind == BCON)
    {
      bcon = (LogicBCon) search_BCon (logic->BCons, wff->name);
      return bcon->matrix[eval_formula (wff->prearg, logic)][eval_formula(wff->postarg, logic)];
    }
  else
    {
      perror ("Evaluating current formula... Unexpected error!\n");
      return -2;
    }
}


/***
  Procedure print_current_evaluating_formula_pn:
   Prints current evaluating formula in polish notation changing variables with
   its values.
***/
void print_current_evaluating_formula_pn (char formula[], Logic logic)
{
  int i;
  
  for (i = 0; i < (int) strlen (formula); i++)
    {
      if (isupper (formula[i]))
        printf ("%c", formula[i]);
      else if (islower (formula[i]))
        printf ("%i", var_value (search_var (logic->Vars, formula[i])));
    }
}


/***
  Procedure evaluation:
   The great job. Evaluates all possibilities of the formula, prints it
   depending on evaluation type selected and finally prints statistics.
***/
void evaluation (Work work)
{
  int i, all = 0, desig = 0;
  LogicVar var;
  
  /* Print a header with the formula in polish notation */
  printf (" %s\n ", work->formula_pn);
  for (i = 0; i < (int) strlen (work->formula_pn); i++)
    printf ("-");
  printf ("\n");
  
  /*
     The counter algorithm
  */
  
  /* */
  void action (Work work, int *all, int *desig)
    {
      int i;
      
      i = eval_formula (work->wff, work->logic);
      /* Count every evaluation */
      (*all)++;
      /* Print it depending on evaluation type and count designated values */
      if (i >= work->MDV)
        {
          (*desig)++;
          if (work->eval_values == ALL || work->eval_values == DESIGNATED)
            {
               printf (" ");
               print_current_evaluating_formula_pn (work->formula_pn, work->logic);
               printf (" *%i\n", i);
            }
         }
      else
        {
          if (work->eval_values == ALL || work->eval_values == NOTDESIGNATED)
            {
              printf (" ");
              print_current_evaluating_formula_pn (work->formula_pn, work->logic);
              printf ("  %i\n", i);
            }
        }
    }
  
  /* Start condition: Initialize all variable values to 0 */
  var = work->logic->Vars;
  while (var)
    {
      var->value = 0;
      var = var->next;
    }
  /* First action with the first condition */
  action (work, &all, &desig);
  /* The counter */
  var = work->logic->Vars;
  do
    {
      if (var_value (var) < work->DIM - 1)
        {
          set_var_value (var, var_value (var) + 1);
          var = work->logic->Vars;
          action (work, &all, &desig);
        }
      else
        {
          set_var_value (var, 0);
          var = var->next;
        }
    }
  while (var);
  
  /* Print statistics */
  printf ("\n %i possibilities evaluated.\n", all);
  if (work->eval_values == ALL || work->eval_values == DESIGNATED)
    printf (" %i designated values.\n", desig);
  else
    printf (" %i not designated values.\n", all - desig);
  if (desig == all)
    printf (" Tautology.\n");
  else if (desig == 0)
    printf (" Contradiction.\n");
  else
    printf (" The matrices false this formula.\n");
}
