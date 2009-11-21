/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * connectives.c
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


/**
 * Busca una conectiva unaria dada por su nombre dentro de una lista de
 * conectivas unarias.
 *
 * @return Devuelve un puntero a la conectiva, si existe, o el puntero nulo en
 *				 caso contrario.
 */
LogicUCon
search_UCon (LogicUConList list, char name)
{
	LogicUCon aux;
	aux = list;

	while (aux)
		{
			if (aux->name == name)
				return aux;
			else
				aux = aux->next;
		}

	return aux;
}


/**
 * Busca una conectiva binaria dada por su nombre dentro de una lista de
 * conectivas binarias.
 *
 * @return Devuelve un puntero a la conectiva, si existe, o el puntero nulo en
 *				 caso contrario.
 */
LogicBCon
search_BCon (LogicBConList list, char name)
{
	LogicBCon aux;
	aux = list;

	while (aux)
		{
			if (aux->name == name)
				return aux;
			else
				aux = aux->next;
		}

	return aux;
}


/**
 * Añade una conectiva unaria dentro de una lista de conectivas unarias,
 * especificando el nombre de la conectiva, su matriz y la dimensión de ésta.
 *
 * @param list Lista de conectivas unarias.
 * @param name Nombre de la conectiva.
 * @param mtx Al ser una conectiva unaria, la matriz es un vector de enteros con
 *				los valores de la conectiva.
 * @param dimension Dimensión de la matriz (del vector).
 *
 * @return 0: éxito.\n
 *				 1: error, la conectiva ya está en la lista.
 */
int
add_UCon (LogicUConList *list, char name, int *mtx, int dimension)
{
	int i;
	LogicUCon new, aux;

	new = (LogicUCon) malloc (sizeof (LogicUConType));

	/* Si la lista está vacía, el nuevo nodo es el primer elemento */
	if (*list == NULL)
		{
			new->next = *list;
			*list = new;
		}
	else
		{
			aux = *list;
			/* Buscamos el último elemento */
			while (aux->next)
				{
					/* Descartamos los elementos que ya se encuentran en la lista */
					if (aux->name == name || aux->next->name == name)
						return 1;
					else
						aux = aux->next;
				}
				new->next = aux->next;
				aux->next = new;
		}

	/* Definimos los elementos de la conectiva */
	new->name = name;
	new->matrix = (int*) calloc (dimension, sizeof (int));
	for (i = 0; i < dimension; i++)
		new->matrix[i] = mtx[i];
	return 0;
}


/**
 * Añade una conectiva binaria dentro de una lista de conectivas binarias,
 * especificando el nombre de la conectiva, su matriz y la dimensión de ésta.
 *
 * @param list Lista de conectivas binarias.
 * @param name Nombre de la conectiva.
 * @param mtx Matriz de enteros con los valores de la conectiva.
 * @param dimension Dimensión de la matriz.
 *
 * @return 0: éxito.\n
 *				 1: error, la conectiva ya está en la lista.
 */
int
add_BCon (LogicBConList *list, char name, int **mtx, int dimension)
{
	int i, j;
	LogicBCon new, aux;

	new = (LogicBCon) malloc (sizeof (LogicBConType));

	/* Si la lista está vacía, el nuevo nodo es el primer elemento */
	if (*list == NULL)
		{
			new->next = *list;
			*list = new;
		}
	else
		{
			aux = *list;
			/* Buscamos el último elemento */
			while (aux->next)
				{
					/* Descartamos los elementos que ya se encuentran en la lista */
					if (aux->name == name || aux->next->name == name)
						return 1;
					else
						aux = aux->next;
				}
				new->next = aux->next;
				aux->next = new;
		}

	/* Definimos los elementos de la conectiva */
	new->name = name;
	new->matrix = (int**) calloc (dimension, sizeof (int*));
	for (i = 0; i < dimension; i++)
		new->matrix[i] = (int*) calloc (dimension, sizeof (int));
	for (i = 0; i < dimension; i++)
		{
			for (j = 0; j < dimension; j++)
				new->matrix[i][j] = mtx[i][j];
		}
	return 0;
}


/**
 * Define las conectivas unarias por defecto, esto es, como en el modelo de las
 * lógicas multivaluadas de Łukasiewicz. Sólo se define una conectiva unaria,
 * la negación (N), de este modo: \f$ \neg x =_{def} ( 1 - x ) \f$.
 */
void
set_default_UCons (Logic logic)
{
	int i;
	int *mtx;

	mtx = (int*) calloc (DIM, sizeof (int));

	/* Negación (N):
	 *  Nx [¬x]	=df	(1 - x)	 */
	for (i = 0; i < DIM; i++)
		mtx[i] = logic->dim - 1 - i;
	add_UCon (&logic->UCons, 'N', mtx, logic->dim);

	free (mtx);
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
set_default_BCons (Logic logic)
{
	int i, j;
	int **mtx;

	mtx = (int**) calloc (DIM, sizeof (int*));
	for (i = 0; i < DIM; i++)
		mtx[i] = calloc (DIM, sizeof (int));

	/* Implicación (C):
	 *  Cxy [x -> y]  =df  min {n, n - x + y}  */
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				mtx[i][j] = MIN ((DIM - 1), (DIM - 1 - i + j));
		}
	add_BCon (&logic->BCons, 'C', mtx, logic->dim);

	/* Conjunción (K):
	 *  Kxy [x & y]  =df  min {x, y}  */
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				mtx[i][j] = MIN (i, j);
		}
	add_BCon (&logic->BCons, 'K', mtx, logic->dim);

	/* Disyunción (A):
	 *  Axy [x v y]  =df  Max {x, y}  */
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				mtx[i][j] = MAX (i, j);
		}
	add_BCon (&logic->BCons, 'A', mtx, logic->dim);

	/* Bicondicional (E):
	 *  Exy [x <-> y]  =df  n - |x - y|  */
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				mtx[i][j] = DIM - 1 - abs (i - j);
		}
	add_BCon (&logic->BCons, 'E', mtx, logic->dim);

	free (mtx);
}


/**
 * Elimina de una lógica una conectiva, unaria o binaria indistintamente, dada
 * por su nombre, liberando su memoria.
 */
void
del_connective (Logic logic, char connective)
{
	int i;
	LogicUCon unyprev, unyaux;
	LogicBCon binprev, binaux;

	if (search_UCon (logic->UCons, connective))
		{
			unyprev = NULL;
			unyaux = logic->UCons;

			while (unyaux && unyaux->name != connective)
				{
					unyprev = unyaux;
					unyaux = unyaux->next;
				}
			if (!unyaux || unyaux->name != connective)
				{
					perror ("¡Error inesperado!\n");
					return;
				}
			else
				{
					if (!unyprev)
						logic->UCons = unyaux->next;
					else
						unyprev->next = unyaux->next;

					free (unyaux->matrix);
					free (unyaux);
				}
		}

	else if (search_BCon (logic->BCons, connective))
		{
			binprev = NULL;
			binaux = logic->BCons;

			while (binaux && binaux->name != connective)
				{
					binprev = binaux;
					binaux = binaux->next;
				}
			if (!binaux || binaux->name != connective)
				{
					perror ("¡Error inesperado!\n");
					return;
				}
			else
				{
					if (!binprev)
						logic->BCons = binaux->next;
					else
						binprev->next = binaux->next;

					for (i = 0; i < DIM; i++)
						free (binaux->matrix[i]);
					free (binaux->matrix);
					free (binaux);
				}
		}
	else
		fprintf (stderr, " La conectiva %c no existe.\n", connective);
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
write_uny_matrix (FILE *file, LogicUCon connective, int dimension)
{
	int i;

	if (file)
		{
			fprintf (file, "%c\n", connective->name);
			for (i = 0; i < dimension; i++)
				fprintf (file, "%i ", connective->matrix[i]);
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
write_bin_matrix (FILE *file, LogicBCon connective, int dimension)
{
	int i, j;

	if (file)
		{
			fprintf (file, "%c\n", connective->name);
			for (i = 0; i < dimension; i++)
				{
					for (j = 0; j < dimension; j++)
						fprintf (file, "%i ", connective->matrix[i][j]);
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
write_matrices (FILE *file, Logic logic)
{
	LogicUCon unyaux;
	LogicBCon binaux;

	if (file)
		{
			unyaux = logic->UCons;
			while (unyaux)
				{
					write_uny_matrix (file, unyaux, DIM);
					unyaux = unyaux->next;
				}

			binaux = logic->BCons;
			while (binaux)
				{
					write_bin_matrix (file, binaux, DIM);
					binaux = binaux->next;
				}

			return 0;
		}
	else
		return 1;
}
