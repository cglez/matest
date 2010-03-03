/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * wffs_std.c
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


typedef struct _LlStack LlStack;
struct _LlStack
	{
		char symbol;
		struct _LlStack *next;
	};


void
ll_stack_push (LlStack *stack, char ch)
{
	LlStack* new;

	/* Crear un nodo nuevo */
	new = (LlStack*) malloc (sizeof (LlStack));
	new->symbol = ch;

	/* Añadimos la pila a continuación del nuevo nodo */
	new->next = *stack;
	/* Ahora, el comienzo de nuestra pila es en nuevo nodo */
	*stack = new;
}


char
ll_stack_pop (LlStack *stack)
{
	LlStack*			node;	 /* variable auxiliar para manipular nodo */
	char					ch;		 /* variable auxiliar para retorno */

	/* Nodo apunta al primer elemento de la pila */
	node = *stack;
	if (!node)
		return 0; /* Si no hay nodos en la pila retornamos 0 */
	/* Asignamos a pila toda la pila menos el primer elemento */
	*stack = node->next;
	/* Guardamos el valor de retorno */
	ch = node->symbol;
	/* Borrar el nodo */
	free (node);
	return ch;
}


LlSymbolType
ll_symbol_type_std (char symbol, LlLogic* logic)
{
	if (islower (symbol))
		return LL_SYMBOL_VAR;
	else if (isupper (symbol))
		{
			if (ll_ucon_list_get_ucon_by_symbol (logic->ucons, symbol))
				return LL_SYMBOL_U_CON;
			else if (ll_bcon_list_get_bcon_by_symbol (logic->bcons, symbol))
				return LL_SYMBOL_B_CON;
			else
				return LL_SYMBOL_NONE;
		}
	else if (symbol == '(' || symbol == '[' || symbol == '{')
		return LL_SYMBOL_AUX_OPEN;
	else if (symbol == ')' || symbol == ']' || symbol == '}')
		return LL_SYMBOL_AUX_CLOSE;
	else
		return LL_SYMBOL_NONE;
}


int
priority (char symbol, LlLogic* logic)
{
	switch (ll_symbol_type_std (symbol, logic))
		{
			case LL_SYMBOL_AUX_OPEN :
				return 0;
			case LL_SYMBOL_U_CON :
				return 1;
			case LL_SYMBOL_B_CON :
				return 2;
			case LL_SYMBOL_AUX_CLOSE :
				return 3;
		}
}


void
parse_std (LogicWFF *wff, char formula[], LlLogic* logic)
{
	int i;
	char ch;
	LlStack stack, aux;

	stack = (LlStack) malloc (sizeof (LlStack));
	aux = (LlStack) malloc (sizeof (LlStack));

	stack = aux = NULL;

	for (i = (int) strlen (formula) - 1; i >= 0; i--)
		{
			switch (ll_symbol_type_std (formula[i], logic))
				{
					case LL_SYMBOL_VAR :
						ll_stack_push (&stack, formula[i]);
						break;
					case LL_SYMBOL_U_CON :
					case LL_SYMBOL_B_CON :
						while (aux && (priority (aux->symbol, logic) <= priority (formula[i], logic)))
							{
								ch = ll_stack_pop (&aux);
								ll_stack_push (&stack, ch);
							}
						ll_stack_push (&aux, formula[i]);
						break;
					case LL_SYMBOL_AUX_CLOSE :
						ll_stack_push (&aux, formula[i]);
						break;
					case LL_SYMBOL_AUX_OPEN :
						while (ll_symbol_type_std (aux->symbol, logic) != LL_SYMBOL_AUX_CLOSE)
							{
								ch = ll_stack_pop (&aux);
								ll_stack_push (&stack, ch);
							}
						ch = ll_stack_pop (&aux);
						break;
				}
		}

	while (aux)
		{
			ch = ll_stack_pop (&aux);
			ll_stack_push (&stack, ch);
		}

	while (stack)
		printf ("%c", ll_stack_pop (&stack));
	printf ("\n");
}
