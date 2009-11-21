/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * MaTest.c
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


/**
 * @file MaTest.c
 *
 * Este archivo contiene la función principal "main" y el manejador de opciones
 * interactivo.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#include "MaTest.h"
#include "callbacks.h"


/**
 * Fución principal main. Define los elementos principales y pasa al modo
 * interactivo.
 */
int
main (int argc, char *argv[])
{
	int c, index, initdim = 0, initmdv = 0;
	Work work;

	opterr = 0;
	
	work = (Work) malloc (sizeof (workType));
	work->logic = (Logic) malloc (sizeof (LogicType));
	work->DIM = 0;
	work->formula_pn[0] = 0;
	work->wff = NULL;
	
	
	while ((c = getopt (argc, argv, "d:m:f:s:hv")) != -1)
		switch (c)
			{
				case 'h':
					menu_usage();
					return 0;
				case 'v':
					menu_version();
					return 0;
				case 'd':
					initdim = atoi (optarg);
					break;
				case 'm':
					initmdv = atoi (optarg);
					break;
				case 'f':
					/* if (is_wff_pn (optarg, work->logic)) */
					strcpy (work->formula_pn, optarg);
					break;
				case 's':
					if (strcmp (optarg, "a"))  /* || strcmp (optarg, "all") == 0) */
						work->eval_values = ALL;
					else if (strcmp (optarg, "d") ) /* || strcmp (optarg, "designated") == 0) */
						work->eval_values = DESIGNATED;
					else if (strcmp (optarg, "n")) /* || strcmp (optarg, "not-designated") == 0) */
						work->eval_values = NOTDESIGNATED;
					break;
				case '?':
					if (optopt == c)
						fprintf (stderr, "Option -%c requires an argument.\n", optopt);
					else if (isprint (optopt))
						fprintf (stderr, "Unknown option `-%c'.\n", optopt);
					else
						fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
					return 1;
			}
	
	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);
	
	if (initdim >= 2)
		work->DIM = initdim;
	if (initmdv >= 0 && initmdv <= work->DIM)
		work->MDV = initmdv;
	
	mode_gui (argc, argv, work);
	//mode_text (work);
	return (0);
}
