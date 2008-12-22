/*****************************************************************************\
#                                                                             #
#                           ---   MaTest 1.0   ---                            #
#                     Matrix Tester for logical matrices.                     #
#       Copyright (C) 2008, César González Gutiérrez <ceguel@gmail.com>       #
#                                                                             #
#    This file is part of MaTest.                                             #
#                                                                             #
#    MaTest is free software: you can redistribute it and/or modify           #
#    it under the terms of the GNU General Public License as published by     #
#    the Free Software Foundation, either version 3 of the License, or        #
#    (at your option) any later version.                                      #
#                                                                             #
#    MaTest is distributed in the hope that it will be useful,                #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#    GNU General Public License for more details.                             #
#                                                                             #
#    You should have received a copy of the GNU General Public License        #
#    along with MaTest.  If not, see <http://www.gnu.org/licenses/>.          #
#                                                                             #
#                                                                             #
\*****************************************************************************/


/*                               MaTest.h
*
*    This files contains the common code for the whole program, that's it:
*    the preprocessor declarations, the data types definitions and the
*    prototypes of the functions.
*/

#ifndef _MATEST_H_
#define _MATEST_H_


/*
***  Preprocessor declarations
*/

#include <stdbool.h>
#include "logics.h"

#define VERSION "1.0a"
#define MAX_FORMULA_LENGHT BUFSIZ
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define DIM logic->dim
#define MDV logic->mdv


/*
***  Data definition
*/


/* Evaluation show types enumeration */
typedef enum _evaluation_style
  {
    ALL,
    DESIGNATED,
    NOTDESIGNATED
  }
  EvalStyle;


/*
typedef struct
  {
    int position;
    int *vector;
  }
  LIFO;
*/


/* General struct, contains all data needed by the evaluation */
typedef struct _work
  {
    Logic logic;
    char formula_pn[MAX_FORMULA_LENGHT];
    LogicWFF wff;
    EvalStyle eval_values;
  }
  workType;

typedef workType *Work;


/*
***  Function prototypes
*/

/* Connective related function prototypes.
   Functions present in file connectives.c */
void add_custom_UCon (LogicUConList *list, char name, int dimension);
void add_custom_BCon (Logic logic, char name);
void set_default_UCons (Logic logic);
void set_default_BCons (Logic logic);
void print_uny_matrix (LogicUCon connective, Logic logic);
void print_bin_matrix (LogicBCon connective, Logic logic);
void show_matrices (Logic logic);
int write_uny_matrix (FILE *file, LogicUCon connective, int dimension);
int write_bin_matrix (FILE *file, LogicBCon connective, int dimension);
void write_matrices (Logic logic, FILE *file);


/* Variable related function prototypes.
   Functions present in file variables.c */
bool is_empty_var_list (LogicVarList list);
void del_var_list (LogicVarList *list);
LogicVar search_var (LogicVarList list, char variable);
void add_var (LogicVarList *list, char variable);
void register_vars (Logic logic, char formula[]);
void set_var_value (LogicVar var, int value);
int var_value (LogicVar var);


/* Well formed formulas in polish notation related function prototypes.
   Functions present in file wffs_pn.c */
LogicSymbKind symbol_kind_pn (char symbol, Logic logic);
bool check_string (char formula[]);
bool is_wff_pn (char formula[], Logic logic);


/* Evaluation related function prototypes.
   Functions present in file evaluation.c */
bool set_atom (LogicWFF *tree, LogicSymbKind kind, char name, int *value);
void parse_polish (LogicWFF *tree, char formula[], Logic logic);
void del_wff (LogicWFF *wff);
int eval_formula (LogicWFF wff, Logic logic);
void print_eval_formula (char formula[], Logic logic);
void evaluation (Work work);


/* User related function prototypes.
   Functions present in file user.c */
char readin (char str[], char pattern[]);
void clear_scr (void);
void make_pause (void);
void menu_header (void);
void menu_info (Work work);
void menu_options (void);
void menu_init (void);
void menu_dimension (void);
void menu_about (void);
void menu_help (void);
void menu_index (Work work);


#endif

