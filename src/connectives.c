/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * connectives.c
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


/**
 * @file connectives.c
 *
 * Este archivo contiene el código responsable de manejar las conectivas, su
 * creación, definición, eliminación...
 * Muchas de las funciones aquí presentes vienen por pares, unas para manejar
 * las conectivas unarias y otras para las conectivas binarias, unas manejan
 * vectores, las otras matrices.
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include <libintl.h>
#define _(String) gettext (String)
#include "MaTest.h"


/*
 * Crea una conectiva unaria nueva.
 */
LogicsUCon*
logics_ucon_new (char symbol, int *matrix, int dimension)
{
	LogicsUCon  *ucon;
	int         i;

	ucon = (LogicsUCon*) malloc (sizeof (LogicsUCon));
	ucon->matrix = (int*) calloc (dimension, sizeof (int));

	ucon->symbol = symbol;
	for (i = 0; i < dimension; i++)
		ucon->matrix[i] = matrix[i];

	return ucon;
}


/*
 * Crea una conectiva binaria nueva.
 */
LogicsBCon*
logics_bcon_new (char symbol, int **matrix, int dimension)
{
	LogicsBCon  *bcon;
	int         i, j;

	bcon = (LogicsBCon*) malloc (sizeof (LogicsBCon));
	bcon->matrix = (int**) calloc (dimension, sizeof (int*));
	for (i = 0; i < dimension; i++)
		bcon->matrix[i] = (int*) calloc (dimension, sizeof (int));

	bcon->symbol = symbol;
	for (i = 0; i < dimension; i++)
		for (j = 0; j < dimension; j++)
			bcon->matrix[i][j] = matrix[i][j];
	
	return bcon;
}


/**
 * Busca una conectiva unaria dada por su nombre dentro de una lista de
 * conectivas unarias.
 *
 * @return Devuelve un puntero a la conectiva, si existe, o el puntero nulo en
 *         caso contrario.
 */
LogicsUCon*
logics_ucon_list_get_ucon_by_symbol (LogicsUConList ucon_list, char symbol)
{
	LogicsUCon  *ucon;
	
	ucon = ucon_list;
	while (ucon)
		{
			if (ucon->symbol == symbol)
				return ucon;
			else
				ucon = ucon->next;
		}

	return ucon;
}


/**
 * Busca una conectiva binaria dada por su nombre dentro de una lista de
 * conectivas binarias.
 *
 * @return Devuelve un puntero a la conectiva, si existe, o el puntero nulo en
 *         caso contrario.
 */
LogicsBCon*
logics_bcon_list_get_bcon_by_symbol (LogicsBConList bcon_list, char symbol)
{
	LogicsBCon* bcon;
	
	bcon = bcon_list;
	while (bcon)
		{
			if (bcon->symbol == symbol)
				return bcon;
			else
				bcon = bcon->next;
		}

	return bcon;
}


/*
 * Borra una lista de conectivas unarias.
 */
void
logics_ucon_list_free (LogicsUConList *ucon_list)
{
	LogicsUCon  *ucon, *ucon_next;
	
	ucon = *ucon_list;
	while (ucon)
		{
			ucon_next = ucon->next;
			free (ucon->matrix);
			free (ucon);
			ucon = ucon_next;
		}
	*ucon_list = NULL;
}


/*
 * Borra una lista de conectivas binarias.
 */
void
logics_bcon_list_free (LogicsBConList *bcon_list, int dimension)
{
	LogicsBCon  *bcon, *bcon_next;
	int         i;

	bcon = *bcon_list;
	while (bcon)
		{
			bcon_next = bcon->next;
			for (i = 0; i < dimension; i++)
				free (bcon->matrix[i]);
			free (bcon->matrix);
			free (bcon);
			bcon = bcon_next;
		}
	*bcon_list = NULL;
}


/*
 * Añade una conectiva unaria al final de una lista de conectivas unarias
 * siempre y cuando no exista ya una con el mismo símbolo.
 */
int
logics_ucon_list_append_ucon (LogicsUConList *ucon_list, LogicsUCon* ucon)
{
	LogicsUCon  *aux;
	
	/* Si la lista está vacía, el nuevo nodo es el primer elemento */
	if (*ucon_list == NULL)
		{
			ucon->next = *ucon_list;
			*ucon_list = ucon;
		}
	else
		{
			aux = *ucon_list;
			/* Buscamos el último elemento */
			while (aux->next)
				{
					/* Descartamos los elementos que ya se encuentran en la lista */
					if (aux->symbol == ucon->symbol || aux->next->symbol == ucon->symbol)
						{
							fprintf (stderr, "* Ya hay una conectiva definida como %c.\n", ucon->symbol);
							return 1;
						}
					else
						aux = aux->next;
				}
				ucon->next = aux->next;
				aux->next = ucon;
		}
	
	return 0;
}


/*
 * Añade una conectiva binaria al final de una lista de conectivas binarias
 * siempre y cuando no exista ya una con el mismo símbolo.
 */
int
logics_bcon_list_append_bcon (LogicsBConList *bcon_list, LogicsBCon* bcon)
{
	LogicsBCon  *aux;
	
	/* Si la lista está vacía, el nuevo nodo es el primer elemento */
	if (*bcon_list == NULL)
		{
			bcon->next = *bcon_list;
			*bcon_list = bcon;
		}
	else
		{
			aux = *bcon_list;
			/* Buscamos el último elemento */
			while (aux->next)
				{
					/* Descartamos los elementos que ya se encuentran en la lista */
					if (aux->symbol == bcon->symbol || aux->next->symbol == bcon->symbol)
						{
							fprintf (stderr, "* Ya hay una conectiva definida como %c.\n", bcon->symbol);
							return 1;
						}
					else
						aux = aux->next;
				}
				bcon->next = aux->next;
				aux->next = bcon;
		}
	
	return 0;
}


/**
 * Define las conectivas unarias por defecto, esto es, como en el modelo de las
 * lógicas multivaluadas de Łukasiewicz. Sólo se define una conectiva unaria,
 * la negación (N), de este modo: \f$ \neg x =_{def} ( 1 - x ) \f$.
 */
void
logics_logic_set_default_ucons_lukasiewicz (LogicsLogic* logic)
{
	LogicsUCon  *ucon;
	int         *matrix;
	int         i;

	matrix = (int*) calloc (DIM, sizeof (int));

	/* Negación (N):
	 *  Nx [¬x]	=df	(1 - x)	 */
	for (i = 0; i < DIM; i++)
		matrix[i] = MAXV - i;
	ucon = logics_ucon_new ('N', matrix, DIM);
	logics_ucon_list_append_ucon (&logic->ucons, ucon);

	free (matrix);
}


/**
 * Define las conectivas binarias por defecto, esto es, como en el modelo de las
 * lógicas multivaluadas de Łukasiewicz. Las conectivas binarias que se definen
 * son las siguientes:
 *	 - Implicación (C): \f$ x \rightarrow y =_{def} \min \{n, n - x + y\} \f$.
 *	 - Conjunción	(K): \f$ x \wedge y =_{def} \min \{x, y\} \f$.
 *	 - Disyunción	(A): \f$ x \vee y =_{def} \max \{x, y\} \f$.
 *	 - Bicondicional (E): \f$ x \leftrightarrow y =_{def} n - | x - y | \f$.
 */
void
logics_logic_set_default_bcons_lukasiewicz (LogicsLogic* logic)
{
	LogicsBCon  *bcon;
	int         **mtx;
	int         i, j;

	mtx = (int**) calloc (DIM, sizeof (int*));
	for (i = 0; i < DIM; i++)
		mtx[i] = calloc (DIM, sizeof (int));

	/* Implicación (C):
	 *  Cxy [x -> y]  =df  min {n, n - x + y}  */
	for (i = 0; i < DIM; i++)
		for (j = 0; j < DIM; j++)
			mtx[i][j] = MIN (MAXV, (MAXV - i + j));
	bcon = logics_bcon_new ('C', mtx, DIM);
	logics_bcon_list_append_bcon (&logic->bcons, bcon);

	/* Conjunción (K):
	 *  Kxy [x & y]  =df  min {x, y}  */
	for (i = 0; i < DIM; i++)
		for (j = 0; j < DIM; j++)
			mtx[i][j] = MIN (i, j);
	bcon = logics_bcon_new ('K', mtx, DIM);
	logics_bcon_list_append_bcon (&logic->bcons, bcon);

	/* Disyunción (A):
	 *  Axy [x v y]  =df  Max {x, y}  */
	for (i = 0; i < DIM; i++)
		for (j = 0; j < DIM; j++)
			mtx[i][j] = MAX (i, j);
	bcon = logics_bcon_new ('A', mtx, DIM);
	logics_bcon_list_append_bcon (&logic->bcons, bcon);

	/* Bicondicional (E):
	 *  Exy [x <-> y]  =df  n - |x - y|  */
	for (i = 0; i < DIM; i++)
		for (j = 0; j < DIM; j++)
			mtx[i][j] = MAXV - abs (i - j);
	bcon = logics_bcon_new ('E', mtx, DIM);
	logics_bcon_list_append_bcon (&logic->bcons, bcon);

	free (mtx);
}


/**
 * Elimina de una lógica una conectiva, unaria o binaria indistintamente, dada
 * por su nombre, liberando su memoria.
 */
void
logics_con_delete_by_symbol (LogicsLogic* logic, char symbol)
{
	int i;
	LogicsUCon *unyprev, *unyaux;
	LogicsBCon *binprev, *binaux;

	if (logics_ucon_list_get_ucon_by_symbol (logic->ucons, symbol))
		{
			unyprev = NULL;
			unyaux = logic->ucons;

			while (unyaux && unyaux->symbol != symbol)
				{
					unyprev = unyaux;
					unyaux = unyaux->next;
				}
			if (!unyaux || unyaux->symbol != symbol)
				{
					perror ("* ¡Error inesperado!\n");
					return;
				}
			else
				{
					if (!unyprev)
						logic->ucons = unyaux->next;
					else
						unyprev->next = unyaux->next;

					free (unyaux->matrix);
					free (unyaux);
				}
		}

	else if (logics_bcon_list_get_bcon_by_symbol (logic->bcons, symbol))
		{
			binprev = NULL;
			binaux = logic->bcons;

			while (binaux && binaux->symbol != symbol)
				{
					binprev = binaux;
					binaux = binaux->next;
				}
			if (!binaux || binaux->symbol != symbol)
				{
					perror ("* Error inesperado\n");
					return;
				}
			else
				{
					if (!binprev)
						logic->bcons = binaux->next;
					else
						binprev->next = binaux->next;

					for (i = 0; i < DIM; i++)
						free (binaux->matrix[i]);
					free (binaux->matrix);
					free (binaux);
				}
		}
	else
		fprintf (stderr, "* La conectiva %c no existe.\n", symbol);
}


/**
 * Escribe en un archivo externo la matriz de una conectiva unaria en forma de
 * tabla simple. El formato pretende ser legible tanto por un humano como por
 * una máquina, editable como texto plano e importable por el programa.
 *
 * @return 0: éxito.\n
 *				 1: error, archivo inválido.
 */
int
write_ucon_matrix (FILE *file, LogicsUCon* ucon, int dimension)
{
	int i;

	if (file)
		{
			fprintf (file, "%c\n", ucon->symbol);
			for (i = 0; i < dimension; i++)
				fprintf (file, "%i ", ucon->matrix[i]);
			fprintf (file, "\n\n");

			return 0;
		}
	else
		return 1;
}


/**
 * Escribe en un archivo externo la matriz de una conectiva binaria en forma de
 * tabla simple. El formato pretende ser legible tanto por un humano como por
 * una máquina, editable como texto plano e importable por el programa.
 *
 * @return 0: éxito.\n
 *				 1: error, archivo inválido.
 */
int
write_bcon_matrix (FILE *file, LogicsBCon* bcon, int dimension)
{
	int i, j;

	if (file)
		{
			fprintf (file, "%c\n", bcon->symbol);
			for (i = 0; i < dimension; i++)
				{
					for (j = 0; j < dimension; j++)
						fprintf (file, "%i ", bcon->matrix[i][j]);
					fprintf (file, "\n");
				}
			fprintf (file, "\n");

			return 0;
		}
	else
		return 1;
}


/**
 * Escribe todas las matrices de todas las conectivas en un archivo externo en
 * forma de tabla simple. Primero se escriben las conectivas unarias y luego las
 * binarias, en el orden en que hayan sido definidas. El formato pretende ser
 * legible tanto por un humano como por una máquina, editable como texto plano
 * e importable por el programa.
 *
 * @return 0: éxito.\n
 *				 1: error, archivo inválido.
 */
int
write_matrices (FILE *file, LogicsLogic* logic)
{
	LogicsUCon  *ucon;
	LogicsBCon  *bcon;

	if (file)
		{
			ucon = logic->ucons;
			while (ucon)
				{
					write_ucon_matrix (file, ucon, DIM);
					ucon = ucon->next;
				}

			bcon = logic->bcons;
			while (bcon)
				{
					write_bcon_matrix (file, bcon, DIM);
					bcon = bcon->next;
				}

			return 0;
		}
	else
		return 1;
}
