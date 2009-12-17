/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * user_gui.c
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
 * Llamamos a init_app() al arranque para cargar la estructura MaTestGUI, que
 * contiene las referencias a los widgets que necesitaremos. Esto se lleva a
 * cabo usando GtkBuilder para leer el archivo XML.
 */
gboolean 
init_app (MaTestGUI *gui)
{
	GtkBuilder            *builder;
	GError                *err=NULL;
	PangoFontDescription  *font_desc;

	/* use GtkBuilder to build our interface from the XML file */
	builder = gtk_builder_new ();
	if (gtk_builder_add_from_file (builder, UI_FILE, &err) == 0)
		{
			error_message (err->message);
			g_error_free (err);
			return FALSE;
		}

	/* get the widgets which will be referenced in callbacks */
	gui->window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
	gui->statusbar = GTK_WIDGET (gtk_builder_get_object (builder, "statusbar"));
	gui->text_view = GTK_WIDGET (gtk_builder_get_object (builder, "text_view"));
	gui->progressbar = GTK_WIDGET (gtk_builder_get_object (builder, "progressbar"));
	gui->textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (gui->text_view));
	gui->spin_mdv = GTK_WIDGET (gtk_builder_get_object (builder, "spin_mdv"));
	gui->entry_formula = GTK_WIDGET (gtk_builder_get_object (builder, "entry_formula"));
	gui->label_dimmension = GTK_LABEL (gtk_builder_get_object (builder, "label_dimmension"));
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

	gtk_label_set_text (gui->label_dimmension,
	                    g_strdup_printf ("%ix%i", gui->work->DIM, gui->work->DIM));

	return TRUE;
}


/*
We call error_message() any time we want to display an error message to the
user. It will both show an error dialog and log the error to the terminal
window.
*/
void
error_message (const gchar *message)
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
mode_gui (int argc, char *argv[], Work work)
{
	MaTestGUI   *gui;
	GtkWidget   *b_con;
	LogicUCon   unyaux;
	LogicBCon   binaux;

	/* allocate the memory needed by our MaTestGUI struct */
	gui = g_slice_new (MaTestGUI);

	gui->work = work;
	if (gui->work->logic->dim < 2)
		{
			gui->work->logic->dim = 2;
			gui->work->logic->mdv = 1;
		}

	/* initialize GTK+ libraries */
	gtk_init (&argc, &argv);

	if (init_app (gui) == FALSE) return 1; /* error loading UI */

	gtk_spin_button_set_range (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           1, gui->work->MAXV);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           gui->work->MAXV);

	gui->work->logic->UCons = NULL;
	gui->work->logic->BCons = NULL;
	set_default_UCons (gui->work->logic);
	set_default_BCons (gui->work->logic);
	
	unyaux = gui->work->logic->UCons;
	while (unyaux)
		{
			b_con = gtk_button_new_with_label (g_strdup_printf ("%c", unyaux->name));
			g_signal_connect (b_con, "clicked",
		                    G_CALLBACK (on_b_ucon_clicked), (gpointer) unyaux->name);
		  gtk_container_add (GTK_CONTAINER (gui->hb_ucons), b_con);
			gtk_widget_show (b_con);
			unyaux = unyaux->next;
		}

	binaux = gui->work->logic->BCons;
	while (binaux)
		{
			b_con = gtk_button_new_with_label (g_strdup_printf ("%c", binaux->name));
			g_signal_connect (G_OBJECT (b_con), "clicked",
		                    G_CALLBACK (on_b_bcon_clicked), gui);
			gtk_container_add (GTK_CONTAINER (gui->hb_bcons), b_con);
			gtk_widget_show (b_con);
			binaux = binaux->next;
		}
	
	/* show the window */
	gtk_widget_show (gui->window);

	/* enter GTK+ main loop */
	gtk_main ();

	/* free memory we allocated for MaTestGUI struct */
	g_slice_free (MaTestGUI, gui);

	return 0;
}

void
add_ucon_button (MaTestGUI *gui, char symb)
{
	GtkWidget    *b_ucon;

	b_ucon = gtk_button_new_with_label ((char) g_strdup_printf ("%c", symb));
	g_signal_connect (G_OBJECT (b_ucon), "clicked",
		                G_CALLBACK (on_b_ucon_clicked), gui);
	gtk_container_add (GTK_CONTAINER (gui->hb_ucons), b_ucon);
	gtk_widget_show (b_ucon);
}


gchar*
show_matrices_gui (Logic logic)
{
	int i, j;
	GString *text;
	LogicUCon uaux = logic->UCons;
	LogicBCon baux = logic->BCons;

	text = g_string_new ("");

	while (uaux)
		{
			g_string_append_printf (text, "  %c |", uaux->name);
			for (i = 0; i < DIM; i++)
				{
					if (i >= MDV)
						g_string_append_printf (text, " *%i", i);
					else
						g_string_append_printf (text, "  %i", i);
				}

			g_string_append_printf (text, "\n----+");
			for (i = 0; i < DIM; i++)
				g_string_append_printf (text, "---");
		
			g_string_append_printf (text, "-\n" "    |");
			for (i = 0; i < DIM; i++)
				{
					if (uaux->matrix[i] >= MDV)
						g_string_append_printf (text, " *%i", uaux->matrix[i]);
					else
						g_string_append_printf (text, "  %i", uaux->matrix[i]);
				}
			g_string_append_printf (text, "\n\n");
			uaux = uaux->next;
		}

	while (baux)
		{
			g_string_append_printf (text, "  %c |", baux->name);
			for (i = 0; i < DIM; i++)
				{
					if (i >= MDV)
						g_string_append_printf (text, " *%i", i);
					else
						g_string_append_printf (text, "  %i", i);
				}
		
			g_string_append_printf (text, "\n----+");
			for (i = 0; i < DIM; i++)
				g_string_append_printf (text, "---");
		
			g_string_append_printf (text, "-\n");
			for (i = 0; i < DIM; i++)
				{
					if (i >= MDV)
						g_string_append_printf (text, " *%i |", i);
					else
						g_string_append_printf (text, "  %i |", i);
		
					for (j = 0; j < DIM; j++)
						{
							if (baux->matrix[i][j] >= MDV)
								g_string_append_printf (text, " *%i", baux->matrix[i][j]);
							else
								g_string_append_printf (text, "  %i", baux->matrix[i][j]);
						}
					g_string_append_printf (text, "\n");
				}
			g_string_append_printf (text, "\n");
			baux = baux->next;
		}

	return text->str;
}


gint
add_ucon_gui (MaTestGUI *gui)
{
	GtkDialog       *dialog;
	GtkWidget       *content;
	GtkWidget       *vbox, *hbox;
	GtkWidget       *table;
	GtkWidget       *label_name;
	GtkWidget       *input_name;
	GtkWidget       *spinvalue;
	GtkAdjustment   **values;
	gint            i, resp;
	int             *matrix;
	gchar           *symb;

	values = g_new (GtkAdjustment, gui->work->DIM);
	matrix = g_new0 (int, gui->work->DIM);

	dialog = gtk_dialog_new_with_buttons (_("Nueva conectiva unaria"),
	                                      GTK_WINDOW (gui->window),
	                                      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                      GTK_STOCK_OK,
	                                      GTK_RESPONSE_ACCEPT,
	                                      //GTK_STOCK_CANCEL,
	                                      //GTK_RESPONSE_REJECT,
	                                      NULL);
	gtk_dialog_set_has_separator (dialog, FALSE);
	
	content = gtk_dialog_get_content_area (dialog);
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (content), vbox);
	//hbox = gtk_hbox_new (FALSE, 0);
	//gtk_container_add (GTK_CONTAINER (vbox), hbox);
	//label_name = gtk_label_new (_("Símbolo: "));
	//gtk_container_add (GTK_CONTAINER (hbox), label_name);
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
			symb = gtk_entry_get_text (GTK_ENTRY (input_name));
			if (isalpha (symb[0]))
				{
					for (i = 0; i < gui->work->DIM; i++)
						matrix[i] = (int) gtk_adjustment_get_value (values[i]);
					
					add_UCon (&gui->work->logic->UCons,
						        toupper ((char) symb[0]),
				  		      matrix,
				    		    gui->work->DIM);
					add_ucon_button (gui, symb);
					
				}
		}
	gtk_widget_destroy (GTK_WIDGET (dialog));
	
	return 0;
}



gint
edit_ucon_gui (MaTestGUI *gui, char symb)
{
	GtkDialog       *dialog;
	GtkWidget       *content;
	GtkWidget       *vbox;
	GtkWidget       *table;
	GtkWidget       *label_symb;
	GtkWidget       **spinvalues;
	LogicUCon       ucon;
	gint            i, resp;

	ucon = search_UCon (gui->work->logic->UCons, symb);
	spinvalues = g_new (GtkWidget, gui->work->DIM);

	dialog = gtk_dialog_new_with_buttons (_("Editando"),
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
	label_symb = gtk_label_new ("");
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
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		for (i = 0; i < gui->work->DIM; i++)
			ucon->matrix[i] = gtk_spin_button_get_value_as_int (spinvalues[i]);
	gtk_widget_destroy (GTK_WIDGET (dialog));
	
	return 0;
}


/*
void
edit_ucon_gui (Logic logic, char name)
{
	GtkWidget       *win;
	GtkWidget       *table;
	GtkWidget       *spinvalue;
	GtkWidget       *vbox, *hbox;
	GtkWidget       *b_ok, *b_cancel;
	LogicBCon       bcon;
	guint           i, j;
	
	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (win), g_strdup_printf ("Editando %c", name));
	
	bcon = search_BCon (logic->BCons, name);
	
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (win), vbox);
	
	table = gtk_table_new (DIM, DIM, TRUE);
	gtk_container_add (GTK_CONTAINER (vbox), table);
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				{
					spinvalue = gtk_spin_button_new_with_range (0, MAXV, 1);
					gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinvalue),
					                           bcon->matrix[i][j]);					
					gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalue), 0);
					gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalue), T[i]);
					gtk_table_attach_defaults (GTK_TABLE (table),
					                           spinvalue,
					                           i, i + 1,
					                           j, j + 1);
					gtk_widget_show (spinvalue);
				}
		}
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox), hbox);
	b_cancel = gtk_button_new_with_label (_("Cancelar"));
	gtk_container_add (GTK_CONTAINER (hbox), b_cancel);
	b_ok = gtk_button_new_with_label (_("OK"));
	gtk_container_add (GTK_CONTAINER (hbox), b_ok);
	
	gtk_widget_show (vbox);
	gtk_widget_show (hbox);
	gtk_widget_show (table);
	gtk_widget_show (b_ok);
	gtk_widget_show (b_cancel);
	gtk_widget_show (win);
}
*/

gint
add_bcon_gui (Logic logic)
{
	GtkWidget       *win;
	GtkWidget       *table;
	GtkWidget       *spinvalue;
	guint           i, j;

	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (win), _("Nueva conectiva binaria"));

	table = gtk_table_new (DIM, DIM, TRUE);
	gtk_container_add (GTK_CONTAINER (win), table);
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				{
					spinvalue = gtk_spin_button_new_with_range (0, MAXV, 1);
					gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalue), 0);
					gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalue), TRUE);
					gtk_table_attach_defaults (GTK_TABLE (table),
					                           spinvalue,
					                           i, i + 1,
					    j, j + 1);
					gtk_widget_show (spinvalue);
				}
		}
	gtk_widget_show (table);
	gtk_widget_show (win);

	return 0;
}


void
edit_bcon_gui (Logic logic, char name)
{
	GtkWidget       *win;
	GtkWidget       *table;
	GtkWidget       *spinvalue;
	GtkWidget       *vbox, *hbox;
	GtkWidget       *b_ok, *b_cancel;
	LogicBCon       bcon;
	guint           i, j;

	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (win), g_strdup_printf ("Editando %c", name));

	bcon = search_BCon (logic->BCons, name);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (win), vbox);

	table = gtk_table_new (DIM, DIM, TRUE);
	gtk_container_add (GTK_CONTAINER (vbox), table);
	for (i = 0; i < DIM; i++)
		{
			for (j = 0; j < DIM; j++)
				{
					spinvalue = gtk_spin_button_new_with_range (0, MAXV, 1);
					gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinvalue),
					                           bcon->matrix[i][j]);
					gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalue), 0);
					gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalue), TRUE);
					gtk_table_attach_defaults (GTK_TABLE (table),
					                           spinvalue,
					                           i, i + 1,
					                           j, j + 1);
					gtk_widget_show (spinvalue);
				}
		}
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (vbox), hbox);
	b_cancel = gtk_button_new_with_label (_("Cancelar"));
	gtk_container_add (GTK_CONTAINER (hbox), b_cancel);
	b_ok = gtk_button_new_with_label (_("OK"));
	gtk_container_add (GTK_CONTAINER (hbox), b_ok);
	
	gtk_widget_show (vbox);
	gtk_widget_show (hbox);
	gtk_widget_show (table);
	gtk_widget_show (b_ok);
	gtk_widget_show (b_cancel);
	gtk_widget_show (win);
}


/*
void
add_formula_gui (MaTestGUI *gui)
{
	NewFormulaGUI *newf_gui;

	newf_gui = g_slice_new (NewFormulaGUI);
	newf_gui->work = &gui->work;

	newf_gui->win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (newf_gui->win), _("Nueva fórmula"));
	newf_gui->hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (newf_gui->win), newf_gui->hbox);
	gtk_widget_show (newf_gui->hbox);

	newf_gui->entry = gtk_entry_new ();
	gtk_entry_set_width_chars (newf_gui->entry, 15);
	gtk_container_add (GTK_CONTAINER (newf_gui->hbox), newf_gui->entry);
	gtk_widget_show (newf_gui->entry);

	newf_gui->button = gtk_button_new_with_label (_("Aceptar"));
	g_signal_connect (newf_gui->button, "clicked",
	                  G_CALLBACK (on_b_new_formula_ok_clicked), newf_gui);
  gtk_container_add (GTK_CONTAINER (newf_gui->hbox), newf_gui->button);
	gtk_widget_show (newf_gui->button);

	gtk_widget_show (newf_gui->win);
}
*/


gchar*
get_current_evaluating_formula_pn (char formula[], Logic logic)
{
	int i;
	GString *text;

	text = g_string_new ("");

	for (i = 0; i < (int) strlen (formula); i++)
		{
			if (symbol_kind_pn (formula[i], logic) == VAR)
				g_string_append_printf (text, "%i", var_value (search_var (logic->Vars, formula[i])));
			else
				g_string_append_printf (text, "%c", formula[i]);
		}

	return text->str;
}


gchar*
evaluation_gui (MaTestGUI *gui)
{
	int all = 0, desig = 0;
	LogicVar var;
	GString *text;

	text = g_string_new ("");
	
	/* Imprime una cabecera con la fórmula en notación polaca */
	g_string_append_printf (text, "%s\n\n", gui->work->formula_pn);
	
	/*
	 * El algoritmo contador
	 */
	void action (Work work, int *all, int *desig)
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
					if (gui->work->eval_values == ALL || gui->work->eval_values == DESIGNATED)
						{
							 g_string_append_printf (text, get_current_evaluating_formula_pn (gui->work->formula_pn, gui->work->logic));
							 g_string_append_printf (text, " *%i\n", i);
						}
				 }
			else
				{
					if (gui->work->eval_values == ALL || gui->work->eval_values == NOTDESIGNATED)
						{
							g_string_append_printf (text, get_current_evaluating_formula_pn (gui->work->formula_pn, gui->work->logic));
							g_string_append_printf (text, "  %i\n", i);
						}
				}
		}
	
	/* Condición inicial: todos los valores inicializados a 0 */
	var = gui->work->logic->Vars;
	while (var)
		{
			var->value = 0;
			var = var->next;
		}
	/* Primera acción con la primera de las condiciones */
	action (gui->work, &all, &desig);
	/* El contador */
	var = gui->work->logic->Vars;
	do
		{
			if (var_value (var) < gui->work->DIM - 1)
				{
					set_var_value (var, var_value (var) + 1);
					var = gui->work->logic->Vars;
					action (gui->work, &all, &desig);
				}
			else
				{
					set_var_value (var, 0);
					var = var->next;
				}
		}
	while (var);
	
	/* Imprime las estadísticas */
	g_string_append_printf (text, "\n%i posibilidades evaluadas.\n", all);
	if (gui->work->eval_values == ALL || gui->work->eval_values == DESIGNATED)
		g_string_append_printf (text, "%i valores designados.\n", desig);
	else
		g_string_append_printf (text, "%i valores no designados.\n", all - desig);
	if (desig == all)
		g_string_append_printf (text, "TAUTOLOGÍA.\n");
	else if (desig == 0)
		g_string_append_printf (text, "CONTRADICCIÓN.\n");
	else
		g_string_append_printf (text, "Las matrices dadas FALSAN la fórmula.\n");
	
	return text->str;
}
