/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * MaTest.h
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
 * @file MaTest.h
 *
 * Este archivo contiene el código común a todo el programa, esto es,
 * declaraciones del preprocesador, definición de tipos de datos y los
 * prototipos de las funciones.
 */

#ifndef _MATEST_H_
#define _MATEST_H_


/*
***	Declaraciones del preprocesador
 */

#include <stdbool.h>
#include <libintl.h>
#include "logics.h"

#define VERSION "1.1"
#define MAX_FORMULA_LENGHT BUFSIZ



/*
***	Definición de tipos de datos
 */

/**
 * Enumeración con los tipos de evaluación.
 */
typedef enum _evaluation_style
	{
		ALL,           /**< Se muestran todos los valores. */
		DESIGNATED,    /**< Se muestran sólo los valores designados. */
		NOTDESIGNATED  /**< Se muestran sólo los valores no designados. */
	}
	EvalStyle;


/*
typedef struct
	{
		int position;
		int *vector;
	}
	LIFO;
*/


/**
 * Estructura general con todos los datos necesarios para la evaluación, esto
 * es, contiene el trabajo definido actualmente tal y como se evaluará.
 */
typedef struct _work
	{
		Logic logic;                          /**< Una lógica definida como se quiera. */
		char formula_pn[MAX_FORMULA_LENGHT];  /**< Fórmula en notación polaca. */
		LogicWFF wff;                         /**< Una fórmula bien formada como estructura en árbol. */
		EvalStyle eval_values;                /**< El tipo de evaluación a realizar. */
	}
	workType;

typedef workType *Work;


/*
***	Prototipos de las funciones
 */

/* Prototipos de las funciones relacionadas con el manejo de conectivas.
 * Funciones presentes en el archivo connectives.c */
void add_custom_UCon (LogicUConList *list, char name, int dimension);
void add_custom_BCon (Logic logic, char name);
void set_default_UCons (Logic logic);
void set_default_BCons (Logic logic);
void print_uny_matrix (LogicUCon connective, Logic logic);
void print_bin_matrix (LogicBCon connective, Logic logic);
void show_matrices (Logic logic);
int write_uny_matrix (FILE *file, LogicUCon connective, int dimension);
int write_bin_matrix (FILE *file, LogicBCon connective, int dimension);
int write_matrices (FILE *file, Logic logic);


/* Prototipos de las funciones relacionadas con el manejo de variables.
 * Funciones presentes en el archivo variables.c */
void register_vars (Logic logic, char formula[]);


/* Prototipos de las funciones relacionadas con las fórmulas en notación polaca.
 * Funciones presentes en el archivo wffs_pn.c */
LogicSymbKind symbol_kind_pn (char symbol, Logic logic);
bool check_string (char formula[]);
bool is_wff_pn (char formula[], Logic logic);


/* Prototipos de las funciones relacionadas con la evaluación.
 * Funciones presentes en el archivo evaluation.c */
bool set_atom (LogicWFF *tree, LogicSymbKind kind, char name, int *value);
void parse_polish (LogicWFF *tree, char formula[], Logic logic);
void del_wff (LogicWFF *wff);
int eval_formula (LogicWFF wff, Logic logic);
void print_eval_formula (char formula[], Logic logic);
void evaluation (Work work);


/* Prototipos de las funciones que interactúan con el usuario.
 * Funciones presentes en el archivo user.c */
char readin (char str[], char pattern[]);
void clear_scr (void);
void make_pause (void);
void menu_header (void);
void menu_info (Work work);
void menu_options (void);
void menu_init (void);
void menu_dimension (void);
void menu_about (void);
void menu_help (void);
void menu_index (Work work);


#endif
