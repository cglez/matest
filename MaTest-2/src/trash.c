/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */

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
	straux = sprint_varlist (gui->work->logic);
	g_string_append_printf (text, "%s", straux);
	g_free (straux);
	g_string_append_printf (text, " %s\n", gui->work->formula_pn);

	/*
	 * El algoritmo contador
	 */
	void action (Work* work, int *all, int *desig)
		{
			int i;

			i = ll_wff_get_value (work->wff, work->logic);

			/* Cuenta cada evaluación */
			(*all)++;
			/* Imprime una evaluación dependiendo del tipo de evaluación seleccionado
			 * y cuenta los valores designados */
			if (i >= gui->work->MDV)
				{
					(*desig)++;
					if (gui->work->evaluation_style == ALL || gui->work->evaluation_style == DESIGNATED)
						{
							straux = (gchar*) print_varlist_values (gui->work->logic->vars);
							g_string_append_printf (text, "%s", straux);
							g_free (straux);
							straux = (gchar*) sprint_formula_valuation_pn (gui->work->wff, gui->work->logic);
							g_string_append_printf (text, " %s", straux);
							g_free (straux);
							g_string_append_printf (text, "  *%x\n", ll_wff_get_value (gui->work->wff, gui->work->logic));
						}
				 }
			else
				{
					if (gui->work->evaluation_style == ALL || gui->work->evaluation_style == NOT_DESIGNATED)
						{
							straux = print_varlist_values (gui->work->logic->vars);
							g_string_append_printf (text, "%s", straux);
							g_free (straux);
							straux = sprint_formula_valuation_pn (gui->work->wff, gui->work->logic);
							g_string_append_printf (text, " %s", straux);
							g_free (straux);
							g_string_append_printf (text, "   %x\n", ll_wff_get_value (gui->work->wff, gui->work->logic));
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


/*
 *
 */
void
print_current_variables_values (LlLogic *logic)
{
	LlVar *var = logic->vars;

	while (var)
		{
			printf ("%i ", ll_var_get_value (var));
			var = var->next;
		}
}


void
print_evaluation_header (Work *work)
{
	LlVar  *var = work->logic->vars;

	putchar (' ');

	while (var)
		{
			printf ("%s ", var->symbol);
			var = var->next;
		}
	printf ("  %s\n", work->formula_pn);

	var = work->logic->vars;
	while (var)
		{
			printf ("--");
			var = var->next;
		}
	printf ("   -\n");
}


/*
 * Acción individual del algoritmo contador
 */
void
action (FILE *output, Work* work, int *all, int *desig)
{
	int val;

	val = ll_wff_get_value (work->wff, work->logic);
	/* Cuenta cada evaluación */
	(*all)++;
	/* Imprime una evaluación dependiendo del tipo de evaluación seleccionado
	 * y cuenta los valores designados */
	if (val >= work->MDV)
		{
			(*desig)++;
			if (work->evaluation_style == ALL || work->evaluation_style == DESIGNATED)
				{
					 fprintf (output, " ");
					 //sprint_formula_valuation_pn (output, work->formula_pn, work->logic);
					 print_current_variables_values (work->logic);
					 fprintf (output, " *%i\n", val);
				}
		 }
	else
		{
			if (work->evaluation_style == ALL || work->evaluation_style == NOT_DESIGNATED)
				{
					fprintf (output, " ");
					//sprint_formula_valuation_pn (output, work->formula_pn, work->logic);
					print_current_variables_values (work->logic);
					fprintf (output, "  %i\n", val);
				}
		}
}


void
evaluation (FILE *output, Work* work)
{
	int i, all = 0, desig = 0;
	LlVar *var;

	/* Imprime una cabecera con la fórmula en notación polaca */
	print_evaluation_header (work);
	/*
	for (i = 0; i < ll_var_list_length (work->logic->vars); i++)
		fprintf (output, "  ");
	fprintf (output, "   %s\n ", work->formula_pn);
	*//*
	for (i = 0; i < (int) strlen (work->formula_pn); i++)
		fprintf (output, "-");
	fprintf (output, "\n");
	*/
	//sprint_varlist (work->logic);

	/* Condición inicial: todos los valores inicializados a 0 */
	var = work->logic->vars;
	while (var)
		{
			var->value = 0;
			var = var->next;
		}
	/* Primera acción con la primera de las condiciones */
	action (output, work, &all, &desig);

	/* El contador */
	var = work->logic->vars;
	do
		{
			if (ll_var_get_value (var) < work->MAXV)
				{
					ll_var_set_value (var, ll_var_get_value (var) + 1);
					var = work->logic->vars;
					action (output, work, &all, &desig);
				}
			else
				{
					ll_var_set_value (var, 0);
					var = var->next;
				}
		}
	while (var);

	/* Imprime las estadísticas */
	fprintf (output, "\n %i posibilidades evaluadas.\n", all);
	if (work->evaluation_style == ALL || work->evaluation_style == DESIGNATED)
		fprintf (output, " %i valores designados.\n", desig);
	else
		fprintf (output, " %i valores no designados.\n", all - desig);
	if (desig == all)
		fprintf (output, " TAUTOLOGÍA.\n");
	else if (desig == 0)
		fprintf (output, " CONTRADICCIÓN.\n");
	else
		fprintf (output, " Las matrices dadas FALSAN la fórmula.\n");
}


gchar*
sprint_formula_valuation_pn (LlWFF wff, LlLogic* logic)
{
	gchar    *buffer,
	         *straux;

	buffer = g_new0 (gchar, 1);

	if (!wff) {
		return buffer;
	}
	buffer = g_strdup_printf ("%x", ll_wff_get_value (wff, logic));

	straux = sprint_formula_valuation_pn (wff->prearg, logic);
	buffer = g_strconcat (buffer, straux, NULL);
	g_free (straux);

	straux = sprint_formula_valuation_pn (wff->postarg, logic);
	buffer = g_strconcat (buffer, straux, NULL);
	g_free (straux);

	return buffer;
}


/**
 * Añade una variable proposicional a una lista si no existe ya una con ese
 * nombre, y la sitúa por orden alfabético, con lo que la lista queda ordenada.
 */
void
logics_var_list_add_var (LogicsVarList *list, char variable)
{
	LogicsVar new, aux;
	
	/* Descarta los elementos que ya están en la lista */
	if (logics_var_list_get_var_by_symbol (*list, variable))
		return;
	else
		{
			/* Crea el nuevo nodo */
			new = (LogicsVar) malloc (sizeof (LogicsVar));
			new->name = variable;
			new->value = 0;
			
			/* Si la lista está vacía o la letra es previa a la primera de la lista,
			 * el nuevo nodo es ahora el primer elemento */
			if ((*list == NULL) || (variable < (*list)->name))
				{
					/* Añade la lista después del nodo */
					new->next = *list;
					/* Ahora la lista empieza con este nodo */
					*list = new;
				}
			/* Sino, busca una variable de letra posterior para situarla antes o al
			 * final de la lista. */
			else
				{
					aux = *list;
					while (aux->next && aux->name < variable)
						aux = aux->next;
					
					new->next = aux->next;
					aux->next = new;
				}
		}
}
