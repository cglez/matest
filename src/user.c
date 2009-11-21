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


#include <stdio.h>
#include <ctype.h>
#include <string.h>
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


/**
 * Procedimiento para limpiar la pantalla. Pretende ser multiplataforma,
 * utiliza según qué método dependiendo del parámetro que se le pase al
 * preprocesador. Para sistemas tipo Unix se llama a la función 'clear',
 * para Windows/DOS se llama a la función 'cls', por defecto se imprime una
 * serie de 24 saltos de línea.
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


/*
 * Procedimiento para hacer una pausa en la salida por la terminal.
 * Imprime un mensaje y espera hasta que el usuario pulse Intro.
 */
void make_pause (void)
{
	printf (_("\nPulse Intro para continuar... "));
	fflush (stdout);
	while (getchar() != '\n');
}


/*
 * Imprime la versión del programa. Esta función es llamada desde la línea de
 * comandos, según lo aconsejado por la FSF.
 */
void menu_version (void)
{
	printf (_("MaTest %s\n"
	          "Copyright (C) 2008, 2009 - César González Gutiérrez.\n"
						"License GPLv3+: GNU General Public Licence version 3 or later,\n"
						"see <http://gnu.org/licenses/gpl.html>.\n"
						"This is free software: you are free to change and redistribute it.\n"
						"There is NO WARRANTY, to the extent permitted by law.\n"), VERSION);
}


/*
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


/*
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
