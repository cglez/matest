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


#include "MaTest.h"


bool set_atom (WFF *tree, AtomType intype, char inname, int *invalue)
{
  atom father = NULL;
  atom node = *tree;
  
  while (node)
    {
      if (node->type == UCON)
        {
          father = node;
          node = node->arg1;
        }
      else if (node->type == BCON)
        {
          if (set_atom (&node->arg1, intype, inname, invalue))
            return true;
          else if (set_atom (&node->arg2, intype, inname, invalue))
            return true;
          else
            return false;
        }
      else if (node->type == VAR)
        return false;
    }
  
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
***/
void parse_polish (char formula[], WFF *tree, Logic logic)
{
  Var var;
  int i;
  
  for (i = 0; i < strlen (formula); i++)
    {
      if (symbol_type (formula[i], logic) == VAR)
        {
          var = (Var) search_var (formula[i], logic->Vars);
          set_atom (tree, VAR, formula[i], &var->value);
        }
      else if (symbol_type (formula[i], logic) == UCON)
        set_atom (tree, UCON, formula[i], NULL);
      else if (symbol_type (formula[i], logic) == BCON)
        set_atom (tree, BCON, formula[i], NULL);
      else
        {
          printf ("Parsing... Unexpected error!\n");
          make_pause ();
          return;
        }
    }
}


/***
***/
void del_wff (WFF *wff)
{
  if (*wff == NULL)
    return;
  if ((*wff)->arg1)
    del_wff (&(*wff)->arg1);
  if ((*wff)->arg2)
    del_wff (&(*wff)->arg2);
  free (wff);
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
    printf ("Evaluating current formula. Unexpected error!");
}


/***
***/
void print_eval_formula (char formula[], Logic logic)
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
***/
void evaluate (Work work)
{
  int i, j, all = 0, nodesig = 0;
  Var var, aux, last;
  
  var = work->logic->Vars;
  last = (Var) last_var (work->logic->Vars);
  
  while (var)
    {
      var->value = 0;
      var = var->next;
    }
  
  printf ("%s\n", work->pol_formula);
  for (i = 0; i < strlen (work->pol_formula); i++)
    printf ("-");
  printf ("\n");
  
  while (last->value < work->logic->dimmension)
    {
      var = work->logic->Vars;
      for (i = 0; i < work->logic->dimmension; i++)
        {
          set_var_value (var->name, i, work->logic->Vars);
          j = eval_formula (work->wff, work->logic);
          all++;
          
          if (j < work->logic->mdv)
            {
              nodesig++;
              
              if (work->eval_values == ALL || work->eval_values == NOTDESIGNATED)
                {
                  print_eval_formula (work->pol_formula, work->logic);
                  printf ("  %i\n", j);
                }
            }
          else
            {
              if (work->eval_values == ALL || work->eval_values == DESIGNATED)
                {
                  print_eval_formula (work->pol_formula, work->logic);
                  printf (" *%i\n", j);
                }
            }
        }
      
      while (var)
        {
          if (var->value == work->logic->dimmension - 1
              && var->next
              && var->next->value < work->logic->dimmension - 1)
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
          else if (var->value == work->logic->dimmension - 1
                   && !var->next)
            var->value = work->logic->dimmension;
          else
            var = var->next;
        }
    }
  printf ("\n %i posibilities evaluated.\n", all);
  printf (" %i not designated values.\n", nodesig);
  if (nodesig == 0)
    printf (" Tautology.\n");
  else if (nodesig == all)
    printf (" Contradiction.\n");
  else
    printf (" The matrices falses this formula.\n");
}


/*
int eval_rpn (char formula[], Logic logic)
{ 
  int i, pos = 0, val1, val2;
  int *lifo;
  unyCon ucon;
  binCon bcon;
  
  lifo = calloc (strlen (formula), sizeof (int));
  
  for (i = strlen (formula) - 1; i >= 0; i--)
    {
      switch (symbol_type (formula[i]))
        {
          case VAR:
            printf ("Ponemos la variable en la pila.\n");
            lifo[pos] = var_value (formula[i]);
            pos++;
            printf ("Puesta.\n");
            break;
          case UCON:
            val1 = lifo[--pos];
            ucon = (unyCon) search_unycon (logic->unyConns, formula[i]);
            lifo[pos++] = ucon->matrix [val1];
          case BCON:
            val1 = lifo[--pos];
            val2 = lifo[--pos];
            bcon = (binCon) search_dyacon (logic->binConns, formula[i]);
            lifo[pos++] = bcon->matrix [val1][val2];
        }
    }
  
  return lifo[pos];
}
*/


/*
FIFO create_fifo (int size)
{
  FIFO *fifo;
  fifo = (FIFO) malloc (sizeof (FIFO);
  fifo->position = -1;
  fifo->vector = calloc (size, sizeof (int));
  return fifo;
}
*/

/*
void del_fifo (FIFO fifo)
{
  free (fifo->vector);
  free (fifo);
  return;
}
*/

/*
void fifo_push (int *fifo, int pos, int value)
{
  fifo[pos] = value;
}

int fifo_push (int *fifo, int pos)
{
  return fifo[pos];
}
*/

/*
void evaluate (int base, int orden, char formula[])
{
  int i, j;
  variable varnode;
  */
  /* Inicialize all the values to 0 */
  /*
  for (i = 0; i <= orden; i++)
    auxvec[i] = 0;
  
  while (varnode->next != NULL && node->value <= base)
    {
      for (i = 0; i < base; i++)
        {
          varnode->value = i;
          
          for (j = orden - 1; j >= 0; j--)
            printf ("%i", auxvec[j]);
          printf ("\n");
        }
      
      for (i = 0; i < orden; i++)
        {
          if ((auxvec[i] == base - 1) && (auxvec[i + 1] < base - 1))
            {
              auxvec[i + 1]++;
              for (j = i; j >= 0; j--)
                auxvec[j] = 0;
              break;
            }
        }
    }
}
*/

/*
int main (void)
{
  int val;
  char the_formula[100];
  Logic the_logic;
  
  the_logic = (Logic) malloc (sizeof (logicType));
  
  the_logic->dimmension = 5;
  the_logic->mdv = the_logic->dimmension - 1;
  the_logic->vars = (VarList) malloc (sizeof (varType));
  // the_logic->vars = NULL;
  
  set_default_unycons (the_logic);
  set_default_dyacons (the_logic);
  
  do
    {
      printf ("Dame una formula: ");
      scanf ("%s", the_formula);
    }
  while (!is_wff_pk (the_formula));
  printf ("OK. %i elements.\n", strlen (the_formula));
  
  register_vars (the_logic->vars, the_formula);
  print_var_list (the_logic->vars);
  
  val = polk_eval (the_formula);
  printf ("%i\n", val);
}
*/

