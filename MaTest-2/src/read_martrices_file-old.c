/*
 * Lector de archivos con matrices.
 */
// FIXME Comentar el código
// FIXME Liberar la memoria de las conectivas usadas si algo falla
// TODO Notación estandar
// TODO Reconocer una directiva MDV
// TODO Reconocer comentarios
int
read_matricesfile (Work *work, char *filename)
{
	FILE       *file;
	LlLogic    *logic;
	LlUCon     *ucon;
	LlBCon     *bcon;
	char        con_symbol[SLEN + 1],
	            strsymb[SLEN + 1],
	            auxsymb[BUFSIZ/2],
	            line[BUFSIZ], *auxstr,
	            value[BUFSIZ/2];
	int         i, j, dim,
	            row = 0, col = 0,
	            vector[BUFSIZ/2],
	           **mtx = NULL;

	// Comprobamos que el fichero se puede abrir para lectura.
	file = fopen (filename, "r");
	if (file == NULL) {
		perror ("El archivo no existe o no tiene permisos para su lectura.\n");
		return 1;
	}

	logic = g_new0 (LlLogic, 1);

	while (!feof (file)) {
		while (fgets (line, BUFSIZ, file)) {
			if (sscanf (line, "%s", auxsymb)) {
				if (auxsymb[0] == '#') {
					printf ("Se ha encontrado un comentario. Pasamos a la siguiente linea.\n");
					break;
				}
				if (strlen (auxsymb) == 0) {
					if (mtx) {
						if (mtx[1][0] == -1 && con_symbol[0] != '\0') {
							printf ("* Guardamos una conectiva unaria.\n");
							//sprintf (strsymb, "%c", con_symbol);
							ucon = ll_ucon_new (auxsymb, auxsymb, mtx[0], logic->dimension);
							logic->ucons = ll_ucon_list_append (logic->ucons, ucon);
							con_symbol[0] = '\0';
							row = 0;
							for (i = 0; i < logic->dimension; i++) {
								free (mtx[i]);
							}
							free (mtx);
							mtx = NULL;
						}
						else if (con_symbol[0] != '\0')	{
							printf ("** Guardamos una conectiva binaria.\n");
							//sprintf (strsymb, "%c", con_symbol);
							bcon = ll_bcon_new (auxsymb, auxsymb, mtx, logic->dimension);
							logic->bcons = ll_bcon_list_append (logic->bcons, bcon);
							con_symbol[0] = '\0';
							row = 0;
							for (i = 0; i < logic->dimension; i++) {
								free (mtx[i]);
							}
							free (mtx);
							mtx = NULL;
						}
						else {
							printf ("Hay una matriz sin símbolo asociado.\n");
							return 9;
						}
					}
					break;
				}
				else if (!isdigit (auxsymb[0])) {
					if (ll_ucon_list_get_ucon_by_symbol (logic->ucons, con_symbol) ||
					    ll_bcon_list_get_bcon_by_symbol (logic->bcons, con_symbol)) {
						fprintf (stderr, "Definición reiterada de la conectiva %s.\n", con_symbol);
						return 6;
					}
					else {
						printf ("No hay un símbolo de conectiva declarado. Se toma \'%s\'.\n", auxsymb);
						strcpy (con_symbol, auxsymb);
					}
					break;
				}
				/*
				else if (isalpha (auxsymb[0])) {
					if (con_symbol == 0) {
						printf ("No hay un símbolo de conectiva declarado.\n");
						con_symbol = auxchar;
					}
					else if (ll_ucon_list_get_ucon_by_symbol (logic->ucons, &con_symbol) ||
									 ll_bcon_list_get_bcon_by_symbol (logic->bcons, &con_symbol)) {
						fprintf (stderr, "Definición reiterada de la conectiva %c.\n", con_symbol);
						return 6;
					}
					break;
				}
				*/
			}
			if (sscanf (line, "%u", &i)) {
				col = 0;
				auxstr = line;
				while (sscanf (auxstr, "%u", &vector[col]) != EOF) {
					printf ("Se ha encontrado el valor \"%i\".\n", vector[col]);
					sprintf (value, "%u", vector[col]);
					auxstr = strstr (auxstr, value);
					auxstr = auxstr + strlen (value);
					col++;
				}
				for (i = 0; i < col; i++)
					printf ("vector[%i]:%i ", i, vector[i]);
				printf ("Guardado.\n");
				dim = col;
				printf ("Comprobamos dimension.\n");
				if (logic->dimension == 0) {
					logic->dimension = dim;
					printf ("Definida dimension = %i.\n", logic->dimension);
				}
				else if (logic->dimension != dim) {
					perror ("El número de elementos de una fila es distinto al esperado.\n");
					return 3;
				}
				printf ("Declaramos la matriz si no existía.\n");
				if (!mtx) {
					mtx = (int**) calloc (logic->dimension, sizeof (int*));
					for (i = 0; i < dim; i++) {
						mtx[i] = (int*) calloc (logic->dimension, sizeof (int));
					}
					mtx[1][0] = -1;
					printf ("Declarada.\n");
				}
				else {
					printf ("Ya estaba declarada.\n");
					if (row < logic->dimension)
						row++;
					else {
						printf ("Demasiadas filas para una matriz.\n");
						return 8;
					}
				}
				if (row > logic->dimension) {
					perror ("Una matriz tiene más filas de las esperadas.\n");
					return 4;
				}
				for (i = 0; i < logic->dimension; i++) {
					if (vector[i] > logic->dimension) {
						perror ("Hay elementos mayores que la dimensión de su matriz.\n");
						return 5;
					}
				}
				for (col = 0; col < logic->dimension; col++)
					mtx[row][col] = vector[col];
				for (i = 0; i < logic->dimension; i++)
					printf ("mtx[%i][%i]:%i ", row, i, mtx[row][i]);
				putchar ('\n');
			}
		}
	}
	/*
	if (!work->logic) {
		work->logic = g_new0 (LlLogic, 1);
	}
	*/
	MDV = MAXV;
	work->logic = logic;
	work->logic_modified = TRUE;
	return 0;
}
