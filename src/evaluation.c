/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * evaluation.c
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
set_atom (LogicWFF *tree, LogicSymbKind kind, char name, int *value)
{
	LogicAtom father = NULL;
	LogicAtom node = *tree;
	
	while (node)
		{
			/* Las conectivas unarias tienen un sólo argumento: trivial */
			if (node->kind == UCON)
				{
					father = node;
					node = node->postarg;
				}
			/* Con las conectivas binarias buscamos recursivamente */
			else if (node->kind == BCON)
				{
					if (set_atom (&node->prearg, kind, name, value))
						return true;
					else if (set_atom (&node->postarg, kind, name, value))
						return true;
					else
						return false;
				}
			/* Las variables carecen de argumentos */
			else if (node->kind == VAR)
				return false;
		}

	/* Si el árbol está vacío, reservamos memoria y establecemos los valores */
	if (father == NULL)
		{
			(*tree) = (LogicWFF) malloc (sizeof (LogicWFFtype));
			(*tree)->kind = kind;
			(*tree)->name = name;
			(*tree)->value = value;
			(*tree)->prearg = (*tree)->postarg = NULL;
			return true;
		}
	else if (father->kind == UCON)
		{
			node = (LogicAtom) malloc (sizeof (LogicWFFtype));
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
					node = (LogicAtom) malloc (sizeof (LogicWFFtype));
					father->prearg = node;
					node->kind = kind;
					node->name = name;
					node->value = value;
					node->prearg = node->postarg = NULL;
					return true;
				}
			else
				{
					node = (LogicAtom) malloc (sizeof (LogicWFFtype));
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
			perror ("Estableciendo el elemento de una fbf... ¡Error inesperado!\n");
			make_pause ();
			return false;
		}
}


/**
 * Elimina el árbol de fórmula bien formada dado como argumento liberando su
 * memoria.
 */
void
del_wff (LogicWFF *wff)
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
eval_formula (LogicWFF wff, Logic logic)
{
	LogicUCon ucon;
	LogicBCon bcon;
	
	if (!wff)
		{
			perror ("El árbol de la fbf está vacío.\n");
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
			perror ("Evaluando la fórmula actual... ¡Error inesperado!\n");
			return -2;
		}
}


/*
 * Procedimiento para imprimir la fórmula que se está evaluando actualmente
 * en notación polaca cambiando las variables por los valores que correspondan.
 */
char*
print_current_evaluating_formula_pn (char formula[], Logic logic)
{
	int i;
	static char str[MAX_FORMULA_LENGHT];
	
	for (i = 0; i < (int) strlen (formula); i++)
		{
			if (symbol_kind_pn (formula[i], logic) == VAR)
				str[i] = (char) var_value (search_var (logic->Vars, formula[i]));
			else
				str[i] = formula[i];
		}

	return str;
}
