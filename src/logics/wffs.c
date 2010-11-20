/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * wffs.c
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


#include "logics.h"
#include <stdlib.h>


/**
 * Busca recursivamente el primer argumento libre en el árbol de fbf dado.
 * Después establece el nombre del elemento, el tipo y el puntero al valor
 * correspondiente si se trata de una variable.
 *
 * @param wff Árbol de fórmula bien formada.
 * @param symbol_type Tipo de nodo.
 * @param symbol Símbolo del nodo.
 * @param value Valor del nodo.
 * @return true: si tiene éxito, false: en caso contrario.
 */
bool
ll_wff_add_node (LlWFF *wff, LlSymbolType symbol_type, char symbol[], int *value)
{
	LlWFFNode *father = NULL;
	LlWFFNode *node = *wff;

	while (node)
		{
			/* Las conectivas unarias tienen un sólo argumento: trivial */
			if (node->type == LL_WFF_NODE_U_CON)
				{
					father = node;
					node = node->postarg;
				}
			/* Con las conectivas binarias buscamos recursivamente */
			else if (node->type == LL_WFF_NODE_B_CON)
				{
					if (ll_wff_add_node (&node->prearg, symbol_type, symbol, value))
						return true;
					else if (ll_wff_add_node (&node->postarg, symbol_type, symbol, value))
						return true;
					else
						return false;
				}
			/* Las variables carecen de argumentos */
			else if (node->type == LL_WFF_NODE_VAR)
				return false;
		}

	/* Si el árbol está vacío, reservamos memoria y establecemos los valores */
	if (father == NULL)
		{
			(*wff) = (LlWFF) malloc (sizeof (LlWFFNode));
			(*wff)->type = symbol_type;
			strcpy ((*wff)->symbol, symbol);
			(*wff)->value = value;
			(*wff)->prearg = (*wff)->postarg = NULL;
			return true;
		}
	else if (father->type == LL_WFF_NODE_U_CON)
		{
			node = (LlWFFNode*) malloc (sizeof (LlWFFNode));
			father->postarg = node;
			node->type = symbol_type;
			strcpy (node->symbol, symbol);
			node->value = value;
			node->prearg = node->postarg = NULL;
			return true;
		}
	else if (father->type == LL_WFF_NODE_B_CON)
		{
			if (!father->prearg)
				{
					node = (LlWFFNode*) malloc (sizeof (LlWFFNode));
					father->prearg = node;
					node->type = symbol_type;
					strcpy (node->symbol, symbol);
					node->value = value;
					node->prearg = node->postarg = NULL;
					return true;
				}
			else
				{
					node = (LlWFFNode*) malloc (sizeof (LlWFFNode));
					father->postarg = node;
					node->type = symbol_type;
					strcpy (node->symbol, symbol);
					node->value = value;
					node->prearg = node->postarg = NULL;
					return true;
				}
		}
	else
		{
			perror ("Estableciendo el elemento de una fbf... Error inesperado.\n");
			return false;
		}
}


/**
 * Elimina el árbol de fórmula bien formada dado como argumento liberando su
 * memoria.
 *
 * @param wff Un árbol de fórmula bien formada a borrar.
 */
void
ll_wff_free (LlWFF *wff)
{
	if (!(*wff))
		return;
	ll_wff_free (&(*wff)->prearg);
	ll_wff_free (&(*wff)->postarg);
	free (*wff);
	*wff = NULL;
}


/**
 * Calcula recursivamente y devuelve el valor que se asigna a una fórmula
 * recorriendo el árbol de la fbf, basándose en los valores que toman sus
 * variables y la semántica de las contectivas contenidas en una lógica de
 * contexto.
 *
 * @return n >= 0: el valor que toma la fórmula.\n
 *             -1: error, el árbol de la fbf está vacío.\n
 *             -2: error inesperado.
 */
int
ll_wff_get_value (LlWFF wff, LlLogic* logic)
{
	LlUCon* ucon;
	LlBCon* bcon;

	if (!wff)
		{
			perror ("* El árbol de fbf está vacío.\n");
			return -1;
		}
	else if (wff->type == LL_WFF_NODE_VAR)
		return *wff->value;
	else if (wff->type == LL_WFF_NODE_U_CON)
		{
			ucon = (LlUCon*) ll_ucon_list_get_ucon_by_symbol (logic->ucons, wff->symbol);
			return ucon->matrix[ll_wff_get_value (wff->postarg, logic)];
		}
	else if (wff->type == LL_WFF_NODE_B_CON)
		{
			bcon = (LlBCon*) ll_bcon_list_get_bcon_by_symbol (logic->bcons, wff->symbol);
			return bcon->matrix[ll_wff_get_value (wff->prearg, logic)][ll_wff_get_value(wff->postarg, logic)];
		}
	else
		{
			perror ("* Evaluando la fórmula actual... Error inesperado.\n");
			return -2;
		}
}
