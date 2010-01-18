/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * user_gui.c
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

#include "callbacks.h"

/* Standard gettext macros. */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


/*
 * Llamamos a init_gui() al arranque para cargar la estructura MaTestGUI, que
 * contiene las referencias a los widgets que necesitaremos. Esto se lleva a
 * cabo usando GtkBuilder para leer el archivo XML.
 */
gboolean 
init_gui (MaTestGUI *gui)
{
	GtkBuilder            *builder;
	GError                *err=NULL;
	PangoFontDescription  *font_desc;

	/* use GtkBuilder to build our interface from the XML file */
	builder = gtk_builder_new ();
	if (gtk_builder_add_from_file (builder, UI_FILE, &err) == 0)
		{
			dialog_error (err->message);
			g_error_free (err);
			return FALSE;
		}

	/* get the widgets which will be referenced in callbacks */
	gui->window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
	gui->statusbar = GTK_WIDGET (gtk_builder_get_object (builder, "statusbar"));
	gui->text_view = GTK_WIDGET (gtk_builder_get_object (builder, "text_view"));
	gui->progressbar = GTK_WIDGET (gtk_builder_get_object (builder, "progressbar"));
	gui->textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (gui->text_view));
	gui->spin_dimension = GTK_WIDGET (gtk_builder_get_object (builder, "spin_dimension"));
	gui->spin_mdv = GTK_WIDGET (gtk_builder_get_object (builder, "spin_mdv"));
	gui->entry_formula = GTK_WIDGET (gtk_builder_get_object (builder, "entry_formula"));
	gui->label_formula = GTK_LABEL (gtk_builder_get_object (builder, "label_formula"));
	gui->hb_ucons = GTK_WIDGET (gtk_builder_get_object (builder, "hb_ucons"));
	gui->hb_bcons = GTK_WIDGET (gtk_builder_get_object (builder, "hb_bcons"));

	/* connect signals, passing our TutorialTextEditor struct as user data */
	gtk_builder_connect_signals (builder, gui);

	/* free memory used by GtkBuilder object */
	g_object_unref (G_OBJECT (builder));

	/* set the text view font */
	font_desc = pango_font_description_from_string ("monospace 10");
	gtk_widget_modify_font (gui->text_view, font_desc);
	pango_font_description_free (font_desc);

	/* set the default icon to the GTK "edit" icon */
	gtk_window_set_default_icon_name (GTK_STOCK_EDIT);

	return TRUE;
}


/*
We call dialog_error() any time we want to display an error message to the
user. It will both show an error dialog and log the error to the terminal
window.
*/
void
dialog_error (const gchar *message)
{
	GtkWidget  *dialog;

	/* log to terminal window */
	g_warning (message);

	/* create an error message dialog and display modally to the user */
	dialog = gtk_message_dialog_new (NULL,
	                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                 GTK_MESSAGE_ERROR,
	                                 GTK_BUTTONS_OK,
	                                 message);

	gtk_window_set_title (GTK_WINDOW (dialog), "Error!");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}


/*
 * Modo con interfaz gráfica.
 */
int
mode_gui (int argc, char *argv[], Work* work)
{
	MaTestGUI    *gui;
	GtkWidget    *b_con;
	LogicsUCon   *ucon;
	LogicsBCon   *bcon;

	/* allocate the memory needed by our MaTestGUI struct */
	gui = g_slice_new (MaTestGUI);

	gui->work = work;
	if (gui->work->logic->dimension < 2)
		{
			gui->work->logic->dimension = 2;
			gui->work->logic->mdv = 1;
		}
	
	/* initialize GTK+ libraries */
	gtk_init (&argc, &argv);

	if (init_gui (gui) == FALSE)
		return 1; /* error loading UI */
	
	gtk_spin_button_set_range (GTK_SPIN_BUTTON (gui->spin_dimension),
	                           2, 16);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui->spin_dimension),
	                           gui->work->DIM);
	gtk_spin_button_set_range (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           1, gui->work->MAXV);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           gui->work->MAXV);	


	gui->work->logic->ucons = NULL;
	gui->work->logic->bcons = NULL;
	logics_logic_set_default_ucons_lukasiewicz (gui->work->logic);
	logics_logic_set_default_bcons_lukasiewicz (gui->work->logic);

	ucon = gui->work->logic->ucons;
	while (ucon)
		{
			b_con = gtk_button_new_with_label (g_strdup_printf ("%c", ucon->symbol));
			g_signal_connect (b_con, "clicked",
		                    G_CALLBACK (on_b_ucon_clicked), gui);
		  gtk_container_add (GTK_CONTAINER (gui->hb_ucons), b_con);
			gtk_widget_show (b_con);
			ucon = ucon->next;
		}

	bcon = gui->work->logic->bcons;
	while (bcon)
		{
			b_con = gtk_button_new_with_label (g_strdup_printf ("%c", bcon->symbol));
			g_signal_connect (G_OBJECT (b_con), "clicked",
		                    G_CALLBACK (on_b_bcon_clicked), gui);
			gtk_container_add (GTK_CONTAINER (gui->hb_bcons), b_con);
			gtk_widget_show (b_con);
			bcon = bcon->next;
		}
	
	/* show the window */
	gtk_widget_show (gui->window);

	/* enter GTK+ main loop */
	gtk_main ();

	/* free memory we allocated for MaTestGUI struct */
	g_slice_free (MaTestGUI, gui);

	return 0;
}

/* Guardar matrices a archivo */
gint
write_matrices_gui (MaTestGUI *gui)
{
	GtkWidget   *chooser;
	gchar       *filename=NULL;
	FILE        *file;

	chooser = gtk_file_chooser_dialog_new (_("Guardar matrices"),
	                                       GTK_WINDOW (gui->window),
	                                       GTK_FILE_CHOOSER_ACTION_SAVE,
	                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                       GTK_STOCK_SAVE, GTK_RESPONSE_OK,
	                                       NULL);

	if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_OK)
		{
			filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));
			file = fopen (filename, "w");
			if (file)
				{
					write_matrices (file, gui->work->logic);
					fclose (file);
				}
		}

	gtk_widget_destroy (chooser);
	return 0;
}


gchar*
print_matrices_gui (LogicsLogic* logic)
{
	int         i, j;
	GString     *text;
	LogicsUCon  *ucon = logic->ucons;
	LogicsBCon  *bcon = logic->bcons;

	text = g_string_new ("");

	while (ucon)
		{
			g_string_append_printf (text, "  %c |", ucon->symbol);
			for (i = 0; i < DIM; i++)
				{
					if (i >= MDV)
						g_string_append_printf (text, " *%x", i);
					else
						g_string_append_printf (text, "  %x", i);
				}

			g_string_append_printf (text, "\n----+");
			for (i = 0; i < DIM; i++)
				g_string_append_printf (text, "---");
		
			g_string_append_printf (text, "-\n" "    |");
			for (i = 0; i < DIM; i++)
				{
					if (ucon->matrix[i] >= MDV)
						g_string_append_printf (text, " *%x", ucon->matrix[i]);
					else
						g_string_append_printf (text, "  %x", ucon->matrix[i]);
				}
			g_string_append_printf (text, "\n\n");
			ucon = ucon->next;
		}

	while (bcon)
		{
			g_string_append_printf (text, "  %c |", bcon->symbol);
			for (i = 0; i < DIM; i++)
				{
					if (i >= MDV)
						g_string_append_printf (text, " *%x", i);
					else
						g_string_append_printf (text, "  %x", i);
				}
		
			g_string_append_printf (text, "\n----+");
			for (i = 0; i < DIM; i++)
				g_string_append_printf (text, "---");
		
			g_string_append_printf (text, "-\n");
			for (i = 0; i < DIM; i++)
				{
					if (i >= MDV)
						g_string_append_printf (text, " *%x |", i);
					else
						g_string_append_printf (text, "  %x |", i);
		
					for (j = 0; j < DIM; j++)
						{
							if (bcon->matrix[i][j] >= MDV)
								g_string_append_printf (text, " *%x", bcon->matrix[i][j]);
							else
								g_string_append_printf (text, "  %x", bcon->matrix[i][j]);
						}
					g_string_append_printf (text, "\n");
				}
			g_string_append_printf (text, "\n");
			bcon = bcon->next;
		}

	return text->str;
}


void
add_ucon_button (MaTestGUI *gui, char symb)
{
	GtkWidget    *b_ucon;

	/* Ponemos la etiqueta con el símbolo de la conectiva (en mayúscula) como
	 * cadena con sufijo 0 */
	b_ucon = gtk_button_new_with_label (g_strdup_printf ("%c", toupper (symb)));
	g_signal_connect (G_OBJECT (b_ucon), "clicked",
		                G_CALLBACK (on_b_ucon_clicked), gui);
	gtk_container_add (GTK_CONTAINER (gui->hb_ucons), b_ucon);
	gtk_widget_show (b_ucon);
}


void
add_bcon_button (MaTestGUI *gui, char symb)
{
	GtkWidget    *b_bcon;

	/* Ponemos la etiqueta con el símbolo de la conectiva (en mayúscula) como
	 * cadena con sufijo 0 */
	b_bcon = gtk_button_new_with_label (g_strdup_printf ("%c", toupper (symb)));
	g_signal_connect (G_OBJECT (b_bcon), "clicked",
		                G_CALLBACK (on_b_bcon_clicked), gui);
	gtk_container_add (GTK_CONTAINER (gui->hb_bcons), b_bcon);
	gtk_widget_show (b_bcon);
}


gint
dialog_ucon_new (MaTestGUI *gui)
{
	GtkDialog       *dialog;
	GtkWidget       *content;
	GtkWidget       *vbox;
	GtkWidget       *table;
	GtkWidget       *input_name;
	GtkWidget       *spinvalue;
	GtkAdjustment   **values;
	LogicsUCon      *ucon;
	gint            i;
	int             *matrix;
	gchar           *label;
	char            symb;

	values = g_new (GtkAdjustment, gui->work->DIM);
	matrix = g_new0 (int, gui->work->DIM);

	dialog = gtk_dialog_new_with_buttons (_("Nueva conectiva unaria"),
	                                      GTK_WINDOW (gui->window),
	                                      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                      GTK_STOCK_OK,
	                                      GTK_RESPONSE_ACCEPT,
	                                      GTK_STOCK_CANCEL,
	                                      GTK_RESPONSE_REJECT,
	                                      NULL);
	gtk_dialog_set_has_separator (dialog, FALSE);
	
	content = gtk_dialog_get_content_area (dialog);
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (content), vbox);
	input_name = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (input_name), 1);
	gtk_entry_set_width_chars (GTK_ENTRY (input_name), 1);
	gtk_container_add (GTK_CONTAINER (vbox), input_name);

	table = gtk_table_new (1, gui->work->DIM, TRUE);
	gtk_container_add (GTK_CONTAINER (vbox), table);
	for (i = 0; i < gui->work->DIM; i++)
		{
			spinvalue = gtk_spin_button_new_with_range (0, gui->work->MAXV, 1);
			gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalue), 0);
			gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalue), TRUE);
			values[i] = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (spinvalue));
			gtk_table_attach_defaults (GTK_TABLE (table),
			                           spinvalue,
			                           i, i + 1,
			                           0, 1);
		}
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			label = gtk_entry_get_text (GTK_ENTRY (input_name));
			symb = label[0];
			if (isalpha (symb))
				{
					for (i = 0; i < gui->work->DIM; i++)
						matrix[i] = (int) gtk_adjustment_get_value (values[i]);
					ucon = logics_ucon_new (toupper (symb), matrix, gui->work->DIM);
					logics_ucon_list_append_ucon (&gui->work->logic->ucons, ucon);
					add_ucon_button (gui, symb);
				}
		}
	gtk_widget_destroy (GTK_WIDGET (dialog));
	
	return 0;
}



gint
dialog_ucon_edit (MaTestGUI *gui, char symb)
{
	GtkDialog       *dialog;
	GtkWidget       *content;
	GtkWidget       *vbox;
	GtkWidget       *table;
	GtkWidget       *label_symb;
	GtkWidget       *b_delete;
	GtkWidget       **spinvalues;
	LogicsUCon      *ucon;
	gint            i;

	ucon = logics_ucon_list_get_ucon_by_symbol (gui->work->logic->ucons, symb);
	spinvalues = g_new (GtkWidget, gui->work->DIM);

	dialog = gtk_dialog_new_with_buttons (g_strdup_printf (_("Editando %c"), symb),
	                                      GTK_WINDOW (gui->window),
	                                      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                      GTK_STOCK_OK,
	                                      GTK_RESPONSE_ACCEPT,
	                                      GTK_STOCK_CANCEL,
	                                      GTK_RESPONSE_REJECT,
	                                      NULL);
	gtk_dialog_set_has_separator (dialog, FALSE);
	
	content = gtk_dialog_get_content_area (dialog);
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (content), vbox);
	label_symb = gtk_label_new (g_strdup_printf ("%c", symb));
	gtk_container_add (GTK_CONTAINER (vbox), label_symb);

	table = gtk_table_new (1, gui->work->DIM, TRUE);
	gtk_container_add (GTK_CONTAINER (vbox), table);
	for (i = 0; i < gui->work->DIM; i++)
		{
			spinvalues[i] = gtk_spin_button_new_with_range (0, gui->work->MAXV, 1);
			gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalues[i]), 0);
			gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalues[i]), TRUE);
			gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinvalues[i]),
					                       ucon->matrix[i]);
			gtk_table_attach_defaults (GTK_TABLE (table),
			                           spinvalues[i],
			                           i, i + 1,
			                           0, 1);
		}
	b_delete = gtk_button_new_with_label (_("Borrar conectiva"));
	g_signal_connect (G_OBJECT (b_delete), "clicked",
		                G_CALLBACK (on_b_ucon_del_clicked), gui);
	gtk_container_add (GTK_CONTAINER (vbox), b_delete);
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		for (i = 0; i < gui->work->DIM; i++)
			ucon->matrix[i] = gtk_spin_button_get_value_as_int (spinvalues[i]);
	gtk_widget_destroy (GTK_WIDGET (dialog));
	
	return 0;
}


gint
dialog_bcon_new (MaTestGUI *gui)
{
	GtkDialog       *dialog;
	GtkWidget       *content;
	GtkWidget       *vbox;
	GtkWidget       *table;
	GtkWidget       *input_symbol;
	GtkWidget       *spinvalue;
	GtkAdjustment   ***values;
	LogicsBCon      *bcon;
	gint            i, j;
	int             **matrix;
	gchar           *label;
	char            symb;

	values = g_new (GtkAdjustment*, gui->work->DIM);
	for (i = 0; i < gui->work->DIM; i++)
		values[i] = g_new (GtkAdjustment, gui->work->DIM);
	matrix = g_new (int*, gui->work->DIM);
	for (i = 0; i < gui->work->DIM; i++)
		matrix[i] = g_new0 (int, gui->work->DIM);

	dialog = gtk_dialog_new_with_buttons (_("Nueva conectiva binaria"),
	                                      GTK_WINDOW (gui->window),
	                                      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                      GTK_STOCK_OK,
	                                      GTK_RESPONSE_ACCEPT,
	                                      GTK_STOCK_CANCEL,
	                                      GTK_RESPONSE_REJECT,
	                                      NULL);
	gtk_dialog_set_has_separator (dialog, FALSE);
	
	content = gtk_dialog_get_content_area (dialog);
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (content), vbox);
	input_symbol = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (input_symbol), 1);
	gtk_entry_set_width_chars (GTK_ENTRY (input_symbol), 1);
	gtk_container_add (GTK_CONTAINER (vbox), input_symbol);

	table = gtk_table_new (gui->work->DIM, gui->work->DIM, TRUE);
	gtk_container_add (GTK_CONTAINER (vbox), table);
	for (i = 0; i < gui->work->DIM; i++)
		for (j = 0; j < gui->work->DIM; j++)
			{
				spinvalue = gtk_spin_button_new_with_range (0, gui->work->MAXV, 1);
				gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalue), 0);
				gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalue), TRUE);
				values[i][j] = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (spinvalue));
				gtk_table_attach_defaults (GTK_TABLE (table),
					                         spinvalue,
				                           j, j + 1,
			  		                       i, i + 1);
			}
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			label = gtk_entry_get_text (GTK_ENTRY (input_symbol));
			symb = label[0];
			if (isalpha (symb))
				{
					for (i = 0; i < gui->work->DIM; i++)
						for (j = 0; j < gui->work->DIM; j++)
							matrix[i][j] = (int) gtk_adjustment_get_value (values[i][j]);
					bcon = logics_bcon_new (toupper (symb), matrix, gui->work->DIM);
					logics_bcon_list_append_bcon (&gui->work->logic->bcons, bcon);
					add_bcon_button (gui, symb);
				}
		}
	gtk_widget_destroy (GTK_WIDGET (dialog));
	
	return 0;
}


gint
dialog_bcon_edit (MaTestGUI *gui, char symb)
{
	GtkDialog       *dialog;
	GtkWidget       *content;
	GtkWidget       *vbox;
	GtkWidget       *table;
	GtkWidget       *label_symb;
	GtkWidget       ***spinvalues;
	LogicsBCon      *bcon;
	gint            i, j;

	bcon = logics_bcon_list_get_bcon_by_symbol (gui->work->logic->bcons, symb);
	spinvalues = g_new (GtkWidget*, gui->work->DIM);
	for (i = 0; i < gui->work->DIM; i++)
		spinvalues[i] = g_new (GtkWidget, gui->work->DIM);

	dialog = gtk_dialog_new_with_buttons (g_strdup_printf (_("Editando %c"), symb),
	                                      GTK_WINDOW (gui->window),
	                                      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                      GTK_STOCK_OK,
	                                      GTK_RESPONSE_ACCEPT,
	                                      GTK_STOCK_CANCEL,
	                                      GTK_RESPONSE_REJECT,
	                                      NULL);
	gtk_dialog_set_has_separator (dialog, FALSE);
	
	content = gtk_dialog_get_content_area (dialog);
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (content), vbox);
	label_symb = gtk_label_new (g_strdup_printf ("%c", symb));
	gtk_container_add (GTK_CONTAINER (vbox), label_symb);

	table = gtk_table_new (gui->work->DIM, gui->work->DIM, TRUE);
	gtk_container_add (GTK_CONTAINER (vbox), table);
	for (i = 0; i < gui->work->DIM; i++)
		for (j = 0; j < gui->work->DIM; j++)
			{
				spinvalues[i][j] = gtk_spin_button_new_with_range (0, gui->work->MAXV, 1);
				gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalues[i][j]), 0);
				gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalues[i][j]), TRUE);
				gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinvalues[i][j]),
						                       bcon->matrix[i][j]);
				gtk_table_attach_defaults (GTK_TABLE (table),
				                           spinvalues[i][j],
				                           j, j + 1,
					                         i, i + 1);
			}
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			for (i = 0; i < gui->work->DIM; i++)
				for (j = 0; j < gui->work->DIM; j++)
					bcon->matrix[i][j] = gtk_spin_button_get_value_as_int (spinvalues[i][j]);
		}
	gtk_widget_destroy (GTK_WIDGET (dialog));
	
	return 0;
}


gchar*
get_current_evaluating_formula_pn (char formula[], LogicsLogic* logic)
{
	int i;
	GString *text;

	text = g_string_new ("");

	for (i = 0; i < (int) strlen (formula); i++)
		{
			if (logics_symbol_pn_get_type (formula[i], logic) == LOGICS_SYMBOL_VAR)
				g_string_append_printf (text, "%x", logics_var_get_value (logics_var_list_get_var_by_symbol (logic->vars, formula[i])));
			else
				g_string_append_printf (text, "%c", formula[i]);
		}

	return text->str;
}


gchar*
evaluation_gui (MaTestGUI *gui)
{
	int        all = 0,
	           desig = 0;
	LogicsVar  *var;
	gchar      *straux;
	GString    *text;

	text = g_string_new ("");

	if (!gui->work->wff)
		{
			g_string_append_printf (text, "No hay ninguna fórmula que evaluar,\nintroduzca una.");
			return text->str;
		}
	
	/* Imprime una cabecera con la fórmula en notación polaca */
	g_string_append_printf (text, "%s\n\n", gui->work->formula_pn);
	
	/*
	 * El algoritmo contador
	 */
	void action (Work* work, int *all, int *desig)
		{
			int i;
			
			i = eval_formula (gui->work->wff, gui->work->logic);
			/* Cuenta cada evaluación */
			(*all)++;
			/* Imprime una evaluación dependiendo del tipo de evaluación seleccionado
			 * y cuenta los valores designados */
			if (i >= gui->work->MDV)
				{
					(*desig)++;
					if (gui->work->evaluation_style == ALL || gui->work->evaluation_style == DESIGNATED)
						{
							 g_string_append_printf (text, get_current_evaluating_formula_pn (gui->work->formula_pn, gui->work->logic));
							 g_string_append_printf (text, " *%x\n", i);
						}
				 }
			else
				{
					if (gui->work->evaluation_style == ALL || gui->work->evaluation_style == NOT_DESIGNATED)
						{
							g_string_append_printf (text, get_current_evaluating_formula_pn (gui->work->formula_pn, gui->work->logic));
							g_string_append_printf (text, "  %x\n", i);
						}
				}
		}
	
	/* Condición inicial: todos los valores inicializados a 0 */
	var = gui->work->logic->vars;
	while (var)
		{
			var->value = 0;
			var = var->next;
		}
	/* Primera acción con la primera de las condiciones */
	action (gui->work, &all, &desig);
	/* El contador */
	var = gui->work->logic->vars;
	do
		{
			if (logics_var_get_value (var) < gui->work->DIM - 1)
				{
					logics_var_set_value (var, logics_var_get_value (var) + 1);
					var = gui->work->logic->vars;
					action (gui->work, &all, &desig);
				}
			else
				{
					logics_var_set_value (var, 0);
					var = var->next;
				}
		}
	while (var);
	
	/* Imprime las estadísticas */
	//g_string_append_printf (text, "\n%i posibilidades evaluadas.\n", all);
	if (desig == all)
		g_string_prepend (text, "TAUTOLOGÍA.\n\n");
	else if (desig == 0)
		g_string_prepend (text, "CONTRADICCIÓN.\n\n");
	else
		g_string_prepend (text, "Se FALSA la fórmula.\n\n");
	
	if (gui->work->evaluation_style == ALL || gui->work->evaluation_style == DESIGNATED)
		{
			straux = g_strdup_printf ("%i/%i valores designados.\n", desig, all);
			g_string_prepend (text, straux);
			g_free (straux);
		}
	else
		{
			straux = g_strdup_printf ("%i/%i valores no designados.\n", all - desig, all);
			g_string_prepend (text, straux);
			g_free (straux);
		}
	
	return text->str;
}
