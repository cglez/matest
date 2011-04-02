/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * user_gui.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <string.h>
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

	/* usamos GtkBuilder para contruir la interfaz desde el archivo XML */
	builder = gtk_builder_new ();
	if (gtk_builder_add_from_file (builder, UI_FILE, &err) == 0)
		{
			dialog_error (err->message);
			g_error_free (err);
			return FALSE;
		}

	/* obtenemos los widgets que van a ser referenciados a llamadas */
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
	gui->m_view_all = GTK_WIDGET (gtk_builder_get_object (builder, "m_view_all"));
	gui->m_view_desig = GTK_WIDGET (gtk_builder_get_object (builder, "m_view_desig"));
	gui->m_view_notdesig = GTK_WIDGET (gtk_builder_get_object (builder, "m_view_notdesig"));

	/* conecta las señales, pasamos el registro MaTestGUI como datos de usuario */
	gtk_builder_connect_signals (builder, gui);

	/* libera la memoria del objeto GtkBuilder */
	g_object_unref (G_OBJECT (builder));

	/* ajustamos la fuente del área de texto */
	font_desc = pango_font_description_from_string ("monospace 10");
	gtk_widget_modify_font (gui->text_view, font_desc);
	pango_font_description_free (font_desc);

	return TRUE;
}


/*
 * Muestra un mensaje de error al usuario. Se muestra un diálogo de error y
 * se registra en la terminal también.
 */
void
dialog_error (const gchar *message)
{
	GtkWidget  *dialog;

	/* escribe el error en la terminal */
	g_warning ("%s", message);

	/* crea un diálogo modal con el mensaje de error */
	dialog = gtk_message_dialog_new (NULL,
	                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                 GTK_MESSAGE_ERROR,
	                                 GTK_BUTTONS_OK,
	                                 "%s", message);

	gtk_window_set_title (GTK_WINDOW (dialog), "Error!");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}


/*
 * Modo interactivo con interfaz gráfica.
 */
int
mode_gui (int argc, char *argv[], Work* work)
{
	MaTestGUI          *gui;
	GtkWidget          *b_con;
	LlUCon             *ucon;
	LlBCon             *bcon;
	int                initdim = work->DIM,
	                   initmdv = work->MDV;
	EvaluationStyle    init_ev_style = work->evaluation_style;

	/* asigna memoria para el registro MaTestGUI */
	gui = g_slice_new (MaTestGUI);

	gui->work = work;

	/* inicializamos las librerías GTK+ */
	gtk_init (&argc, &argv);

	if (init_gui (gui) == FALSE)
		return 1; /* error cargando la interfaz */

	/* inicializa los rangos de la dimensión y el mínimo valor designado así como
	 * sus valores iniciales */
	gtk_spin_button_set_range (GTK_SPIN_BUTTON (gui->spin_dimension),
	                           2, 16);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui->spin_dimension),
	                           initdim);
	gtk_spin_button_set_range (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           1, gui->work->MAXV);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           initmdv);

	/* establece el estilo de evaluación inicial */
	switch (init_ev_style)
		{
			case ALL:
				gtk_menu_item_activate (GTK_MENU_ITEM (gui->m_view_all));
				break;
			case DESIGNATED:
				gtk_menu_item_activate (GTK_MENU_ITEM (gui->m_view_desig));
				break;
			case NOT_DESIGNATED:
				gtk_menu_item_activate (GTK_MENU_ITEM (gui->m_view_notdesig));
				break;
		}

	/* define las conectivas por defecto */
	gui->work->logic->ucons = NULL;
	gui->work->logic->bcons = NULL;
	ll_logic_set_default_ucons_lukasiewicz (gui->work->logic);
	ll_logic_set_default_bcons_lukasiewicz (gui->work->logic);

	/* crea los botones para las conectivas */
	ucon = gui->work->logic->ucons;
	while (ucon)
		{
			b_con = gtk_button_new_with_label (g_strdup_printf ("%s", ucon->symbol));
			g_signal_connect (b_con, "clicked",
		                    G_CALLBACK (on_b_ucon_clicked), gui);
		  gtk_container_add (GTK_CONTAINER (gui->hb_ucons), b_con);
			gtk_widget_show (b_con);
			ucon = ucon->next;
		}
	bcon = gui->work->logic->bcons;
	while (bcon)
		{
			b_con = gtk_button_new_with_label (g_strdup_printf ("%s", bcon->symbol));
			g_signal_connect (G_OBJECT (b_con), "clicked",
		                    G_CALLBACK (on_b_bcon_clicked), gui);
			gtk_container_add (GTK_CONTAINER (gui->hb_bcons), b_con);
			gtk_widget_show (b_con);
			bcon = bcon->next;
		}

	/* Muestra la ventana */
	gtk_widget_show (gui->window);

	/* Entra en el bucle principal GTK+ */
	gtk_main ();

	/* Liberamos la memoria usada por el registro MaTestGUI */
	g_slice_free (MaTestGUI, gui);

	return 0;
}


/*
 * Diálogo Acerca de
 */
gboolean
dialog_about ()
{
	GtkWidget     *dialog;
	GtkBuilder    *builder;
	GError        *err=NULL;

	/* usamos GtkBuilder para contruir el díalogo desde su archivo XML */
	builder = gtk_builder_new ();
	if (gtk_builder_add_from_file (builder, ABOUT_DIALOG_FILE, &err) == 0)
		{
			dialog_error (err->message);
			g_error_free (err);
			return FALSE;
		}

	dialog = GTK_WIDGET (gtk_builder_get_object (builder, "aboutdialog"));
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
	g_object_unref (G_OBJECT (builder));

	return TRUE;
}


/*
 * Guardar matrices a archivo
 */
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


/*
 * Imprime las matrices en el área de texto.
 */
gchar*
print_matrices_gui (LlLogic* logic)
{
	int         i, j;
	GString     *text;
	LlUCon      *ucon = logic->ucons;
	LlBCon      *bcon = logic->bcons;

	text = g_string_new ("");

	while (ucon)
		{
			g_string_append_printf (text, "  %s |", ucon->symbol);
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
			g_string_append_printf (text, "  %s |", bcon->symbol);
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
add_ucon_button (MaTestGUI *gui, char symbol[])
{
	GtkWidget    *b_ucon;
	gchar        *label;

	/* Ponemos la etiqueta con el símbolo de la conectiva (en mayúscula) */
	label = g_strdup_printf ("%s", g_ascii_strup (symbol, -1));
	b_ucon = gtk_button_new_with_label (label);
	g_free (label);
	g_signal_connect (G_OBJECT (b_ucon), "clicked",
		                G_CALLBACK (on_b_ucon_clicked), gui);
	gtk_container_add (GTK_CONTAINER (gui->hb_ucons), b_ucon);
	gtk_widget_show (b_ucon);
}


void
add_bcon_button (MaTestGUI *gui, char symbol[])
{
	GtkWidget    *b_bcon;
	gchar        *label;

	/* Ponemos la etiqueta con el símbolo de la conectiva (en mayúscula) */
	label = g_strdup_printf ("%s", g_ascii_strup (symbol, -1));
	b_bcon = gtk_button_new_with_label (label);
	g_free (label);
	g_signal_connect (G_OBJECT (b_bcon), "clicked",
		                G_CALLBACK (on_b_bcon_clicked), gui);
	gtk_container_add (GTK_CONTAINER (gui->hb_bcons), b_bcon);
	gtk_widget_show (b_bcon);
}


/*
void
del_bcon_button (MaTestGUI *gui, char symbol)
{
	GtkWidget    *b_bcon;

	gtk_button_get_by_symbol
*/

gint
dialog_ucon_new (MaTestGUI *gui)
{
	GtkDialog       *dialog;
	GtkWidget       *content,
									*table,
									*input_symbol,
									*label_col,
									*spinvalue;
	GtkAdjustment   **values;
	LlUCon          *ucon;
	gint            i;
	int             *matrix;
	const gchar     *label;

	values = (GtkAdjustment**) g_new (GtkAdjustment, gui->work->DIM);
	matrix = g_new0 (int, gui->work->DIM);

	dialog = GTK_DIALOG (gtk_dialog_new_with_buttons (_("Nueva conectiva unaria"),
	                                                  GTK_WINDOW (gui->window),
	                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                                  GTK_STOCK_OK,
	                                                  GTK_RESPONSE_ACCEPT,
	                                                  GTK_STOCK_CANCEL,
	                                                  GTK_RESPONSE_REJECT,
	                                                  NULL));
	gtk_dialog_set_has_separator (dialog, FALSE);

	content = gtk_dialog_get_content_area (dialog);
	table = gtk_table_new (2, gui->work->DIM + 1, FALSE);
	gtk_container_add (GTK_CONTAINER (content), table);
	input_symbol = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (input_symbol), 1);
	gtk_entry_set_width_chars (GTK_ENTRY (input_symbol), 1);
	gtk_table_attach_defaults (GTK_TABLE (table), input_symbol, 0, 1, 0, 1);

	for (i = 0; i < gui->work->DIM; i++)
		{
			label_col = gtk_label_new (g_strdup_printf ("%x", i));
			gtk_table_attach_defaults (GTK_TABLE (table), label_col,
                                 i + 1, i + 2,
                                 0, 1);

			spinvalue = gtk_spin_button_new_with_range (0, gui->work->MAXV, 1);
			gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalue), 0);
			gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalue), TRUE);
			values[i] = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (spinvalue));
			gtk_table_attach_defaults (GTK_TABLE (table),
			                           spinvalue,
			                           i + 1, i + 2,
			                           1, 2);
		}
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			label = gtk_entry_get_text (GTK_ENTRY (input_symbol));
			if (g_ascii_isalpha (label[0]))
				{
					for (i = 0; i < gui->work->DIM; i++)
						matrix[i] = (int) gtk_adjustment_get_value (values[i]);
					ucon = ll_ucon_new (label, matrix, gui->work->DIM);
					ll_ucon_list_append_ucon (&gui->work->logic->ucons, ucon);
					add_ucon_button (gui, label);
				}
		}
	gtk_widget_destroy (GTK_WIDGET (dialog));

	return 0;
}


gint
dialog_ucon_edit (MaTestGUI *gui, char symbol[])
{
	GtkDialog       *dialog;
	GtkWidget       *content,
	                *table,
	                *label_symb,
	                *label_col,
	                *b_delete,
	                **spinvalues;
	LlUCon          *ucon;
	gint            i;

	ucon = ll_ucon_list_get_ucon_by_symbol (gui->work->logic->ucons, symbol);
	spinvalues = (GtkWidget**) g_new (GtkWidget, gui->work->DIM);

	dialog = GTK_DIALOG (gtk_dialog_new_with_buttons (g_strdup_printf (_("Editando %s"), symbol),
	                                                  GTK_WINDOW (gui->window),
	                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                                  GTK_STOCK_OK,
	                                                  GTK_RESPONSE_ACCEPT,
	                                                  GTK_STOCK_CANCEL,
	                                                  GTK_RESPONSE_REJECT,
	                                                  NULL));
	gtk_dialog_set_has_separator (dialog, FALSE);

	content = gtk_dialog_get_content_area (dialog);
	table = gtk_table_new (2, gui->work->DIM + 1, TRUE);
	gtk_container_add (GTK_CONTAINER (content), table);
	label_symb = gtk_label_new (g_strdup_printf ("%s", symbol));
	gtk_label_set_markup (GTK_LABEL (label_symb),
	                      g_strdup_printf ("<b>%s</b>", symbol));
	gtk_table_attach_defaults (GTK_TABLE (table), label_symb, 0, 1, 0, 1);


	for (i = 0; i < gui->work->DIM; i++)
		{
			label_col = gtk_label_new (g_strdup_printf ("%x", i));
			gtk_table_attach_defaults (GTK_TABLE (table), label_col,
                                 i + 1, i + 2,
                                 0, 1);

			spinvalues[i] = gtk_spin_button_new_with_range (0, gui->work->MAXV, 1);
			gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalues[i]), 0);
			gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalues[i]), TRUE);
			gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinvalues[i]),
					                       ucon->matrix[i]);
			gtk_table_attach_defaults (GTK_TABLE (table),
			                           spinvalues[i],
			                           i + 1, i + 2,
			                           1, 2);
		}
	/*
	b_delete = gtk_button_new_with_label (_("Borrar conectiva"));
	g_signal_connect (G_OBJECT (b_delete), "clicked",
		                G_CALLBACK (on_b_ucon_del_clicked), gui);
	gtk_container_add (GTK_CONTAINER (vbox), b_delete);
	*/
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		for (i = 0; i < gui->work->DIM; i++)
			ucon->matrix[i] = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spinvalues[i]));
	gtk_widget_destroy (GTK_WIDGET (dialog));

	return 0;
}


gint
dialog_bcon_new (MaTestGUI *gui)
{
	GtkDialog       *dialog;
	GtkWidget       *content,
	                *table,
	                *input_symbol,
	                *label_row, *label_col,
	                *spinvalue;
	GtkAdjustment   ***values;
	LlBCon          *bcon;
	gint            i, j;
	int             **matrix;
	const gchar     *label;

	values = (GtkAdjustment***) g_new (GtkAdjustment*, gui->work->DIM);
	for (i = 0; i < gui->work->DIM; i++)
		values[i] = (GtkAdjustment**) g_new (GtkAdjustment, gui->work->DIM);
	matrix = g_new (int*, gui->work->DIM);
	for (i = 0; i < gui->work->DIM; i++)
		matrix[i] = g_new0 (int, gui->work->DIM);

	dialog = GTK_DIALOG (gtk_dialog_new_with_buttons (_("Nueva conectiva binaria"),
	                                                  GTK_WINDOW (gui->window),
	                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                                  GTK_STOCK_OK,
	                                                  GTK_RESPONSE_ACCEPT,
	                                                  GTK_STOCK_CANCEL,
	                                                  GTK_RESPONSE_REJECT,
	                                                  NULL));
	gtk_dialog_set_has_separator (dialog, FALSE);

	content = gtk_dialog_get_content_area (dialog);
	table = gtk_table_new (gui->work->DIM + 1, gui->work->DIM + 1, FALSE);
	gtk_container_add (GTK_CONTAINER (content), table);
	input_symbol = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (input_symbol), 1);
	gtk_entry_set_width_chars (GTK_ENTRY (input_symbol), 1);
	gtk_entry_set_alignment (GTK_ENTRY (input_symbol), 1);
	gtk_table_attach_defaults (GTK_TABLE (table), input_symbol, 0, 1, 0, 1);

	for (i = 0; i < gui->work->DIM; i++) {
		label_row = gtk_label_new (g_strdup_printf ("%x", i));
		gtk_table_attach_defaults (GTK_TABLE (table), label_row,
		                           0, 1,
		                           i + 1, i + 2);
		label_col = gtk_label_new (g_strdup_printf ("%x", i));
		gtk_table_attach_defaults (GTK_TABLE (table), label_col,
			                         i + 1, i + 2,
			                         0, 1);
		for (j = 0; j < gui->work->DIM; j++)
			{
				spinvalue = gtk_spin_button_new_with_range (0, gui->work->MAXV, 1);
				gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalue), 0);
				gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalue), TRUE);
				values[i][j] = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (spinvalue));
				gtk_table_attach_defaults (GTK_TABLE (table),
					                         spinvalue,
				                           j + 1, j + 2,
			  		                       i + 1, i + 2);
			}
	}
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			label = gtk_entry_get_text (GTK_ENTRY (input_symbol));
			if (g_ascii_isalpha (label[0]))
				{
					for (i = 0; i < gui->work->DIM; i++)
						for (j = 0; j < gui->work->DIM; j++)
							matrix[i][j] = (int) gtk_adjustment_get_value (values[i][j]);
					bcon = ll_bcon_new (g_ascii_strup (label, -1), matrix, gui->work->DIM);
					ll_bcon_list_append_bcon (&gui->work->logic->bcons, bcon);
					add_bcon_button (gui, label);
				}
		}
	gtk_widget_destroy (GTK_WIDGET (dialog));

	return 0;
}


gint
dialog_bcon_edit (MaTestGUI *gui, char symbol[])
{
	GtkDialog  *dialog;
	GtkWidget  *content,
	           *table,
	           *label_symb,
	           *label_row, *label_col,
	           ***spinvalues;
	LlBCon     *bcon;
	gint       i, j;

	bcon = ll_bcon_list_get_bcon_by_symbol (gui->work->logic->bcons, symbol);
	spinvalues = (GtkWidget***) g_new (GtkWidget*, gui->work->DIM);
	for (i = 0; i < gui->work->DIM; i++)
		spinvalues[i] = (GtkWidget**) g_new (GtkWidget, gui->work->DIM);

	dialog = GTK_DIALOG (gtk_dialog_new_with_buttons (g_strdup_printf (_("Editando %s"), symbol),
	                                                  GTK_WINDOW (gui->window),
	                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                                  GTK_STOCK_OK,
	                                                  GTK_RESPONSE_ACCEPT,
	                                                  GTK_STOCK_CANCEL,
	                                                  GTK_RESPONSE_REJECT,
	                                                  NULL));
	gtk_dialog_set_has_separator (dialog, FALSE);

	content = gtk_dialog_get_content_area (dialog);
	table = gtk_table_new (gui->work->DIM + 1, gui->work->DIM + 1, TRUE);
	gtk_container_add (GTK_CONTAINER (content), table);
	label_symb = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (label_symb),
	                      g_strdup_printf ("<b>%s</b>", symbol));
	gtk_table_attach_defaults (GTK_TABLE (table), label_symb, 0, 1, 0, 1);

	for (i = 0; i < gui->work->DIM; i++) {
		label_row = gtk_label_new (g_strdup_printf ("%x", i));
		gtk_table_attach_defaults (GTK_TABLE (table), label_row,
		                           0, 1,
		                           i + 1, i + 2);
		label_col = gtk_label_new (g_strdup_printf ("%x", i));
		gtk_table_attach_defaults (GTK_TABLE (table), label_col,
			                         i + 1, i + 2,
			                         0, 1);
		for (j = 0; j < gui->work->DIM; j++) {
			spinvalues[i][j] = gtk_spin_button_new_with_range (0, gui->work->MAXV, 1);
			gtk_spin_button_set_digits (GTK_SPIN_BUTTON (spinvalues[i][j]), 0);
			gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinvalues[i][j]), TRUE);
			gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinvalues[i][j]),
					                       bcon->matrix[i][j]);
			gtk_table_attach_defaults (GTK_TABLE (table),
			                           spinvalues[i][j],
			                           j + 1, j + 2,
				                         i + 1, i + 2);
		}
	}
	gtk_widget_show_all (content);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			for (i = 0; i < gui->work->DIM; i++)
				for (j = 0; j < gui->work->DIM; j++)
					bcon->matrix[i][j] = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spinvalues[i][j]));
		}
	gtk_widget_destroy (GTK_WIDGET (dialog));

	return 0;
}


gchar*
get_current_evaluating_formula_pn (char formula[], LlLogic* logic)
{
	int        i;
	char       symbol[2];
	GString    *text;

	text = g_string_new ("");

	for (i = 0; i < (int) strlen (formula); i++)
		{
			sprintf (symbol, "%c", formula[i]);
			if (ll_symbol_pn_get_type (formula[i], logic) == LL_SYMBOL_VAR)
				g_string_append_printf (text, "%x", ll_var_get_value (ll_var_list_get_var_by_symbol (logic->vars, symbol)));
			else
				g_string_append_printf (text, "%c", formula[i]);
		}

	return text->str;
}


gchar*
evaluation_gui (MaTestGUI *gui)
{
	int           i,
	              all = 0,
	              desig = 0;
	LlVar         *var;
	static gchar  *straux;
	GString       *text;

	text = g_string_new ("");

	if (!gui->work->wff)
		{
			g_string_append_printf (text, "Ninguna fórmula para evaluar,\nintroduzca una.");
			return text->str;
		}

	/* Imprime una cabecera con las variables en uso y la fórmula */
	straux = print_used_variables (gui->work->logic);
	g_string_append_printf (text, "%s", straux);
	g_free (straux);
	g_string_append_printf (text, "  %s\n", gui->work->formula_pn);

	/*
	 * El algoritmo contador
	 */
	void action (Work* work, int *all, int *desig)
		{
			int i;

			i =
			/* Cuenta cada evaluación */
			(*all)++;
			/* Imprime una evaluación dependiendo del tipo de evaluación seleccionado
			 * y cuenta los valores designados */
			if (i >= gui->work->MDV)
				{
					(*desig)++;
					if (gui->work->evaluation_style == ALL || gui->work->evaluation_style == DESIGNATED)
						{
							straux = print_varlist_values (gui->work->logic->vars);
							g_string_append_printf (text, "%s", straux);
							g_string_append_printf (text, " *%x\n", ll_wff_get_value (gui->work->wff, gui->work->logic));
							g_free (straux);
						}
				 }
			else
				{
					if (gui->work->evaluation_style == ALL || gui->work->evaluation_style == NOT_DESIGNATED)
						{
							straux = print_varlist_values (gui->work->logic->vars);
							g_string_append_printf (text, "%s", straux);
							g_string_append_printf (text, "  %x\n", ll_wff_get_value (gui->work->wff, gui->work->logic));
							g_free (straux);
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
			if (ll_var_get_value (var) < gui->work->DIM - 1)
				{
					ll_var_set_value (var, ll_var_get_value (var) + 1);
					var = gui->work->logic->vars;
					action (gui->work, &all, &desig);
				}
			else
				{
					ll_var_set_value (var, 0);
					var = var->next;
				}
		}
	while (var);

	/* Imprime las estadísticas */
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
