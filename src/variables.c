/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * variables.c
 * This file is part of MaTest
 *
 * Copyright (C) 2008, 2009 - César González Gutiérrez <ceguel@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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
#include <stdbool.h>

#include "MaTest.h"


bool
is_empty_var_list (LogicVarList list)
{
	return (list == NULL);
}


/**
 * Procedimiento que elimina una lista de variables por completo liberando la
 * memoria reservada.
 */
void
del_var_list (LogicVarList *list)
{
	LogicVar aux;
	
	while (*list)
		{
			aux = *list;
			*list = aux->next;
			free (aux);
		}
}


/**
 * Busca una variable dada por su nombre en una lista de variables.
 *
 * @return Puntero a la variable si existe, o el puntero nulo en caso contrario.
 */
LogicVar
search_var (LogicVarList list, char variable)
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


/**
 * Añade una variable proposicional a una lista si no existe ya una con ese
 * nombre, y la sitúa por orden alfabético, con lo que la lista queda ordenada.
 */
void
add_var (LogicVarList *list, char variable)
{
	LogicVar new, aux;
	
	/* Descarta los elementos que ya están en la lista */
	if (search_var (*list, variable))
		return;
	else
		{
			/* Crea el nuevo nodo */
			new = (LogicVar) malloc (sizeof (LogicVarType));
			new->name = variable;
			new->value = 0;
			
			/* Si la lista está vacía o la letra es previa a la primera de la lista,
			 * el nuevo nodo es ahora el primer elemento */
			if ((*list == NULL) || (variable < (*list)->name))
				{
					/* Añade la lista después del nodo */
					new->next = *list;
					/* Ahora la lista empieza con este nodo */
					*list = new;
				}
			/* Sino, busca una variable de letra posterior para situarla antes o al
			 * final de la lista. */
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


/**
 * Procedimiento para registrar las variables de una fórmula. Añade todas las
 * variables presentes en una fórmula en la lista de variables de una lógica.
 *
 * @param logic Lógica a la que van destinadas las variables, además sirve de
 *        contexto para identificar los elementos que son variables.
 * @param formula Fórmula dada como cadena de caracteres.
 */
void
register_vars (Logic logic, char formula[])
{
	int i;
	
	for (i = 0; i < (int) strlen (formula); i++)
		{
			if (symbol_kind_pn (formula[i], logic) == VAR)
				add_var (&logic->Vars, formula[i]);
		}
}


/**
 * Devuelve el número de elementos que contiene una lista de variables.
 */
int
num_vars (LogicVarList list)
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


/**
 * Establece el valor dado en una variable proposicional.
 */
void
set_var_value (LogicVar var, int n)
{
	if (var)
		var->value = n;
	else
    perror ("La variable no está en la lista.\n");
}


/**
 * Devuelve el valor de una variable proposicional.
 *
 * @return \f$n \geq 0\f$ : el valor de la variable.\n
 *         -1: error, la variable no existe.
 */
int
var_value (LogicVar var)
{
	if (var)
		return var->value;
	else
		{
			perror ("La variable no existe.\n");
			return -1;
		}
}
