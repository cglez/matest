/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * MaTest.h
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
 * @file MaTest.h
 *
 * Este archivo contiene el código común a todo el programa, esto es,
 * declaraciones del preprocesador, definición de tipos de datos y los
 * prototipos de las funciones.
 */

#ifndef __MATEST_H__
#define __MATEST_H__


/*
***	Declaraciones del preprocesador
 */

#include <stdbool.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include "logics.h"

#define VERSION "1.2"
#define MAX_FORMULA_LENGHT BUFSIZ


/*
***	Definición de tipos de datos
 */

/**
 * Enumeración con los tipos de evaluación.
 */
typedef enum
	{
		ALL,            /**< Se muestran todos los valores. */
		DESIGNATED,     /**< Se muestran sólo los valores designados. */
		NOT_DESIGNATED  /**< Se muestran sólo los valores no designados. */
	}
	EvaluationStyle;


/**
 * Estructura general con todos los datos necesarios para la evaluación, esto
 * es, contiene el trabajo definido actualmente tal y como se evaluará.
 */
typedef struct _Work Work;
struct _Work
	{
		LogicsLogic* logic;                     /**< Una lógica definida como se quiera. */
		char formula_pn[MAX_FORMULA_LENGHT];    /**< Fórmula en notación polaca. */
		LogicsWFF wff;                          /**< Una fórmula bien formada como estructura en árbol. */
		EvaluationStyle evaluation_style;       /**< El tipo de evaluación a realizar. */
		bool logic_modified;
	};


/*
 * Registro con los widgets y demás datos de la interfaz gráfica a los que se
 * necesita acceder.
 */
typedef struct
	{
		GtkWidget      *window;
		GtkWidget      *statusbar;
		GtkWidget      *text_view;
		GtkWidget      *progressbar;
		GtkWidget      *spin_dimension;
		GtkWidget      *spin_mdv;
		GtkWidget      *entry_formula;
		GtkWidget      *hb_ucons;
		GtkWidget      *hb_bcons;
		GtkTextBuffer  *textbuffer;
		GtkLabel       *label_formula;
		Work           *work;
	}
	MaTestGUI;


/*
***	Prototipos de las funciones
 */

/* Modos interactivos */
int mode_text (Work* work);
int mode_gui (int argc, char *argv[], Work* work);

/* Manejo de conectivas */
void text_ucon_add_custom (LogicsLogic* logic, char symbol);
void text_bcon_add_custom (LogicsLogic* logic, char symbol);
void print_ucon_matrix (LogicsUCon* ucon, LogicsLogic* logic);
void print_bcon_matrix (LogicsBCon* bcon, LogicsLogic* logic);
void print_matrices (LogicsLogic* logic);
int write_ucon_matrix (FILE *file, LogicsUCon* ucon, int dimension);
int write_bcon_matrix (FILE *file, LogicsBCon* bcon, int dimension);
int write_matrices (FILE *file, LogicsLogic* logic);

/* Manejo de variables */
void register_vars (LogicsLogic* logic, char formula[]);

/* Funciones relacionadas con las fórmulas en notación polaca */
//LogicsSymbolType logics_symbol_pn_get_type (char symbol, LogicsLogic* logic);
bool check_string (char formula[]);

/* Funciones relacionadas con la evaluación */
bool logics_wff_add_node (LogicsWFF *wff, LogicsSymbolType type, char name, int* value);
void logics_wff_parse_formula_pn (LogicsWFF *wff, char formula[], LogicsLogic* logic);
void del_wff (LogicsWFF *wff);
int eval_formula (LogicsWFF wff, LogicsLogic* logic);
void print_eval_formula (char formula[], LogicsLogic* logic);
char* print_current_evaluating_formula_pn (char formula_pn[], LogicsLogic* logic);
void evaluation (FILE *output, Work* work);

/* Funciones del modo texto */
char readin (char str[], char pattern[]);
void screen_clear (void);
void make_pause (void);
void menu_usage (void);
void menu_version (void);
void menu_header (void);
void menu_info (Work* work);
void menu_options (void);
void menu_init (void);
void menu_dimension (void);
void menu_about (void);
void menu_help (void);
void menu_index (Work* work);

/* Funciones del modo gráfico. */
gboolean init_gui (MaTestGUI *gui);
void dialog_error (const gchar *message);
gchar* show_matrices_gui (LogicsLogic* logic);
gchar* evaluation_gui (MaTestGUI *gui);
gint dialog_ucon_new (MaTestGUI *gui);
gint dialog_ucon_edit (MaTestGUI *gui, char symb);
gint dialog_bcon_new (MaTestGUI *gui);
gint dialog_bcon_edit (MaTestGUI *gui, char symb);


#endif /* __MATEST_H__ */