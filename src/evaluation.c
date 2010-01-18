/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * evaluation.c
 * This file is part of MaTest
 *
 * Copyright (C) 2008, 2009 - César González Gutiérrez <ceguel@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA. 
 */


/** @file evaluation.c
 *
 * El código de este archivo se encarga de realizar la evaluación según los
 * elementos que se hayen definidos.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "MaTest.h"


/**
 * Busca recursivamente el primer argumento libre en el árbol de fbf dado.
 * Después establece el nombre del elemento, el tipo y el puntero al valor
 * correspondiente si se trata de una variable.
 *
 * @return true: si tiene éxito, false: en caso contrario.
 */
bool
logics_wff_add_node (LogicsWFF *wff, LogicsSymbolType symbol_type, char symbol, int *value)
{
	LogicsWFFNode *father = NULL;
	LogicsWFFNode *node = *wff;
	
	while (node)
		{
			/* Las conectivas unarias tienen un sólo argumento: trivial */
			if (node->type == LOGICS_WFF_NODE_U_CON)
				{
					father = node;
					node = node->postarg;
				}
			/* Con las conectivas binarias buscamos recursivamente */
			else if (node->type == LOGICS_WFF_NODE_B_CON)
				{
					if (logics_wff_add_node (&node->prearg, symbol_type, symbol, value))
						return true;
					else if (logics_wff_add_node (&node->postarg, symbol_type, symbol, value))
						return true;
					else
						return false;
				}
			/* Las variables carecen de argumentos */
			else if (node->type == LOGICS_WFF_NODE_VAR)
				return false;
		}

	/* Si el árbol está vacío, reservamos memoria y establecemos los valores */
	if (father == NULL)
		{
			(*wff) = (LogicsWFF) malloc (sizeof (LogicsWFFNode));
			(*wff)->type = symbol_type;
			(*wff)->symbol = symbol;
			(*wff)->value = value;
			(*wff)->prearg = (*wff)->postarg = NULL;
			return true;
		}
	else if (father->type == LOGICS_WFF_NODE_U_CON)
		{
			node = (LogicsWFFNode*) malloc (sizeof (LogicsWFFNode));
			father->postarg = node;
			node->type = symbol_type;
			node->symbol = symbol;
			node->value = value;
			node->prearg = node->postarg = NULL;
			return true;
		}
	else if (father->type == LOGICS_WFF_NODE_B_CON)
		{
			if (!father->prearg)
				{
					node = (LogicsWFFNode*) malloc (sizeof (LogicsWFFNode));
					father->prearg = node;
					node->type = symbol_type;
					node->symbol = symbol;
					node->value = value;
					node->prearg = node->postarg = NULL;
					return true;
				}
			else
				{
					node = (LogicsWFFNode*) malloc (sizeof (LogicsWFFNode));
					father->postarg = node;
					node->type = symbol_type;
					node->symbol = symbol;
					node->value = value;
					node->prearg = node->postarg = NULL;
					return true;
				}
		}
	else
		{
			perror ("Estableciendo el elemento de una fbf... ¡Error inesperado!\n");
			return false;
		}
}


/**
 * Elimina el árbol de fórmula bien formada dado como argumento liberando su
 * memoria.
 */
void
del_wff (LogicsWFF *wff)
{
	if (!(*wff))
		return;
	del_wff (&(*wff)->prearg);
	del_wff (&(*wff)->postarg);
	free (*wff);
	*wff = NULL;
}


/**
 * Calcula recursivamente y devuelve el valor que se asigna a una fórmula
 * recorriendo el árbol de la fbf, basándose en los valores que toman sus
 * variables.
 *
 * @return n >= 0: el valor que toma la fórmula.\n
 *             -1: error, el árbol de la fbf está vacío.\n
 *             -2: error inesperado.
 */
int
eval_formula (LogicsWFF wff, LogicsLogic* logic)
{
	LogicsUCon* ucon;
	LogicsBCon* bcon;
	
	if (!wff)
		{
			perror ("El árbol de la fbf está vacío.\n");
			return -1;
		}
	else if (wff->type == LOGICS_WFF_NODE_VAR)
		return *wff->value;
	else if (wff->type == LOGICS_WFF_NODE_U_CON)
		{
			ucon = (LogicsUCon*) logics_ucon_list_get_ucon_by_symbol (logic->ucons, wff->symbol);
			return ucon->matrix[eval_formula (wff->postarg, logic)];
		}
	else if (wff->type == LOGICS_WFF_NODE_B_CON)
		{
			bcon = (LogicsBCon*) logics_bcon_list_get_bcon_by_symbol (logic->bcons, wff->symbol);
			return bcon->matrix[eval_formula (wff->prearg, logic)][eval_formula(wff->postarg, logic)];
		}
	else
		{
			perror ("Evaluando la fórmula actual... ¡Error inesperado!\n");
			return -2;
		}
}


/*
 * Procedimiento para imprimir la fórmula que se está evaluando actualmente
 * en notación polaca cambiando las variables por los valores que correspondan.
 */
char*
print_current_evaluating_formula_pn (char formula[], LogicsLogic* logic)
{
	int i;
	static char *str;

	calloc (sizeof (char), strlen (formula) + 1);
	
	for (i = 0; i < (int) strlen (formula); i++)
		{
			if (logics_symbol_pn_get_type (formula[i], logic) == LOGICS_SYMBOL_VAR)
				str[i] = (char) logics_var_get_value (logics_var_list_get_var_by_symbol (logic->vars, formula[i]));
			else
				str[i] = formula[i];
		}
	str[strlen (formula)] = '\0';

	return str;
}


void
evaluation (FILE *output, Work* work)
{
	int i, all = 0, desig = 0;
	LogicsVar *var;
	
	/* Imprime una cabecera con la fórmula en notación polaca */
	fprintf (output, " %s\n ", work->formula_pn);
	for (i = 0; i < (int) strlen (work->formula_pn); i++)
		fprintf (output, "-");
	fprintf (output, "\n");
	
	/*
	 * El algoritmo contador
	 */
	void action (Work* work, int *all, int *desig)
		{
			int i;
			
			i = eval_formula (work->wff, work->logic);
			/* Cuenta cada evaluación */
			(*all)++;
			/* Imprime una evaluación dependiendo del tipo de evaluación seleccionado
			 * y cuenta los valores designados */
			if (i >= work->MDV)
				{
					(*desig)++;
					if (work->evaluation_style == ALL || work->evaluation_style == DESIGNATED)
						{
							 fprintf (output, " ");
							 //print_current_evaluating_formula_pn (output, work->formula_pn, work->logic);
							 fprintf (output, " *%i\n", i);
						}
				 }
			else
				{
					if (work->evaluation_style == ALL || work->evaluation_style == NOT_DESIGNATED)
						{
							fprintf (output, " ");
							//print_current_evaluating_formula_pn (output, work->formula_pn, work->logic);
							fprintf (output, "  %i\n", i);
						}
				}
		}
	
	/* Condición inicial: todos los valores inicializados a 0 */
	var = work->logic->vars;
	while (var)
		{
			var->value = 0;
			var = var->next;
		}
	/* Primera acción con la primera de las condiciones */
	action (work, &all, &desig);
	/* El contador */
	var = work->logic->vars;
	do
		{
			if (logics_var_get_value (var) < work->MAXV)
				{
					logics_var_set_value (var, logics_var_get_value (var) + 1);
					var = work->logic->vars;
					action (work, &all, &desig);
				}
			else
				{
					logics_var_set_value (var, 0);
					var = var->next;
				}
		}
	while (var);
	
	/* Imprime las estadísticas */
	fprintf (output, "\n %i posibilidades evaluadas.\n", all);
	if (work->evaluation_style == ALL || work->evaluation_style == DESIGNATED)
		fprintf (output, " %i valores designados.\n", desig);
	else
		fprintf (output, " %i valores no designados.\n", all - desig);
	if (desig == all)
		fprintf (output, " TAUTOLOGÍA.\n");
	else if (desig == 0)
		fprintf (output, " CONTRADICCIÓN.\n");
	else
		fprintf (output, " Las matrices dadas FALSAN la fórmula.\n");
}


