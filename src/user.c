/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * user.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <libintl.h>
#define _(String) gettext (String)
#include "MaTest.h"


/*
 * Busca el primer caracter que coincida con un patrón dado por una cadena de
 * caracteres. Pensado para recoger una opción dada por el usuario por teclado.
 * Esta función hace la entrada insensible a mayúsculas, devuelve siempre
 * minúsculas.
 *
 * @param str Cadena en la que buscar.
 * @param pattern Patrón dado como cadena de caracteres.
 * @return Caracter encontrado; si es una letra, en minúscula.
 */
char
readin (char str[], char pattern[])
{
	char ch;
	int i;

	fflush (stdout);
	for (;;)
		{
			fgets (str, BUFSIZ, stdin);
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


/*
 * Procedimiento para limpiar la pantalla. Pretende ser multiplataforma,
 * utiliza según qué método dependiendo del parámetro que se le pase al
 * preprocesador. Para sistemas tipo Unix se llama a la función 'clear',
 * para Windows/DOS se llama a la función 'cls', por defecto se imprime una
 * serie de 24 saltos de línea.
 */
void
screen_clear (void)
{
#ifdef UNIX
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


/*
 * Procedimiento para hacer una pausa en la salida por la terminal.
 * Imprime un mensaje y espera hasta que el usuario pulse Intro.
 */
void
make_pause (void)
{
	printf (_("\nPulse Intro para continuar... "));
	fflush (stdout);
	while (getchar() != '\n');
}


/*
 * Imprime la versión del programa. Esta función es llamada desde la línea de
 * comandos, según lo aconsejado por la FSF.
 */
void
menu_version (void)
{
	printf (_("MaTest %s\n"
						"Copyright (C) 2008, 2009 - César González Gutiérrez.\n"
						"License GPLv3+: GNU General Public Licence version 3 or later,\n"
						"see <http://gnu.org/licenses/gpl.html>.\n"
						"This is free software: you are free to change and redistribute it.\n"
						"There is NO WARRANTY, to the extent permitted by law.\n"), VERSION);
}


/*
 * Imprime el uso del programa
 */
void
menu_usage (void)
{
	printf (_("Uso:\n"
						"        matest [opciones] [archivo]\n"
						"\n"
						"Opciones:\n"
						"        -h, --help                          muestra esta ayuda y termina\n"
						"        -v, --version                       muestra la versión y termina\n"
						"        -g, --gui                           con interfaz gráfica (por defecto)\n"
						"        -t, --text                          modo interactivo en modo texto\n"
						"        -d, --dimmension <entero>           dimensión de las matrices\n"
						"        -m, --mdv        <entero>           mínimo valor designado\n"
						"        -f, --formula    <formula>          fórmula que se evaluará\n"
						"        -e, --evaluate                      valores que se evaluarán:\n"
						"                         a, all              - todos\n"
						"                         d, designated       - designados\n"
						"                         n, not-designated   - no designados\n"));
}


/*
 * Procedimiento que imprime la nota legal que debe aparecer al inicio del modo
 * interactivo, como aconseja la FSF.
 */
void
menu_init (void)
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


/*
 * Lector de archivos con matrices.
 */
int
read_matricesfile (Work *work, FILE *file)
{
	LlLogic  *logic;
	LlUCon   *ucon;
	LlBCon   *bcon;
	char     con_symbol = 0,
	         auxchar,
	         line[BUFSIZ], *auxstr,
	         value[BUFSIZ/2];
	int      i, j, dim,
	         row = 0, col = 0,
	         vector[BUFSIZ/2],
	         **mtx = NULL;

	if (file == NULL) {
		perror ("El archivo no existe.\n");
		return 1;
	}

	logic = (LlLogic*) malloc (sizeof (LlLogic));
	if (!logic) {
		perror ("Memoria insuficiente.\n");
		return 2;
	}

	while (!feof (file)) {
		while (fgets (line, BUFSIZ, file)) {
			if (sscanf (line, "%c", &auxchar)) {
				if (isspace (auxchar)) {
					if (mtx) {
						if (mtx[1][0] == -1 && con_symbol != 0) {
							printf ("* Guardamos una conectiva unaria.\n");
							ucon = ll_ucon_new (&con_symbol, mtx[0], logic->dimension);
							ll_ucon_list_append_ucon (&logic->ucons, ucon);
							con_symbol = 0;
							row = 0;
							for (i = 0; i < logic->dimension; i++)
								free (mtx[i]);
							free (mtx);
							mtx = NULL;
						}
						else if (con_symbol != 0)	{
							printf ("** Guardamos una conectiva binaria.\n");
							bcon = ll_bcon_new (&con_symbol, mtx, logic->dimension);
							ll_bcon_list_append_bcon (&logic->bcons, bcon);
							con_symbol = 0;
							row = 0;
							for (i = 0; i < logic->dimension; i++)
								free (mtx[i]);
							free (mtx);
							mtx = NULL;
						}
						else {
							printf ("Hay una matriz sin símbolo asociado.\n");
							return 9;
						}
					}
					break;
				}
				else if (isalpha (auxchar)) {
					if (con_symbol == 0) {
						printf ("No hay un símbolo de conectiva declarado.\n");
						con_symbol = auxchar;
					}
					else if (ll_ucon_list_get_ucon_by_symbol (logic->ucons, &con_symbol) ||
									 ll_bcon_list_get_bcon_by_symbol (logic->bcons, &con_symbol)) {
						fprintf (stderr, "Definición reiterada de la conectiva %c.\n", con_symbol);
						return 6;
					}
					break;
				}
			}
			if (sscanf (line, "%u", &i)) {
				col = 0;
				auxstr = line;
				while (sscanf (auxstr, "%u", &vector[col]) != EOF) {
					printf ("Se ha encontrado el valor \"%i\".\n", vector[col]);
					sprintf (value, "%u", vector[col]);
					auxstr = strstr (auxstr, value);
					auxstr = auxstr + strlen (value);
					col++;
				}
				for (i = 0; i < col; i++)
					printf ("vector[%i]:%i ", i, vector[i]);
				printf ("Guardado.\n");
				dim = col;
				printf ("Comprobamos dimension.\n");
				if (logic->dimension == 0) {
					logic->dimension = dim;
					printf ("Definida dimension = %i.\n", logic->dimension);
				}
				else if (logic->dimension != dim) {
					perror ("El número de elementos de una fila es distinto al esperado.\n");
					return 3;
				}
				printf ("Declaramos la matriz si no existía.\n");
				if (!mtx) {
					mtx = (int**) calloc (logic->dimension, sizeof (int*));
					for (i = 0; i < dim; i++)
					mtx[i] = (int*) calloc (logic->dimension, sizeof (int));
					mtx[1][0] = -1;
					printf ("Declarada.\n");
				}
				else {
					printf ("Ya estaba declarada.\n");
					if (row < logic->dimension)
						row++;
					else {
						printf ("Demasiadas filas para una matriz.\n");
						return 8;
					}
				}
				if (row > logic->dimension) {
					perror ("Una matriz tiene más filas de las esperadas.\n");
					return 4;
				}
				for (i = 0; i < logic->dimension; i++) {
					if (vector[i] > logic->dimension) {
						perror ("Hay elementos mayores que la dimensión de su matriz.\n");
						return 5;
					}
				}
				for (col = 0; col < logic->dimension; col++)
					mtx[row][col] = vector[col];
				for (i = 0; i < logic->dimension; i++)
					printf ("mtx[%i][%i]:%i ", row, i, mtx[row][i]);
				putchar ('\n');
			}
		}
	}
	if (!work->logic)
		work->logic = (LlLogic*) malloc (sizeof (LlLogic));
	if (!work->logic) {
		perror ("Memoria insuficiente.\n");
		return 2;
	}
	logic->mdv = logic->dimension - 1;
	work->logic = logic;
	return 0;
}
