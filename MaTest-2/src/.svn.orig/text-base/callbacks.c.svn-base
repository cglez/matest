/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * callbacks.c
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include "callbacks.h"


void
destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit ();
}


void
on_window_destroy (GtkObject *object, MaTestGUI *gui)
{
	gtk_main_quit ();
}


/* Menú > Archivo > Salir : sale del programa. */
void
on_m_file_quit_activate (GtkObject *object, MaTestGUI *gui)
{
	gtk_main_quit ();
}


/* Botón evaluar */
void
on_b_evaluate_clicked (GtkObject *object, MaTestGUI *gui)
{
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER (gui->textbuffer),
	                          (gchar *) evaluation_gui (gui), -1);
}


/* Botón imprimir matrices */
void
on_b_print_matrices_clicked (GtkObject *object, MaTestGUI *gui)
{
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER (gui->textbuffer),
	                          (gchar *) (print_matrices_gui (gui->work->logic)), -1);
}


/* Botón nueva fórmula */
void
on_b_new_formula_clicked (GtkObject *object, MaTestGUI *gui)
{
	if (strcmp (gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)), "") &&
	    ll_formula_is_wff_pn ((char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)),
	                          gui->work->logic))
		{
			strcpy (gui->work->formula_pn, (char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)));

			if (gui->work->logic->vars)
				ll_var_list_free (&gui->work->logic->vars);
			ll_logic_add_formula_vars (gui->work->logic, gui->work->formula_pn);

			if (gui->work->wff)
				ll_wff_free (&gui->work->wff);
			ll_wff_parse_formula_pn (&gui->work->wff, gui->work->formula_pn, gui->work->logic);
		}
	gtk_entry_set_text (GTK_ENTRY (gui->entry_formula), "");
	gtk_label_set_text (GTK_LABEL (gui->label_formula),
	                    g_strdup_printf ("Fórmula: %s", gui->work->formula_pn));
}


/* Pulsar Enter en la entrada de la fórmula */
void
on_entry_formula_activate (GtkObject *object, MaTestGUI *gui)
{
	if (strcmp (gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)), "") &&
	    ll_formula_is_wff_pn ((char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)),
	                          gui->work->logic))
		{
			strcpy (gui->work->formula_pn, (char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)));

			if (gui->work->logic->vars)
				ll_var_list_free (&gui->work->logic->vars);
			ll_logic_add_formula_vars (gui->work->logic, gui->work->formula_pn);

			if (gui->work->wff)
				ll_wff_free (&gui->work->wff);
			ll_wff_parse_formula_pn (&gui->work->wff, gui->work->formula_pn, gui->work->logic);
		}
	gtk_entry_set_text (GTK_ENTRY (gui->entry_formula), "");
	gtk_label_set_text (GTK_LABEL (gui->label_formula),
	                    g_strdup_printf ("Fórmula: %s", gui->work->formula_pn));
}


/* Botón de una conectiva unaria cualquiera */
void
on_b_ucon_clicked (GtkObject *object, MaTestGUI *gui)
{
	const gchar *label;
	//char  symb;

	label = gtk_button_get_label (GTK_BUTTON (object));
	//symb = (char) label[0];
	dialog_ucon_edit (gui, label);
}


/* Botón de una conectiva binaria cualquiera */
void
on_b_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{
	const gchar *label;
	//char symb;

	label = gtk_button_get_label (GTK_BUTTON (object));
	//symb = (char) label[0];
	dialog_bcon_edit (gui, label);
}


/* Botón añadir conectiva unaria */
void
on_b_add_ucon_clicked (GtkObject *object, MaTestGUI *gui)
{
	dialog_ucon_new (gui);
}


void
on_b_ucon_del_clicked (GtkObject *object, MaTestGUI *gui)
{

}


/* Botón añadir conectiva binaria */
void
on_b_add_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{
	dialog_bcon_new (gui);
}


void
on_b_del_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{

}


/* Cambiar el valor de la dimensión */
void
on_spin_dimension_value_changed (GtkObject *object, MaTestGUI *gui)
{
	ll_ucon_list_free (&gui->work->logic->ucons);
	ll_bcon_list_free (&gui->work->logic->bcons, gui->work->DIM);

	gui->work->logic->dimension = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (gui->spin_dimension));

	ll_logic_set_default_ucons_lukasiewicz (gui->work->logic);
	ll_logic_set_default_bcons_lukasiewicz (gui->work->logic);

	gtk_spin_button_set_range (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           1, gui->work->MAXV);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui->spin_mdv), gui->work->MAXV);
}


/* Cambiar el valor del mínimo valor designado */
void
on_spin_mdv_value_changed (GtkObject *object, MaTestGUI *gui)
{
	gui->work->logic->mdv = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (gui->spin_mdv));
}


/* Menú > Archivo > Guardar : guarda las matrices en un archivo */
void
on_m_file_save_activate (GtkObject *object, MaTestGUI *gui)
{
	write_matrices_gui (gui);
}


/* Menú > Ver > Todos los valores : muestra todos los valores en la evaluación */
void
on_m_view_all_toggled (GtkObject *object, MaTestGUI *gui)
{
	gui->work->evaluation_style = ALL;
}


/* Menú > Ver > Valores designados : muestra los valores designados en la evaluación */
void
on_m_view_desig_toggled (GtkObject *object, MaTestGUI *gui)
{
	gui->work->evaluation_style = DESIGNATED;
}


/* Menú > Ver > Valores no designados : muestra los valores no designados en la evaluación */
void
on_m_view_notdesig_toggled (GtkObject *object, MaTestGUI *gui)
{
	gui->work->evaluation_style = NOT_DESIGNATED;
}


/*
 * Menú > Ayuda > Acerca de : muestra el diálogo con los créditos.
 */
void
on_m_help_about_activate (GtkObject *object, gpointer data)
{
	dialog_about ();
}
