/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * callbacks.h
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


#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <gtk/gtk.h>
#include <glade/glade.h>
#include "MaTest.h"

#define BUILDER_UI_FILE "matest.ui"

/* store the widgets which may need to be accessed in a typedef struct */
typedef struct
	{
		GtkWidget      *window;
		GtkWidget      *statusbar;
		GtkWidget      *text_view;
		GtkWidget      *progressbar;
		GtkTextBuffer  *textbuffer;
		GtkLabel       *label_dimmension;
		GtkLabel       *label_ucons;
		GtkLabel       *label_bcons;
		GtkLabel       *label_formula;
		Work           work;
	}
	MaTestGUI;


gboolean init_app (MaTestGUI *gui);
void on_window_destroy (GtkObject *object, MaTestGUI *gui);
void error_message (const gchar *message);
void on_m_file_quit_activate (GtkObject *object, MaTestGUI *gui);
void on_b_show_matrices_activate (GtkObject *object, MaTestGUI *gui);


#endif
