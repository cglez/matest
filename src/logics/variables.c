/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * variables.c
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


/**
 * @file variables.c
 *
 * Este archivo contiene las funciones encargadas de manejar las variables
 * proposicionales y sus listas.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "logics.h"


/**
 * Crea una variable proposicional nueva.
 *
 * @param symbol Letra o símbolo que representa la variable.
 * @param value Valor asignado a la variable.
 * @return La nueva variable proposicional.
 */
LlVar*
ll_var_new (char* symbol, int value)
{
	LlVar *var;

	var = (LlVar*) malloc (sizeof (LlVar));
	if (!var)
		{
			perror ("liblogics: Error al reservar memoria para una variable proposicional.\n");
			return NULL;
		}
	else
		{
			var->value = value;
			strcpy (var->symbol, symbol);
			return var;
		}
}


/**
 * Establece el valor dado en una variable proposicional.
 */
void
ll_var_set_value (LlVar *var, int value)
{
	if (var)
		var->value = value;
	else
    perror ("liblogics: La variable no existe.\n");
}


/**
 * Devuelve el valor de una variable proposicional.
 *
 * @return \f$n \geq 0\f$ : el valor de la variable.\n
 *         -1: error, la variable no existe.
 */
int
ll_var_get_value (LlVar *var)
{
	if (var)
		return var->value;
	else
		{
			perror ("liblogics: La variable no existe.\n");
			return -1;
		}
}


bool
ll_var_list_is_empty (LlVarList var_list)
{
	return (var_list == NULL);
}


/**
 * Procedimiento que elimina una lista de variables por completo liberando la
 * memoria reservada.
 */
void
ll_var_list_free (LlVarList *var_list)
{
	LlVar *var;

	while (*var_list)
		{
			var = *var_list;
			*var_list = var->next;
			free (var);
		}
}


/**
 * Busca una variable dada por su símbolo en una lista de variables.
 *
 * @return Puntero a la variable si existe, o el puntero nulo en caso contrario.
 */
LlVar*
ll_var_list_get_var_by_symbol (LlVarList var_list, char* symbol)
{
	LlVar *var = var_list;

	while (var)
		{
			if (!strcmp (var->symbol, symbol))
				return var;
			else
				var = var->next;
		}
	return var;
}


/**
 * Añade una variable proposicional a una lista si no existe ya una con ese
 * nombre, y la sitúa por orden alfabético, con lo que la lista queda ordenada.
 */
void
ll_var_list_add_var (LlVarList *var_list, LlVar* var)
{
	LlVar *aux;

	/* No duplicamos las variables que ya existen */
	if (!ll_var_list_get_var_by_symbol (*var_list, var->symbol))
		{
			/* Si la lista está vacía o la letra es previa a la primera de la lista,
			 * el nuevo nodo es ahora el primer elemento */
			if (ll_var_list_is_empty (*var_list) || strcmp (var->symbol, (*var_list)->symbol) < 0)
				{
					/* Añade la lista después del nodo */
					var->next = *var_list;
					/* Ahora la lista empieza con este nodo */
					*var_list = var;
				}
			/* Sino, busca una variable de letra posterior para situarla antes o al
			 * final de la lista. */
			else
				{
					aux = *var_list;
					while (aux->next && strcmp (aux->symbol, var->symbol) < 0)
						aux = aux->next;
					var->next = aux->next;
					aux->next = var;
				}
		}
}


/**
 * Devuelve el número de elementos que contiene una lista de variables.
 */
unsigned int
ll_var_list_length (LlVarList var_list)
{
	unsigned int  length = 0;
	LlVar     *var = var_list;

	if (ll_var_list_is_empty (var_list))
		return 0;
	else
		{
			while (var)
				{
					length++;
					var = var->next;
				}
		}
	return length;
}


/**
 * Procedimiento para registrar las variables de una fórmula. Añade todas las
 * variables presentes en una fórmula en la lista de variables de una lógica.
 *
 * @param logic Lógica a la que van destinadas las variables, además sirve de
 *        contexto para identificar los elementos que son variables.
 * @param formula Fórmula dada como cadena de caracteres.
 */
void
ll_logic_add_formula_vars (LlLogic* logic, char formula[])
{
	LlVar     *var;
	int       i;
	char      symbol[2];

	for (i = 0; i < (int) strlen (formula); i++)
		{
			sprintf (symbol, "%c", formula[i]);
			if (ll_symbol_pn_get_type (formula[i], logic) == LL_SYMBOL_VAR)
				{
					var = ll_var_new (symbol, 0);
			    ll_var_list_add_var (&logic->vars, var);
				}
		}
}
