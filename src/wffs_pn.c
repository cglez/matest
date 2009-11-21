/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * wffs_pn.c
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
 * @file wffs_pn.c
 * 
 * Este archivo contiene el código que se encarga de manejar las fórmulas en
 * notación polaca. Contiene las funciones necesarias para determinar si una
 * fórmula dada es una fórmula bien formada en esta notación, detección de
 * caracteres errones, errores de sintaxis... Contiene a su vez el parser que
 * transforma una fbf en NP a la estructura en árbol propia de una fbf.
 * 
 * Para las fórmulas en notación polaca se emplea la siguiente convención: las
 * letras minúsculas son variables, las letras mayúsculas son conectivas.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "MaTest.h"


/**
 * Categoriza el símbolo dado según una lógica de contexto (según las conectivas
 * definidas) y según la convención para las fórmulas en notación polaca.
 */
LogicSymbKind
symbol_kind_pn (char symbol, Logic logic)
{
	if (islower (symbol))
		return VAR;
	else if (search_UCon (logic->UCons, symbol))
		return UCON;
	else if (search_BCon (logic->BCons, symbol))
		return BCON;
	else
		return NONE;
}


/**
 * Comprueba que la fórmula dada sea una fórmula bien formada en notación
 * polaca, dada una lógica de contexto. Los símbolos admitidos son sólo letras
 * mayúsculas o minúsculas. Aquí se sigue una convención, en una fbf en notación
 * polaca toda letra mayúscula es una conectiva y toda letra minúscula es una
 * variable; se comprueba que las conectivas estén en efecto definidas en la
 * lógica de contexto dada.
 *
 * @return true: si es una fbf en notación polaca, false en caso contrario.
 */
bool
is_wff_pn (char formula[], Logic logic)
{
	int i, deep = 1;
	
	/* Comprobamos que sólo haya caracteres válidos */
	for (i = 0; i < (int) strlen (formula); i++)
		{
			if (!isalpha (formula[i]))
				{
					perror (" La fórmula dada contiene caracteres no válidos.\n");
					return false;
				}
		}
	 	
	for (i = 0; i < (int) strlen (formula); i++)
		{ 
			/* Las variables disminuyen la profundidad de la fórmula */
			if (symbol_kind_pn (formula[i], logic) == VAR)
				deep--;
			/* Las conectivas unarias dejan la profundidad inalterada */
			else if (symbol_kind_pn (formula[i], logic) == UCON)
				deep = deep;
			/* Las conectivas binarias aumentan la profundidad de la fórmula */
			else if (symbol_kind_pn (formula[i], logic) == BCON)
				deep++;
			/* Sino la conectiva no está definida */
			else if (symbol_kind_pn (formula[i], logic) == NONE)
				{
					printf (" La conectiva %c no está definida.\n", formula[i]);
					return false;
				}
		
		/* Si en este punto las conectivas tienen completos sus argumentos pero aún
		 * resta fórmula, está mal formada por dos posibles motivos */
		if (deep == 0 && formula[i + 1] != 0)
			{
				/* Demasiadas variables */
				if (symbol_kind_pn (formula[i + 1], logic) == VAR)
					perror (" Profundidad excedida; demasiadas variables. Revise la fórmula.\n");
				/* o empieza una nueva fórmula */
				else if (symbol_kind_pn (formula[i + 1], logic) == UCON ||
								 symbol_kind_pn (formula[i + 1], logic) == BCON)
					perror (" Hay varias fórmulas juntas.\n");
				return false;
			}
		}
	
	/* Si después de recorrer la fórmula aún restan argumentos, faltan variables */
	if (deep != 0)
		{
			perror (" Profundidad insuficiente; muy pocas variables. Revise la fórmula.\n");
			return false;
		}
	else
		return true;
}


/**
 * Transforma una fbf en notación polaca en un árbol de fórmula bien formada.
 * Aquí se lee la fórmula de izquierda a derecha, estableciendo cada elemento
 * uno a uno, de la conectiva principal a sus argumentos. Esto da como resultado
 * un árbol de fórmula bien formada correcto, como es propio de la notación
 * polaca.
 */
void parse_polish (LogicWFF *tree, char formula[], Logic logic)
{
	LogicVar var;
	int i;
	
	for (i = 0; i < (int) strlen (formula); i++)
		{
			if (symbol_kind_pn (formula[i], logic) == VAR)
				{
					var = (LogicVar) search_var (logic->Vars, formula[i]);
					set_atom (tree, VAR, formula[i], &var->value);
				}
			else if (symbol_kind_pn (formula[i], logic) == UCON)
				set_atom (tree, UCON, formula[i], NULL);
			else if (symbol_kind_pn (formula[i], logic) == BCON)
				set_atom (tree, BCON, formula[i], NULL);
			else
				{
					perror ("Parsing... Unexpected error!\n");
					return;
				}
		}
}
