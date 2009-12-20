/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * callbacks.c
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
on_b_show_matrices_clicked (GtkObject *object, MaTestGUI *gui)
{
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER (gui->textbuffer),
	                          show_matrices_gui (gui->work->logic), -1);
}


void
on_b_new_formula_clicked (GtkObject *object, MaTestGUI *gui)
{
	if (is_wff_pn ((char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)), gui->work->logic))
		{
			strcpy (gui->work->formula_pn, (char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)));
			
			if (gui->work->logic->Vars)
				del_var_list (&gui->work->logic->Vars);
			register_vars (gui->work->logic, gui->work->formula_pn);
			
			if (gui->work->wff)
				del_wff (&gui->work->wff);
			parse_polish (&gui->work->wff, gui->work->formula_pn, gui->work->logic);
		}
	gtk_entry_set_text (GTK_ENTRY (gui->entry_formula), "");
	gtk_label_set_text (GTK_LABEL (gui->label_formula),
	                    g_strdup_printf ("Fórmula: %s", gui->work->formula_pn));
}


void
on_entry_formula_activate (GtkObject *object, MaTestGUI *gui)
{
	if (is_wff_pn ((char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)), gui->work->logic))
		{
			strcpy (gui->work->formula_pn, (char *) gtk_entry_get_text (GTK_ENTRY (gui->entry_formula)));
			
			if (gui->work->logic->Vars)
				del_var_list (&gui->work->logic->Vars);
			register_vars (gui->work->logic, gui->work->formula_pn);
			
			if (gui->work->wff)
				del_wff (&gui->work->wff);
			parse_polish (&gui->work->wff, gui->work->formula_pn, gui->work->logic);
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
	edit_ucon_gui (gui, symb);
}


void
on_b_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{
	gchar *label;
	char symb;
	
	label = gtk_button_get_label (GTK_BUTTON (object));
	symb = (char) label[0];
	edit_bcon_gui (gui, symb);
}


void
on_b_add_ucon_clicked (GtkObject *object, MaTestGUI *gui)
{
	add_ucon_gui (gui);
}


void
on_b_del_ucon_clicked (GtkObject *object, MaTestGUI *gui)
{
	
}


void
on_b_add_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{
	add_bcon_gui (gui);
}


void
on_b_del_bcon_clicked (GtkObject *object, MaTestGUI *gui)
{
	
}


void
on_spin_mdv_value_changed (GtkObject *object, MaTestGUI *gui)
{
	gui->work->logic->mdv = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (gui->spin_mdv));
}


/*
void
on_spin_value_changed (GtkObject *object, gpointer *value)
{
	value = (gint) gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (object));
}
*/


/*
void
on_b_new_formula_ok_clicked (GtkObject *object, NewFormulaGUI *formulagui)
{
	Work work = *formulagui->work;
		if (is_wff_pn ((char *) gtk_entry_get_text (formulagui->entry),
		               work->logic))
			{
				if (work->logic->Vars)
					del_var_list (&work->logic->Vars);
				register_vars (work->logic, work->formula_pn);
				
				if (work->wff)
					del_wff (&work->wff);
				parse_polish (&work->wff,
				              work->formula_pn,
				              work->logic);
			}
	gtk_widget_destroy (formulagui->win);
}
*/
