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
	char symbol[2];

	for (i = 0; i < (int) strlen (formula); i++)
		{
			sprintf (symbol, "c", formula[i]);
			if (ll_symbol_pn_get_type (formula[i], logic) == LL_SYMBOL_VAR)
				fprintf (output, "%i", ll_var_get_value (ll_var_list_get_var_by_symbol (logic->vars, symbol)));
			else
				fprintf (output, "%c", formula[i]);
		}
}


gchar*
print_used_variables (LlLogic *logic)
{
	LlVar   *var = logic->vars;
	gchar   *strout, *straux;
	gint    length;

	length = 2 * ll_var_list_length (logic->vars) + 1;
	strout = g_new0 (gchar, length);

	while (var)
		{
			straux = g_strdup_printf ("%s ", var->symbol);
			g_strlcat (strout, straux, length);
			g_free (straux);
			var = var->next;
		}

	return strout;
}


gchar*
print_varlist_values (LlVarList varlist)
{
	LlVar    *var = varlist;
	gchar    *strout, *straux;
	gint    length;

	length = 2 * ll_var_list_length (varlist) + 1;
	strout = g_new0 (gchar, length);

	while (var)
		{
			straux = g_strdup_printf ("%x ", var->value);
			g_strlcat (strout, straux, length);
			g_free (straux);
			var = var->next;
		}

	return strout;
}


/*
 *
 */
void
print_current_variables_values (LlLogic *logic)
{
	LlVar *var = logic->vars;

	while (var)
		{
			printf ("%i ", ll_var_get_value (var));
			var = var->next;
		}
}


void
print_evaluation_header (Work *work)
{
	LlVar  *var = work->logic->vars;

	putchar (' ');

	while (var)
		{
			printf ("%s ", var->symbol);
			var = var->next;
		}
	printf ("  %s\n", work->formula_pn);

	var = work->logic->vars;
	while (var)
		{
			printf ("--");
			var = var->next;
		}
	printf ("   -\n");
}


/*
 * Acción individual del algoritmo contador
 */
void
action (FILE *output, Work* work, int *all, int *desig)
{
	int val;

	val = ll_wff_get_value (work->wff, work->logic);
	/* Cuenta cada evaluación */
	(*all)++;
	/* Imprime una evaluación dependiendo del tipo de evaluación seleccionado
	 * y cuenta los valores designados */
	if (val >= work->MDV)
		{
			(*desig)++;
			if (work->evaluation_style == ALL || work->evaluation_style == DESIGNATED)
				{
					 fprintf (output, " ");
					 //print_current_evaluating_formula_pn (output, work->formula_pn, work->logic);
					 print_current_variables_values (work->logic);
					 fprintf (output, " *%i\n", val);
				}
		 }
	else
		{
			if (work->evaluation_style == ALL || work->evaluation_style == NOT_DESIGNATED)
				{
					fprintf (output, " ");
					//print_current_evaluating_formula_pn (output, work->formula_pn, work->logic);
					print_current_variables_values (work->logic);
					fprintf (output, "  %i\n", val);
				}
		}
}


void
evaluation (FILE *output, Work* work)
{
	int i, all = 0, desig = 0;
	LlVar *var;

	/* Imprime una cabecera con la fórmula en notación polaca */
	print_evaluation_header (work);
	/*
	for (i = 0; i < ll_var_list_length (work->logic->vars); i++)
		fprintf (output, "  ");
	fprintf (output, "   %s\n ", work->formula_pn);
	*//*
	for (i = 0; i < (int) strlen (work->formula_pn); i++)
		fprintf (output, "-");
	fprintf (output, "\n");
	*/
	//print_used_variables (work->logic);

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
