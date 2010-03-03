/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * evaluation.c
 * This file is part of MaTest
 *
 * Copyright (C) 2008-2010 - César González Gutiérrez <ceguel@gmail.com>
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


/*
 * Procedimiento para imprimir la fórmula que se está evaluando actualmente
 * en notación polaca cambiando las variables por los valores que correspondan.
 */
void
print_current_evaluating_formula_pn (FILE *output, char formula[], LlLogic* logic)
{
	int i;
	
	for (i = 0; i < (int) strlen (formula); i++)
		{
			if (ll_symbol_pn_get_type (formula[i], logic) == LL_SYMBOL_VAR)
				fprintf (output, "%i", ll_var_get_value (ll_var_list_get_var_by_symbol (logic->vars, formula[i])));
			else
				fprintf (output, "%c", formula[i]);
		}
}


/*
 * Acción individual del algoritmo contador
 */
void
action (FILE *output, Work* work, int *all, int *desig)
{
	int i;
	
	i = ll_wff_get_value (work->wff, work->logic);
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
					 print_current_evaluating_formula_pn (output, work->formula_pn, work->logic);
					 fprintf (output, " *%i\n", i);
				}
		 }
	else
		{
			if (work->evaluation_style == ALL || work->evaluation_style == NOT_DESIGNATED)
				{
					fprintf (output, " ");
					print_current_evaluating_formula_pn (output, work->formula_pn, work->logic);
					fprintf (output, "  %i\n", i);
				}
		}
}

	
void
evaluation (FILE *output, Work* work)
{
	int i, all = 0, desig = 0;
	LlVar *var;
	
	/* Imprime una cabecera con la fórmula en notación polaca */
	fprintf (output, " %s\n ", work->formula_pn);
	for (i = 0; i < (int) strlen (work->formula_pn); i++)
		fprintf (output, "-");
	fprintf (output, "\n");
	
	/* Condición inicial: todos los valores inicializados a 0 */
	var = work->logic->vars;
	while (var)
		{
			var->value = 0;
			var = var->next;
		}
	/* Primera acción con la primera de las condiciones */
	action (output, work, &all, &desig);
	
	/* El contador */
	var = work->logic->vars;
	do
		{
			if (ll_var_get_value (var) < work->MAXV)
				{
					ll_var_set_value (var, ll_var_get_value (var) + 1);
					var = work->logic->vars;
					action (output, work, &all, &desig);
				}
			else
				{
					ll_var_set_value (var, 0);
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
