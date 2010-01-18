/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * callbacks.c
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

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


void
on_m_file_quit_activate (GtkObject *object, MaTestGUI *gui)
{
	gtk_main_quit ();
}


void
on_b_evaluate_clicked (GtkObject *object, MaTestGUI *gui)
{
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER (gui->textbuffer),
	                          (gchar *) evaluation_gui (gui), -1);
}


void
on_b_print_matrices_clicked (GtkObject *object, MaTestGUI *gui)
{
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER (gui->textbuffer),
	                          print_matrices_gui (gui->work->logic), -1);
}


void
on_b_new_formula_clicked (GtkObject *object, MaTestGUI *gui)
{
	if (logics_formula_is_wff_pn ((char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)), gui->work->logic))
		{
			strcpy (gui->work->formula_pn, (char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)));
			
			if (gui->work->logic->vars)
				logics_var_list_free (&gui->work->logic->vars);
			register_vars (gui->work->logic, gui->work->formula_pn);
			
			if (gui->work->wff)
				del_wff (&gui->work->wff);
			logics_wff_parse_formula_pn (&gui->work->wff, gui->work->formula_pn, gui->work->logic);
		}
	gtk_entry_set_text (GTK_ENTRY (gui->entry_formula), "");
	gtk_label_set_text (GTK_LABEL (gui->label_formula),
	                    g_strdup_printf ("Fórmula: %s", gui->work->formula_pn));
}


void
on_entry_formula_activate (GtkObject *object, MaTestGUI *gui)
{
	if (logics_formula_is_wff_pn ((char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)), gui->work->logic))
		{
			strcpy (gui->work->formula_pn, (char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)));
			
			if (gui->work->logic->vars)
				logics_var_list_free (&gui->work->logic->vars);
			register_vars (gui->work->logic, gui->work->formula_pn);
			
			if (gui->work->wff)
				del_wff (&gui->work->wff);
			logics_wff_parse_formula_pn (&gui->work->wff, gui->work->formula_pn, gui->work->logic);
		}
	gtk_entry_set_text (GTK_ENTRY (gui->entry_formula), "");
	gtk_label_set_text (GTK_LABEL (gui->label_formula),
	                    g_strdup_printf ("Fórmula: %s", gui->work->formula_pn));
}


void
on_b_ucon_clicked (GtkObject *object, MaTestGUI *gui)
{
	gchar *label;
	char  symb;

	label = gtk_button_get_label (GTK_BUTTON (object));
	symb = (char) label[0];
	dialog_ucon_edit (gui, symb);
}


void
on_b_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{
	gchar *label;
	char symb;
	
	label = gtk_button_get_label (GTK_BUTTON (object));
	symb = (char) label[0];
	dialog_bcon_edit (gui, symb);
}


void
on_b_add_ucon_clicked (GtkObject *object, MaTestGUI *gui)
{
	dialog_ucon_new (gui);
}


void
on_b_ucon_del_clicked (GtkObject *object, MaTestGUI *gui)
{
	
}


void
on_b_add_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{
	dialog_bcon_new (gui);
}


void
on_b_del_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{
	
}


void
on_spin_dimension_value_changed (GtkObject *object, MaTestGUI *gui)
{
	logics_ucon_list_free (&gui->work->logic->ucons);
	logics_bcon_list_free (&gui->work->logic->bcons, gui->work->DIM);
	gui->work->logic->dimension = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (gui->spin_dimension));
	logics_logic_set_default_ucons_lukasiewicz (gui->work->logic);
	logics_logic_set_default_bcons_lukasiewicz (gui->work->logic);
	gtk_spin_button_set_range (GTK_SPIN_BUTTON (gui->spin_mdv),
	                           1, gui->work->MAXV);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (gui->spin_mdv), gui->work->MAXV);
}


void
on_spin_mdv_value_changed (GtkObject *object, MaTestGUI *gui)
{
	gui->work->logic->mdv = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (gui->spin_mdv));
}


void
on_m_file_save_activate (GtkObject *object, MaTestGUI *gui)
{
	write_matrices_gui (gui);
}


void
on_m_view_all_toggled (GtkObject *object, MaTestGUI *gui)
{
	gui->work->evaluation_style = ALL;
}

void
on_m_view_desig_toggled (GtkObject *object, MaTestGUI *gui)
{
	gui->work->evaluation_style = DESIGNATED;
}

void
on_m_view_notdesig_toggled (GtkObject *object, MaTestGUI *gui)
{
	gui->work->evaluation_style = NOT_DESIGNATED;
}
