/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * MaTest.c
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
 * @file MaTest.c
 *
 * Este archivo contiene la función principal "main" y el manejador de opciones
 * interactivo.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <libintl.h>
#define _(String) gettext (String)
#include "MaTest.h"


/**
 * Fución principal main. Define los elementos principales y pasa al modo
 * interactivo.
 */
int
main (int argc, char** argv)
{
	char opt, answer[BUFSIZ], namefile[BUFSIZ];
	FILE *outfile;
	Work work;

	work = (Work) malloc (sizeof (workType));
	work->logic = (Logic) malloc (sizeof (LogicType));
	work->formula_pn[0] = 0;
	work->wff = NULL;

	menu_init();

	/* Preguntamos por la dimensión de las matrices */
	menu_dimension();
	do
		{
			printf (_(" Dimensión de las matrices ( <1 ): "));
			fgets (answer, BUFSIZ, stdin);
			sscanf (answer, "%i", &work->DIM);
		}
	while (work->DIM < 2);
	
	/* Define el Mínimo Valor Designado como en el modelo de Łukasiewicz */
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
						
						printf (_(" Delete connective: "));
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
							{
								del_wff (&work->wff);
								work->wff = NULL;
							}
						parse_polish (&work->wff, work->formula_pn, work->logic);
						
						break;
					
					
					/* Hacer la evaluación */
					case 'e':
						if (work->formula_pn[0] == 0)
							printf (_("\n Primero escriba una fórmula bien formada.\n"));
						else
							{
								clear_scr ();
								evaluation (work);
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
