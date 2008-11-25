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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define VERSION "1.0"
#define MAX_FORMULA_LENGHT BUFSIZ


/*
***  Data definition
*/

/* Symbol types and Atom types for well formed formulas */
typedef enum
  {
    VAR,
    UCON,
    BCON,
    NONE
  }
  SymbolType,
  AtomType;


/* Evaluation show types enumeration */
typedef enum
  {
    ALL,
    DESIGNATED,
    NOTDESIGNATED
  }
  EvalType;


/*
typedef struct
  {
    int position;
    int *vector;
  }
  LIFO;
*/


/* Variable node */
typedef struct _node_var
  {
    char name;
    int value;
    struct _node_var *next;
  }
  varType;

typedef varType *Var;
typedef varType *VarList;


/* Unary connective node */
typedef struct _node_uny_con
  {
    char name;
    int *matrix;
    struct _node_uny_con *next;
  }
  unyConType;

typedef unyConType *unyCon;
typedef unyConType *unyConList;


/* Binary connective node */
typedef struct _node_bin_con
  {
    char name;
    int **matrix;
    struct _node_bin_con *next;
  }
  binConType;

typedef binConType *binCon;
typedef binConType *binConList;


/* Well Formed Formula node */
typedef struct _node_atom
  {
    AtomType type;
    char name;
    int *value;
    struct _node_atom *arg1;
    struct _node_atom *arg2;
  }
  typeAtom;

typedef typeAtom *atom;
typedef typeAtom *WFF;


/* Struct with all data that defines one logic */
typedef struct _logic
  {
    int dimension, mdv;
    VarList Vars;
    unyConList unyConns;
    binConList binConns;
  }
  logicType;
  
typedef logicType *Logic;


/* General struct, contains all data needed for the evaluation */
typedef struct _work
  {
    EvalType eval_values;
    char formula_pn[MAX_FORMULA_LENGHT];
    WFF wff;
    Logic logic;
  }
  workType;

typedef workType *Work;


/*
***  Function prototypes
*/

/* Connective related function prototypes.
   Functions present in file connectives.c */
bool is_unary_connective (char con_name, unyConList *list);
bool is_binary_connective (char con_name, binConList *list);
unyCon search_uny_con (char con_name, unyConList list);
binCon search_bin_con (char con_name, binConList list);
int add_unary_connective (char con_name, int *mtx, unyConList *list, int dimension);
int add_binary_connective (char con_name, int **mtx, binConList *list, int dimension);
void add_custom_uny_conn (char con_name, unyConList *list, int dimension);
void add_custom_bin_conn (char con_name, binConList *list, int dimension);
void set_default_uny_conns (Logic logic);
void set_default_bin_conns (Logic logic);
void del_connective (char con_name, Logic logic);
void print_uny_matrix (unyCon connective, int dimension, int mdv);
void print_bin_matrix (binCon connective, int dimension, int mdv);
void show_matrices (Logic logic);
int write_uny_matrix (unyCon connective, FILE *file, int dimension);
int write_bin_matrix (binCon connective, FILE *file, int dimension);
void write_matrices (Logic logic, FILE *file);


/* Variable related function prototypes.
   Functions present in file variables.c */
bool is_empty_list (VarList list);
void del_var_list (VarList *list);
bool is_in_list (char var_name, VarList *list);
Var search_var (char var_name, VarList list);
Var last_var (VarList list);
void add_var (char var_name, VarList *list);
void register_vars (Work work);
int num_elements (VarList list);
void set_var_value (char var_name, int n, VarList list);
int get_var_value (char var_name, VarList list);



/* Well formed formulas in polish notation related function prototypes.
   Functions present in file wffs_pn.c */
SymbolType symbol_type (char symbol, Logic logic);
bool check_string (char formul[]);
bool is_wff_pk (char formula[], Logic logic);


/* Evaluation related function prototypes.
   Functions present in file evaluation.c */
bool set_atom (WFF *tree, AtomType intype, char inname, int *invalue);
void parse_polish (char formula[], WFF *tree, Logic logic);
void del_wff (WFF *wff);
int eval_formula (WFF wff, Logic logic);
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

