/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * user.c
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
 * Esta función busca el primer caracter que coincida con un patrón dado por
 * una cadena de caracteres. Está pensada para recoger una opción dada por el
 * usuario por teclado. Esta función hace la entrada insensible a mayúsculas,
 * devuelve siempre minúsculas.
 * 
 * @param str Cadena en la que buscar.
 * @param pattern Patrón dado como cadena de caracteres.
 * @return Caracter encontrado; si es una letra, en minúscula.
 */
char readin (char str[], char pattern[])
{
	char ch;
	int i;
	
	fflush (stdout);
	for (;;)
		{
			(void) fgets (str, BUFSIZ, stdin);
			ch = 0;
			for (i = 0; str[i]; i++)
				{
					if (isupper (ch = str[i]))
						ch = tolower (ch);
					if (strchr (pattern, ch))
						return ch;
				}
		}
}


/**
 * Procedimiento para limpiar la pantalla. Pretende ser multiplataforma,
 * utiliza según qué método dependiendo del parámetro que se le pasa al
 * preprocesador. Para sistemas tipo Unix se llama a la función 'clear',
 * para Windows/DOS se llama a la función 'cls', por defecto se imprime una
 * serie de saltos de línea.
 */
void clear_scr (void)
{
#ifdef POSIX
  if (system ("clear"))
		return;
#elif WIN32
	if (system ("cls"))
		return;
#else
	int i;
	for (i = 0; i < 24; i++)
		printf ("\n");
#endif
}


/**
 * Procedimiento para hacer una pausa en la salida por la terminal.
 * Imprime un mensaje y espera hasta que el usuario pulse Intro.
 */
void make_pause (void)
{
	printf (_("\nPulse Intro para continuar... "));
	fflush (stdout);
	while (getchar() != '\n');
}


/**
 * Imprime una cabecera con el nombre del programa, versión y una descripción
 * breve.
 */
void menu_header (void)
{
	clear_scr();
	printf ("\n"
	        "                            --- MaTest %s ---\n"
	        "                     Matrix Tester for logical matrices\n"
	        "\n", VERSION);
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


/**
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


/**
 * Procedimiento que imprime la nota legal que debe aparecer al arranque como
 * aconseja la FSF.
 */
void menu_init (void)
{
	menu_header();
	printf (_("\n"
						" Copyright (C) 2008, 2009 - César González Gutiérrez.\n"
						" License GPLv3+: GNU General Public Licence version 3 or later,\n"
						" see <http://gnu.org/licenses/gpl.html>.\n"
						" This is free software: you are free to change and redistribute it.\n"
						" There is NO WARRANTY, to the extent permitted by law.\n"
						" For more details type 'a' in the main menu.\n\n\n"));
}


void menu_dimension (void)
{
	printf (_(" Defina primero la dimensión de las matrices para empezar a trabajar.\n"
						" Después, el programa definirá automáticamente las conectivas más comunes\n"
						" según el modelo de las lógicas multivaluadas de Łukasiewicz.\n\n"));
}


/**
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


/**
 * Ayuda en línea. No implementado por ahora.
 */
void menu_help (void)
{
	menu_header();
	printf (_("\n"
	          "There isn't inline help available by now.\n"
	          "See the Readme.txt file that come with this program.\n"));
}


/**
 * Imprime el menú global: cabezera, información del trabajo y opciones.
 */
void menu_index (Work work)
{
	menu_header();
	menu_info (work);
	menu_options();
}

