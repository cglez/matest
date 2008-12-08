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

#include "MaTest.h"


/***
  Function set_atom:
   Searches, recursively, the first free connective argument into given well
   formed formula tree. Then sets type, name, and a pointer to value if it's
   a variable.
***/
bool set_atom (WFF *tree, AtomType intype, char inname, int *invalue)
{
  atom father = NULL;
  atom node = *tree;
  
  while (node)
    {
      // Unary connective nodes has only one argument, it's trivial.
      if (node->type == UCON)
        {
          father = node;
          node = node->arg1;
        }
      // With binary connectives, search recursively.
      else if (node->type == BCON)
        {
          if (set_atom (&node->arg1, intype, inname, invalue))
            return true;
          else if (set_atom (&node->arg2, intype, inname, invalue))
            return true;
          else
            return false;
        }
      // Variables hasn't got arguments.
      else if (node->type == VAR)
        return false;
    }
  
  // If tree is empty, allocate memory and set given values
  if (father == NULL)
    {
      (*tree) = (WFF) malloc (sizeof (typeAtom));
      (*tree)->type = intype;
      (*tree)->name = inname;
      (*tree)->value = invalue;
      (*tree)->arg1 = (*tree)->arg2 = NULL;
      return true;
    }
  else if (father->type == UCON)
    {
      node = (atom) malloc (sizeof (typeAtom));
      father->arg1 = node;
      node->type = intype;
      node->name = inname;
      node->value = invalue;
      node->arg1 = node->arg2 = NULL;
      return true;
    }
  else if (father->type == BCON)
    {
      if (!father->arg1)
        {
          node = (atom) malloc (sizeof (typeAtom));
          father->arg1 = node;
          node->type = intype;
          node->name = inname;
          node->value = invalue;
          node->arg1 = node->arg2 = NULL;
          return true;
        }
      else
        {
          node = (atom) malloc (sizeof (typeAtom));
          father->arg2 = node;
          node->type = intype;
          node->name = inname;
          node->value = invalue;
          node->arg1 = node->arg2 = NULL;
          return true;
        }
    }
  else
    {
      printf ("Unexpected error!\n");
      make_pause ();
    }
}


/***
  Procedure del_wff:
   Deletes given well formed formula tree freeing its memory.
***/
void del_wff (WFF *wff)
{
  if ((*wff)->arg1)
    del_wff (&(*wff)->arg1);
  if ((*wff)->arg2)
    del_wff (&(*wff)->arg2);
  free (*wff);
}


/*
void print_wff (WFF f)
{
  if (f == NULL)
    return;
  printf ("%c [%i], ", f->name, *(f->value));
  print_wff (f->arg1);
  print_wff (f->arg2);
}
*/


/***
  Function eval_formula:
   Calculates, recursively, the value of given formula based on its current
   values and returns it. If well formed formula tree is empty, returns -1.
***/
int eval_formula (WFF wff, Logic logic)
{
  unyCon ucon;
  binCon bcon;
  
  if (!wff)
    return -1;
  else if (wff->type == VAR)
    return *wff->value;
  else if (wff->type == UCON)
    {
      ucon = (unyCon) search_uny_con (wff->name, logic->unyConns);
      return ucon->matrix[eval_formula (wff->arg1, logic)];
    }
  else if (wff->type == BCON)
    {
      bcon = (binCon) search_bin_con (wff->name, logic->binConns);
      return bcon->matrix[eval_formula (wff->arg1, logic)][eval_formula(wff->arg2, logic)];
    }
  else
    printf ("Evaluating current formula. Unexpected error!\n");
}


/***
  Procedure print_eval_formula_pn:
   Prints current evaluating polish notation formula changing variables for its
   values.
***/
void print_eval_formula_pn (char formula[], Logic logic)
{
  int i;
  
  for (i = 0; i < strlen (formula); i++)
    {
      if (isupper (formula[i]))
        printf ("%c", formula[i]);
      else if (islower (formula[i]))
        printf ("%i", get_var_value (formula[i], logic->Vars));
    }
}


/***
  Procedure evaluation:
   The great job. Evaluates all possibilities of the formula, prints it
   depending on evaluation type selected and finally prints statistics.
***/
void evaluation (Work work)
{
  int i, j, all = 0, desig = 0;
  Var var, aux, last;
  
  var = work->logic->Vars;
  last = (Var) last_var (work->logic->Vars);
  
  // Initialize all variable values to 0
  while (var)
    {
      var->value = 0;
      var = var->next;
    }
  
  // Print a header with the polish notation formula
  printf (" %s\n ", work->formula_pn);
  for (i = 0; i < strlen (work->formula_pn); i++)
    printf ("-");
  printf ("\n");
  
  // The evaluation
  while (last->value < work->logic->dimension)
    {
      // Always start with the first variable
      var = work->logic->Vars;
      // Go all the first variable values
      for (i = 0; i < work->logic->dimension; i++)
        {
          set_var_value (var->name, i, work->logic->Vars);
          j = eval_formula (work->wff, work->logic);
          // Count all evaluations
          all++;
          
          // Print it depending on the evaluation type and count designated values
          if (j >= work->logic->mdv)
            {
              desig++;
              
              if (work->eval_values == ALL || work->eval_values == DESIGNATED)
                {
                  printf (" ");
                  print_eval_formula_pn (work->formula_pn, work->logic);
                  printf (" *%i\n", j);
                }
            }
          else
            {
              if (work->eval_values == ALL || work->eval_values == NOTDESIGNATED)
                {
                  printf (" ");
                  print_eval_formula_pn (work->formula_pn, work->logic);
                  printf ("  %i\n", j);
                }
            }
        }
      
      // Add 1 to the next order element when it has reached its maximum value
      while (var)
        {
          if (var->value == work->logic->dimension - 1
              && var->next
              && var->next->value < work->logic->dimension - 1)
            {
              j = get_var_value (var->next->name, work->logic->Vars);
              set_var_value (var->next->name, ++j, work->logic->Vars);
              
              aux = work->logic->Vars;
              while (aux != var->next)
                {
                  set_var_value (aux->name, 0, work->logic->Vars);
                  aux = aux->next;
                }
              break;
            }
          // The stop condition
          else if (var->value == work->logic->dimension - 1
                   && !var->next)
            var->value = work->logic->dimension;
          else
            var = var->next;
        }
    }
  
  // Print statistics
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
