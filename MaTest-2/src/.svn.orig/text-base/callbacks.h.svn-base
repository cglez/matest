/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * callbacks.h
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


#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <gtk/gtk.h>
#include "MaTest.h"
#define UI_FILE "matest.ui"
#define ABOUT_DIALOG_FILE "about.ui"

void on_window_destroy (GtkObject *object, MaTestGUI *gui);
void on_m_file_quit_activate (GtkObject *object, MaTestGUI *gui);
void on_b_print_matrices_activate (GtkObject *object, MaTestGUI *gui);
void on_b_ucon_clicked (GtkObject *object, MaTestGUI *gui);
void on_b_ucon_del_clicked (GtkObject *object, MaTestGUI *gui);
void on_b_bcon_clicked (GtkObject *object, MaTestGUI *gui);
void on_b_new_formula_clicked (GtkObject *object, MaTestGUI *gui);
void on_m_view_all_toggled (GtkObject *object, MaTestGUI *gui);
void on_m_view_desig_toggled (GtkObject *object, MaTestGUI *gui);
void on_m_view_notdesig_toggled (GtkObject *object, MaTestGUI *gui);
void on_spin_value_changed (GtkObject *object, gpointer *value);
void destroy (GtkWidget *widget, gpointer data);

#endif /* __CALLBACKS_H__ */
