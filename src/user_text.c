/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * user_text.c
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
 * @file user.c
 * 
 * Este fichero contiene las funciones que se encargan de interactuar con el
 * usuario. Aquí se encuentran funciones para la entrada de datos (selección de
 * opciones principalmente) y para la salida de información: menús, mensajes,
 * etc.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <libintl.h>
#define _(String) gettext (String)
#include "MaTest.h"


/**
 * Procedimiento para definir de manera interactiva una conectiva unaria nueva
 * dada por su nombre en una lista de conectivas unarias. Se pregunta al usuario
 * por todos los valores uno por uno, indicando el subíndice del elemento
 * actual.
 * Si el usuario presiona Enter sin introducir ningún valor, se asigna
 * autómaticamente a la posición actual el último valor introducido.
 */
void
add_custom_UCon (LogicUConList *list, char name, int dimension)
{
	int i;
	int *mtx;
	char value[10];

	mtx = (int*) calloc (dimension, sizeof (int));

	/* Iniciamos todos los valores a -1 para el control de errores */
	for (i = 0; i < dimension; i++)
		mtx[i] = -1;

	/* Preguntamos por los nuevos valores, uno a uno */
	for (i = 0; i < dimension; i++)
		{
			do
				{
					printf (" %c %i: ", name, i);
					(void) fgets (value, 9, stdin);
					if (value[0] == '\n')
						{
							if (i == 0)
								mtx[i] = -1;
							else
								{
									mtx[i] = mtx[i - 1];
									printf (" %c %i: %i\n", name, i, mtx[i]);
								}
						}
					else
						sscanf (value, "%i", &mtx[i]);
				}
			while (mtx[i] < 0 || mtx[i] >= dimension);
		}

	add_UCon (list, name, mtx, dimension);
	free (mtx);
}


/**
 * Procedimiento para definir de manera interactiva una conectiva binaria nueva
 * dada por su nombre en una lista de conectivas binarias. Se pregunta al
 * usuario por todos los valores uno por uno, indicando los subíndices del
 * elemento actual.
 * Si el usuario presiona Enter sin introducir ningún valor, se asigna
 * autómaticamente a la posición actual el último valor introducido.
 */
void
add_custom_BCon (Logic logic, char connective)
{
	int i, j;
	int **mtx;
	char value[10];

	mtx = (int**) calloc (DIM, sizeof (int*));
	for (i = 0; i < DIM; i++)
		mtx[i] = calloc (DIM, sizeof (int));

	/* Iniciamos todos los valores a -1 para el control de errores */
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				mtx[i][j] = -1;
		}

	/* Preguntamos por los nuevos valores, uno a uno */
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				{
					do
						{
							printf (" %c %i %i: ", connective, i, j);
							(void) fgets (value, 8, stdin);
							if (value[0] == '\n')
								{
									if (j == 0 && i == 0)
										mtx[i][j] = -1;
									else if (j == 0)
										{
											mtx[i][j] = mtx[i - 1][logic->dim - 1];
											printf (" %c %i %i: %i\n", connective, i, j, mtx[i][j]);
										}
									else
										{
											mtx[i][j] = mtx[i][j - 1];
											printf (" %c %i %i: %i\n", connective, i, j, mtx[i][j]);
										}
								}
							else
								sscanf (value, "%i", &mtx[i][j]);
						}
					while (mtx[i][j] < 0 || mtx[i][j] >= DIM);
				}
		}

	add_BCon (&logic->BCons, connective, mtx, logic->dim);
	free (mtx);
}


/**
 * Imprime la matriz de una conectiva unaria en forma de tabla, marcando los
 * valores designados con asterisco.
 */
void
print_uny_matrix (LogicUCon connective, Logic logic)
{
	int i;

	printf ("\n");

	printf ("  %c |", connective->name);
	for (i = 0; i < DIM; i++)
		{
			if (i >= MDV)
				printf (" *%i", i);
			else
				printf ("  %i", i);
		}

	printf ("\n----+");
	for (i = 0; i < DIM; i++)
		printf ("---");

	printf ("-\n" "    |");
	for (i = 0; i < DIM; i++)
		{
			if (connective->matrix[i] >= MDV)
				printf (" *%i", connective->matrix[i]);
			else
				printf ("  %i", connective->matrix[i]);
		}
	printf ("\n");
}


/**
 * Imprime la matriz de una conectiva binaria en forma de tabla, marcando los
 * valores designados con asterisco.
 */
void
print_bin_matrix (LogicBCon connective, Logic logic)
{
	int i, j;

	printf ("\n");

	printf ("  %c |", connective->name);
	for (i = 0; i < DIM; i++)
		{
			if (i >= MDV)
				printf (" *%i", i);
			else
				printf ("  %i", i);
		}

	printf ("\n----+");
	for (i = 0; i < DIM; i++)
		printf ("---");

	printf ("-\n");
	for (i = 0; i < DIM; i++)
		{
			if (i >= MDV)
				printf (" *%i |", i);
			else
				printf ("  %i |", i);

			for (j = 0; j < DIM; j++)
				{
					if (connective->matrix[i][j] >= MDV)
						printf (" *%i", connective->matrix[i][j]);
					else
						printf ("  %i", connective->matrix[i][j]);
				}
			printf ("\n");
		}
}


/**
 * Imprime todas las matrices de todas las conectivas, primero las unarias,
 * luego las binarias, en el orden en que se hayen definidas.
 */
void
show_matrices (Logic logic)
{
	LogicUCon unyaux;
	LogicBCon binaux;

	unyaux = logic->UCons;
	while (unyaux)
		{
			print_uny_matrix (unyaux, logic);
			unyaux = unyaux->next;
		}

	binaux = logic->BCons;
	while (binaux)
		{
			print_bin_matrix (binaux, logic);
			binaux = binaux->next;
		}
}


/**
 * Función que imprime la información del trabajo actual, con los siguientes
 * elementos:
 *   - tamaño de las matrices
 *   - mínimo valor designado
 *   - conectivas unarias definidas, separadas por espacios
 *   - conectivas binarias definidas, idem
 *   - fórmula que se evaluará
 *   - tipo de evaluación
 * 
 * @param work La función recibe como argumento un registro de tipo Work.
 */
void menu_info (Work work)
{
	LogicUCon unyaux;
	LogicBCon binaux;
	
	printf (_("  Dimensión de las matrices:  "));
	if (work->DIM)
		printf ("%ix%i", work->DIM, work->DIM);
	else
		printf (_("Sin definir"));
	
	printf (_("\n  Mínimo Valor Designado:     "));
	if (work->MDV)
		printf ("%i", work->MDV);
	else
		printf (_("Sin definir"));
	
	printf (_("\n  Conectivas unarias:         "));
	unyaux = work->logic->UCons;
	while (unyaux)
		{
			printf ("%c ", unyaux->name);
			unyaux = unyaux->next;
		}
	
	printf (_("\n  Conectivas binarias:        "));
	binaux = work->logic->BCons;
	while (binaux)
		{
			printf ("%c ", binaux->name);
			binaux = binaux->next;
		}
	
	if (work->formula_pn[0])
		printf (_("\n  Fórmula:                    %s"), work->formula_pn);
	else
		printf (_("\n  Fórmula:                    Sin definir"));
	
	if (work->eval_values == ALL)
		printf (_("\n  Evaluar valores:            [Todos]  Designados  No designados"));
	if (work->eval_values == DESIGNATED)
		printf (_("\n  Evaluar valores:            Todos  [Designados]  No designados"));
	if (work->eval_values == NOTDESIGNATED)
		printf (_("\n  Evaluar valores:            Todos  Designados  [No designados]"));
	
	printf ("\n\n");
}


/*
 * Imprime el uso del programa
 */
void menu_usage (void)
{
	printf (_("Uso:\n"
						"        matest <opciones>\n"
						"\n"
						"Opciones:\n"
						"        -h                                   muestra esta ayuda\n"
						"        -v                                   muestra la version\n"
						"        -d, --dimmension  <entero>           dimensión de las matrices\n"
						"        -m, --mdv         <entero>           mínimo valor designado\n"
						"        -f, --formula     <formula>          fórmula que se evaluará\n"
						"        -s, --show                           valores que se mostrarán:\n"
						"                          a, all             - todos\n"
						"                          d, designated      - designados\n"
						"                          n, not-designated  - no designados\n"));
}

/*
 * Procedimiento que imprime el menú de opciones.
 */
void menu_options (void)
{
	printf (_(" Opciones:\n"
						"  V: Valores evaluados\n"
						"  M: Redefinir el Mínimo Valor Designado\n"
						"\n"
						"  N: Nueva conectiva           P: Imprimir las matrices por pantalla\n"
						"  D: Borrar conectiva          W: Escribir las matrices en un archivo\n"
						"\n"
						"  F: Introducir Fórmula\n"
						"  E: Evaluar\n"
						"                                        A: Acerca de    H: Ayuda    Q: Salir\n"
						"\n"
						" Select your option: "));
}


/*
 * Procedimiento para requerir la dimensión inicial de las matrices, para
 * definir las conectivas. Aparece al inicio del programa.
 */
void menu_dimension (void)
{
	printf (_(" Defina primero la dimensión de las matrices para empezar a trabajar.\n"
						" Después, el programa definirá automáticamente las conectivas más comunes\n"
						" según el modelo de las lógicas multivaluadas de Łukasiewicz.\n\n"));
}


/*
 * Imprime el texto de la página "Acerca de", ésta contiene una descripción del
 * programa, versión, créditos y nota legal.
 */
void menu_about (void)
{
	clear_scr();
	printf (_("\n"
	          " MaTest versión %s\n"
	          "\n"
	          " AUTOR: César González Gutiérrez <ceguel@gmail.com>\n"
	          "\n"
	          " DESCRIPCIÓN: MaTest (Matrix Tester) es un tester de matrices lógicas, libre\n"
	          "  y multiplataforma. Definidas unas conectivas como matrices, un mínimo valor\n"
	          "  designado y dada una fórmula, calcula todos los posibles valores de dicha\n"
	          "  fórmula y evalúa en cada caso si se trata de un valor designado o no.\n"
	          "  Sucesor de Matrigüity, creado por J.M. Méndez y B. García Noriega en 1982.\n"
	          "\n"), VERSION);
	printf (_(" COPYRIGHT (C) 2008, 2009 - César González Gutiérrez <ceguel@gmail.com>.\n"
	          "  MaTest is free software: you can redistribute it and/or modify\n"
	          "  it under the terms of the GNU General Public License as published by\n"
	          "  the Free Software Foundation, either version 3 of the License, or\n"
	          "  (at your option) any later version.\n"
	          "\n"));
	printf (_("  MaTest is distributed in the hope that it will be useful, but\n"
	          "  without any warranty; without even the implied warranty of\n"
	          "  merchantability or fitness for a particular purpose.\n"
	          "  See the GNU General Public License for more details:\n"
	          "  <http://gnu.org/licenses/gpl.html>.\n"));
}


/*
 * Ayuda en línea. No implementado por ahora.
 */
void menu_help (void)
{
	menu_header();
	printf (_("\n"
	          "There isn't inline help available by now.\n"
	          "See the Readme.txt file that come with this program.\n"));
}


/*
 * Imprime el menú global: cabecera, información del trabajo y opciones.
 */
void menu_index (Work work)
{
	menu_header();
	menu_info (work);
	menu_options();
}


/**
 * Modo interactivo en modo texto.
 */
int
mode_text (Work work)
{
	char opt, answer[BUFSIZ], namefile[BUFSIZ];
	FILE *outfile;
	
	menu_init();
	
	if (work->logic->dim == 0)
		{
			/* Preguntamos por la dimensión de las matrices */
			menu_dimension();
			do
				{
					printf (_(" Dimensión de las matrices ( <1 ): "));
					fgets (answer, BUFSIZ, stdin);
					sscanf (answer, "%i", &work->DIM);
				}
			while (work->DIM < 2);
		}
	
	/* Define el Mínimo Valor Designado como en el modelo de Łukasiewicz */
	if (work->logic->mdv == 0)
		work->MDV = work->DIM - 1;
	
	/* Define las conectivas por defecto */
	set_default_UCons (work->logic);
	set_default_BCons (work->logic);
	
	/* Muestra todos los valores evaluados por defecto */
	work->eval_values = ALL;
	
	/* Manejador de opciones interactivo */
	for (;;)
		{
			menu_index (work);
			opt = readin (answer, "vmpwndfeahq");
			
			switch (opt)
				{
					/* Cambia los valores mostrados en la evaluación */
					case 'v':
						clear_scr ();
						menu_header ();
						menu_info (work);
						printf (_(" ¿Qué valores quiere que sean evaluados?\n"
						          " a: Todos, d: Designados, n: No designados: "));
						opt = readin (answer, "adn\n");
						switch (opt)
							{
								case 'a':
									work->eval_values = ALL;
									break;
								case 'd':
									work->eval_values = DESIGNATED;
									break;
								case 'n':
									work->eval_values = NOTDESIGNATED;
									break;
								default:
									break;
							}
						break;
					
					
					/* Cambiar el Mínimo Valor Designado */
					case 'm':
						clear_scr ();
						menu_header ();
						menu_info (work);
						do
							{
								printf (_(" Nuevo Mínimo Valor Designado [1 - %i]: "), work->DIM - 1);
								fgets (answer, BUFSIZ, stdin);
								sscanf (answer, "%i", &work->MDV);
							}
						while ((work->MDV < 1) || ((work->MDV) > (work->DIM - 1)));
						break;
					
					
					/* Imprimir las matrices por pantalla */
					case 'p':
						clear_scr ();
						show_matrices (work->logic);
						make_pause ();
						break;
					
					
					/* Escribir las matrices en un archivo externo */
					case 'w':
						clear_scr ();
						menu_header ();
						menu_info (work);
						printf (_(" Escriba el nombre del archivo donde escribir: "));
						fgets (answer, BUFSIZ, stdin);
						sscanf (answer, "%s", namefile);
						
						/* Comprueba que el archivo existe */
						outfile = fopen (namefile, "r");
						if (outfile)
							{
								printf (_(" El archivo \'%s\' ya existe, ¿desea sobreescribirlo? (s/N): "), namefile);
								opt = readin (answer, "sy\n");
								if (opt == 'y' || opt == 's')
									{
										outfile = freopen (namefile, "w", outfile);
										if (outfile)
											{
												write_matrices (outfile, work->logic);
												fclose (outfile);
											}
										else
											{
												printf (_("\n No tiene permisos para escribir en este archivo.\n"));
												make_pause ();
											}
									}
								else
									fclose (outfile);
							}
						else
							{
								outfile = fopen (namefile, "w");
								if (outfile)
									{
										write_matrices (outfile, work->logic);
										fclose (outfile);
									}
								else
									{
										printf (_("\n Error: no se pudo escribir en el archivo.\n"));
										make_pause ();
									}
							}
						break;
					
					
					/* Definir una conectiva nueva personalizada
					 * FIXME: pedir primero el nombre de la matriz? */
					case 'n':
						clear_scr ();
						menu_header ();
						menu_info (work);
						printf (_(" ¿Conectiva unaria o binaria? (u/b): "));
						
						opt = readin (answer, "ub\n");
						switch (opt)
							{
								case 'u':
									printf (_(" Nombre de la conectiva unaria: "));
									opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
									if (search_UCon (work->logic->UCons, opt) ||
											search_BCon (work->logic->BCons, opt) )
										{
											printf (_(" Esta conectiva ya existe, ¿desea redefinirla? (s/N): "));
											opt = readin (answer, "ys\n");
											if (opt == 's' || opt == 'y')
												add_custom_UCon (&work->logic->UCons, opt, work->DIM);
										}
									else
										add_custom_UCon (&work->logic->UCons, opt, work->DIM);
									break;
								
								case 'b':
									printf (_(" Nombre de la conectiva binaria: "));
									opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
									if (search_UCon (work->logic->UCons, opt) ||
											search_BCon (work->logic->BCons, opt))
										{
											printf (_(" Esta conectiva ya existe, ¿desea redefinirla? (s/N): "));
											opt = readin (answer, "ys\n");
											if (opt == 's' || opt == 'y')
												add_custom_BCon (work->logic, opt);
										}
									else
										add_custom_BCon (work->logic, opt);
									break;
							}
						break;
					
					
					/* Eliminar una conectiva existente */
					case 'd':
						clear_scr ();
						menu_header ();
						menu_info (work);
						
						printf (_(" Borrar conectiva: "));
						opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz\n"));
						del_connective (work->logic, opt);
						break;
					
					
					/* Pedir una fórmula bien formada y pasársela al parser */
					case 'f':
						clear_scr();
						menu_header();
						menu_info (work);
						do
							{
								printf (_("\n Escriba una fórmula en notación polaca: "));
								fgets (answer, BUFSIZ, stdin);
								sscanf (answer, "%s", work->formula_pn);
							}
						while (!is_wff_pn (work->formula_pn, work->logic));
						
						if (work->logic->Vars)
							del_var_list (&work->logic->Vars);
						register_vars (work->logic, work->formula_pn);
						
						if (work->wff)
							del_wff (&work->wff);
						parse_polish (&work->wff, work->formula_pn, work->logic);
						
						break;
					
					
					/* Hacer la evaluación */
					case 'e':
						if (work->formula_pn[0] == 0)
							printf (_("\n No se ha introducido ninguna fórmula bien formada.\n"));
						else
							{
								clear_scr ();
								evaluation (stdout, work);
							}
						make_pause ();
						break;
					
					
					/* Mostrar la página "Acerca de" */
					case 'a':
						menu_about ();
						make_pause ();
						break;
					
					
					/* Mostrar el menú de ayuda
					 * TODO: por hacer... */
					case 'h':
						menu_help ();
						make_pause ();
						break;
					
					/* Salir preguntado por confirmación */
					case 'q':
						printf (_(" ¿Está seguro? (s/N): "));
						fgets (answer, BUFSIZ, stdin);
						if (answer[0] == 's' || answer[0] == 'y')
							return 0;
						else
							break;
				}
		}
}
