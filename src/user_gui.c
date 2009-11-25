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

#include <stdlib.h>
#include "callbacks.h"


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
	if (gtk_builder_add_from_file (builder, BUILDER_UI_FILE, &err) == 0)
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
	                    g_strdup_printf ("%ix%i", gui->work->logic->dim, gui->work->logic->dim));

	refresh_gui (gui);

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
	MaTestGUI *gui;

	/* allocate the memory needed by our MaTestGUI struct */
	gui = g_slice_new (MaTestGUI);

	gui->work = (Work) malloc (sizeof (workType));
	gui->work->logic = (Logic) malloc (sizeof (LogicType));
	gui->work->DIM = 3;
	gui->work->MDV = gui->work->MAXV;
	gui->work->formula_pn[0] = 0;
	gui->work->wff = NULL;
	set_default_UCons (gui->work->logic);
	set_default_BCons (gui->work->logic);

	/* initialize GTK+ libraries */
	gtk_init (&argc, &argv);

	if (init_app (gui) == FALSE) return 1; /* error loading UI */

	/* show the window */
	gtk_widget_show (gui->window);

	/* enter GTK+ main loop */
	gtk_main ();

	/* free memory we allocated for MaTestGUI struct */
	g_slice_free (MaTestGUI, gui);

	return 0;
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
			g_string_append_printf (text, "\n");
			uaux = uaux->next;
		}

	while (baux)
		{
			g_string_append_printf (text, "\n  %c |", baux->name);
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
			baux = baux->next;
		}

	return text->str;
}


gint
add_BCon_gui (Logic logic)
{
	GtkWidget       *win;
	GtkWidget       *table;
	GtkWidget       *spinvalue;
	guint           i, j;

	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (win), Q_("Nueva conectiva binaria"));

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
edit_bin_con_gui (Logic logic, char name)
{
	GtkWidget       *win;
	GtkWidget       *table;
	GtkWidget       *spinvalue;
	LogicBCon       bcon;
	guint           i, j;

	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (win), g_strdup_printf ("Editando %c", name));

	bcon = search_BCon (logic->BCons, name);

	table = gtk_table_new (DIM, DIM, TRUE);
	gtk_container_add (GTK_CONTAINER (win), table);
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
	gtk_widget_show (table);
	gtk_widget_show (win);
}


void
refresh_gui (MaTestGUI *gui)
{
	GtkWidget   *b_con;
	EditBinCon  *bcon_data;
	LogicUCon   unyaux;
	LogicBCon   binaux;

	bcon_data = g_slice_new (EditBinCon);
	bcon_data->gui = gui;
	
	gtk_spin_button_set_range (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           1, gui->work->MAXV);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           gui->work->MDV);

	unyaux = gui->work->logic->UCons;
	while (unyaux)
		{
			b_con = gtk_button_new_with_label (g_strdup_printf ("%c", unyaux->name));
			g_signal_connect (G_OBJECT (b_con), "clicked",
		                    G_CALLBACK (on_b_uny_con_clicked), (gpointer) unyaux->name);
		  gtk_container_add (GTK_CONTAINER (gui->hb_ucons), b_con);
			gtk_widget_show (b_con);
			unyaux = unyaux->next;
		}

	binaux = gui->work->logic->BCons;
	while (binaux)
		{
			b_con = gtk_button_new_with_label (g_strdup_printf ("%c", binaux->name));
			bcon_data->bcon = binaux;
			g_signal_connect (G_OBJECT (b_con), "clicked",
		                    G_CALLBACK (on_b_bin_con_clicked), gui);
			gtk_container_add (GTK_CONTAINER (gui->hb_bcons), b_con);
			gtk_widget_show (b_con);
			binaux = binaux->next;
		}

	gtk_label_set_text (gui->label_formula,
	                    g_strdup_printf ("Fórmula: %s", gui->work->formula_pn));

	
}
