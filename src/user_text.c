/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * user_text.c
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
text_ucon_add_custom (LogicsLogic* logic, char symbol)
{
	LogicsUCon  *ucon;
	char        value[10];
	int         *matrix;
	int         i;

	matrix = (int*) calloc (DIM, sizeof (int));

	/* Iniciamos todos los valores a -1 para el control de errores */
	for (i = 0; i < DIM; i++)
		matrix[i] = -1;

	/* Preguntamos por los nuevos valores, uno a uno */
	for (i = 0; i < DIM; i++)
		{
			do
				{
					printf (" %c %i: ", symbol, i);
					(void) fgets (value, 9, stdin);
					/* si se pulsa Enter sin introducir un valor, se toma el último
					   introducido si no es el primero */
					if (value[0] == '\n')
						{
							if (i == 0)
								matrix[i] = -1;
							else
								{
									matrix[i] = matrix[i - 1];
									printf (" %c %i: %i\n", symbol, i, matrix[i]);
								}
						}
					else
						sscanf (value, "%i", &matrix[i]);
				}
			while (matrix[i] < 0 || matrix[i] >= DIM);
		}

	ucon = logics_ucon_new (symbol, matrix, DIM);
	logics_ucon_list_append_ucon (&logic->ucons, ucon);
	free (matrix);
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
text_bcon_add_custom (LogicsLogic* logic, char symbol)
{
	LogicsBCon  *bcon;
	char        value[10];
	int         **matrix;
	int         i, j;

	matrix = (int**) calloc (DIM, sizeof (int*));
	for (i = 0; i < DIM; i++)
		matrix[i] = calloc (DIM, sizeof (int));

	/* Iniciamos todos los valores a -1 para el control de errores */
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				matrix[i][j] = -1;
		}

	/* Preguntamos por los nuevos valores, uno a uno */
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				{
					do
						{
							printf (" %c %i %i: ", symbol, i, j);
							(void) fgets (value, 9, stdin);
							/* si se pulsa Enter sin introducir un valor, se toma el último
					       introducido si no es el primero */
							if (value[0] == '\n')
								{
									if (j == 0 && i == 0)
										matrix[i][j] = -1;
									else if (j == 0)
										{
											matrix[i][j] = matrix[i - 1][MAXV];
											printf (" %c %i %i: %i\n", symbol, i, j, matrix[i][j]);
										}
									else
										{
											matrix[i][j] = matrix[i][j - 1];
											printf (" %c %i %i: %i\n", symbol, i, j, matrix[i][j]);
										}
								}
							else
								sscanf (value, "%i", &matrix[i][j]);
						}
					while (matrix[i][j] < 0 || matrix[i][j] >= DIM);
				}
		}

	bcon = logics_bcon_new (symbol, matrix, DIM);
	logics_bcon_list_append_bcon (&logic->bcons, bcon);
	free (matrix);
}


/**
 * Imprime la matriz de una conectiva unaria en forma de tabla, marcando los
 * valores designados con asterisco.
 */
void
print_ucon_matrix (LogicsUCon* ucon, LogicsLogic* logic)
{
	int i;

	printf ("\n");

	printf ("  %c |", ucon->symbol);
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
			if (ucon->matrix[i] >= MDV)
				printf (" *%i", ucon->matrix[i]);
			else
				printf ("  %i", ucon->matrix[i]);
		}
	printf ("\n");
}


/**
 * Imprime la matriz de una conectiva binaria en forma de tabla, marcando los
 * valores designados con asterisco.
 */
void
print_bcon_matrix (LogicsBCon* bcon, LogicsLogic* logic)
{
	int i, j;

	printf ("\n");

	printf ("  %c |", bcon->symbol);
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
					if (bcon->matrix[i][j] >= MDV)
						printf (" *%i", bcon->matrix[i][j]);
					else
						printf ("  %i", bcon->matrix[i][j]);
				}
			printf ("\n");
		}
}


/**
 * Imprime todas las matrices de todas las conectivas, primero las unarias,
 * luego las binarias, en el orden en que se hayen definidas.
 */
void
print_matrices (LogicsLogic* logic)
{
	LogicsUCon* ucon;
	LogicsBCon* bcon;

	ucon = logic->ucons;
	while (ucon)
		{
			print_ucon_matrix (ucon, logic);
			ucon = ucon->next;
		}

	bcon = logic->bcons;
	while (bcon)
		{
			print_bcon_matrix (bcon, logic);
			bcon = bcon->next;
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
void menu_info (Work* work)
{
	LogicsUCon  *ucon;
	LogicsBCon  *bcon;
	
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
	ucon = work->logic->ucons;
	while (ucon)
		{
			printf ("%c ", ucon->symbol);
			ucon = ucon->next;
		}
	
	printf (_("\n  Conectivas binarias:        "));
	bcon = work->logic->bcons;
	while (bcon)
		{
			printf ("%c ", bcon->symbol);
			bcon = bcon->next;
		}
	
	if (work->formula_pn[0])
		printf (_("\n  Fórmula:                    %s"), work->formula_pn);
	else
		printf (_("\n  Fórmula:                    Sin definir"));
	
	if (work->evaluation_style == ALL)
		printf (_("\n  Evaluar valores:            [Todos]  Designados  No designados"));
	if (work->evaluation_style == DESIGNATED)
		printf (_("\n  Evaluar valores:            Todos  [Designados]  No designados"));
	if (work->evaluation_style == NOT_DESIGNATED)
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
	          "  D: Redefinir Dimensión de las matrices\n"
						"  M: Redefinir el Mínimo Valor Designado\n"
						"  V: Valores evaluados\n"
						"\n"
						"  N: Nueva/Editar conectiva         P: Imprimir las matrices por pantalla\n"
						"  K: Borrar conectiva               W: Escribir las matrices en un archivo\n"
						"\n"
						"  F: Introducir Fórmula\n"
						"  E: Evaluar\n"
						"                                                    A: Acerca de    Q: Salir\n"
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
	screen_clear();
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
void menu_index (Work* work)
{
	menu_header();
	menu_info (work);
	menu_options();
}


/**
 * Modo interactivo en modo texto.
 */
int
mode_text (Work* work)
{
	char opt, symbol, answer[BUFSIZ], namefile[BUFSIZ];
	FILE *outfile;
	
	menu_init();
	
	if (work->DIM == 0)
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
	logics_logic_set_default_ucons_lukasiewicz (work->logic);
	logics_logic_set_default_bcons_lukasiewicz (work->logic);
	//work->logic_modified = false;
	
	/* Muestra todos los valores evaluados por defecto */
	work->evaluation_style = ALL;
	
	/* Manejador de opciones interactivo */
	for (;;)
		{
			menu_index (work);
			opt = readin (answer, "dmvpwnkfeahq");
			
			switch (opt)
				{
					/* Redefinir la dimensión de las matrices */
					case 'd':
						do
							{
								printf (_(" Nueva dimensión de las matrices ( <1 ): "));
								fgets (answer, BUFSIZ, stdin);
								sscanf (answer, "%i", &work->DIM);
							}
						while (work->DIM < 2);
						logics_ucon_list_free (&work->logic->ucons);
						logics_bcon_list_free (&work->logic->bcons, work->DIM);
						logics_logic_set_default_ucons_lukasiewicz (work->logic);
				    logics_logic_set_default_bcons_lukasiewicz (work->logic);
						work->MDV = work->MAXV;
						/*
						if (work->logic_modified)
							{
								printf (_(" Se han modificado las matrices por defecto.\n"
								          " ¿Desea guardar los cambios? (s/N)\n"));
								opt = readin (answer, "ysn\n");
								switch (opt)
									{
										case 'y':
										case 's':
											write_matrices (file, work->logic);
										case 'n':
							*/
				    break;
					
					/* Cambia los valores mostrados en la evaluación */
					case 'v':
						screen_clear ();
						menu_header ();
						menu_info (work);
						printf (_(" ¿Qué valores quiere que sean evaluados?\n"
						          " a: Todos, d: Designados, n: No designados: "));
						opt = readin (answer, "adn\n");
						switch (opt)
							{
								case 'a':
									work->evaluation_style = ALL;
									break;
								case 'd':
									work->evaluation_style = DESIGNATED;
									break;
								case 'n':
									work->evaluation_style = NOT_DESIGNATED;
									break;
								default:
									break;
							}
						break;
					
					
					/* Cambiar el Mínimo Valor Designado */
					case 'm':
						screen_clear ();
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
						screen_clear ();
						print_matrices (work->logic);
						make_pause ();
						break;
					
					
					/* Escribir las matrices en un archivo externo */
					case 'w':
						screen_clear ();
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
									{
										fclose (outfile);
										fprintf (stderr, _("\n Error: no fue posible abrir el archivo.\n"));
									}
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
										fprintf (stderr, _("\n Error: no se pudo escribir en el archivo.\n"));
										make_pause ();
									}
							}
						break;
					
					
					/* Definir una conectiva nueva personalizada o editar una existente */
					case 'n':
						screen_clear ();
						menu_header ();
						menu_info (work);

						printf (_(" Nombre de la conectiva: "));
						symbol = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz"));
						if (logics_ucon_list_get_ucon_by_symbol (work->logic->ucons, symbol))
							{
								printf (_(" Ya hay una conectiva unaria con el mismo símbolo, ¿desea redefinirla? (s/N): "));
								opt = readin (answer, "ys\n");
								if (opt == 's' || opt == 'y')
									{
										text_ucon_add_custom (work->logic, symbol);
										work->logic_modified = true;
									}
							}
						else if (logics_bcon_list_get_bcon_by_symbol (work->logic->bcons, symbol))
							{
								printf (_(" Ya hay una conectiva binaria con el mismo símbolo, ¿desea redefinirla? (s/N): "));
								opt = readin (answer, "ys\n");
								if (opt == 's' || opt == 'y')
									{
										text_bcon_add_custom (work->logic, symbol);
										work->logic_modified = true;
									}
							}
						else
							{
								printf (_(" ¿Conectiva unaria o binaria? (u/b): "));
								opt = readin (answer, "ub\n");
								switch (opt)
									{
										case 'u':
											logics_con_delete_by_symbol (work->logic, symbol);
											text_ucon_add_custom (work->logic, symbol);
											work->logic_modified = true;
											break;
										case 'b':
											logics_con_delete_by_symbol (work->logic, symbol);
											text_bcon_add_custom (work->logic, symbol);
											work->logic_modified = true;
											break;
									}
							}
						break;
					
					
					/* Eliminar una conectiva existente */
					case 'k':
						screen_clear ();
						menu_header ();
						menu_info (work);
						
						printf (_(" Borrar conectiva: "));
						opt = toupper (readin (answer, "abcdefghijklmnopqrstuvwxyz\n"));
						logics_con_delete_by_symbol (work->logic, opt);
						break;
					
					
					/* Pedir una fórmula bien formada y pasársela al parser */
					case 'f':
						screen_clear();
						menu_header();
						menu_info (work);
						do
							{
								printf (_("\n Escriba una fórmula en notación polaca: "));
								fgets (answer, BUFSIZ, stdin);
								sscanf (answer, "%s", work->formula_pn);
							}
						while (!logics_formula_is_wff_pn (work->formula_pn, work->logic));
						
						if (work->logic->vars)
							logics_var_list_free (&work->logic->vars);
						register_vars (work->logic, work->formula_pn);
						
						if (work->wff)
							del_wff (&work->wff);
						logics_wff_parse_formula_pn (&work->wff, work->formula_pn, work->logic);
						
						break;
					
					
					/* Hacer la evaluación */
					case 'e':
						if (work->formula_pn[0] == 0)
							printf (_("\n No se ha introducido ninguna fórmula bien formada.\n"));
						else
							{
								screen_clear ();
								evaluation (stdout, work);
							}
						make_pause ();
						break;
					
					
					/* Mostrar la página "Acerca de" */
					case 'a':
						menu_about ();
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
