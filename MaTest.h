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


/* Well Formed Formula nodes */
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
    int dimmension, mdv;
    VarList Vars;
    unyConList unyConns;
    binConList binConns;
  }
  logicType;
  
typedef logicType *Logic;


typedef struct _work
  {
    EvalType eval_values;
    char pol_formula[MAX_FORMULA_LENGHT];
    WFF wff;
    Logic logic;
  }
  workType;

typedef workType *Work;


/*
***  Function prototypes
*/

/* Connective related functions prototypes */
bool is_unary_connective (char con_name, unyConList *list);
bool is_binary_connective (char con_name, binConList *list);
int add_unary_connective (char con_name, int *mtx, unyConList *list, int dimmension);
int add_binary_connective (char con_name, int **mtx, binConList *list, int dimmension);
void set_default_uny_conns (Logic logic);
void set_default_bin_conns (Logic logic);
void print_uny_matrix (unyCon connective, int dimmension, int mdv);
void print_bin_matrix (binCon connective, int dimmension, int mdv);
int write_uny_matrix (unyCon connective, FILE *file, int dimmension);
int write_bin_matrix (binCon connective, FILE *file, int dimmension);


/* Variable related functions prototypes */
void add_var (char var_name, VarList *list);
bool is_empty_list (VarList list);
void del_list (VarList *list);
void print_list (VarList ls);
bool is_in_list (char var_name, VarList *list);


/* Polk well formed formula related functions prototypes */
bool check_string (char formul[]);
bool is_wff_pk (char formula[], Logic logic);


/* User related functions prototypes */
char readin (char str[], char pattern[]);
void clear_scr (void);
void menu_header (void);
void menu_info (Work work);
void menu_options (void);
void menu_dimmension (void);
void menu_index (Work work);


/* Evaluation related functions prototypes */
bool set_atom (WFF *tree, AtomType intype, char inname, int *invalue);
void parse_polish (char formula[], WFF *tree, Logic logic);
void print_wff (WFF f);
int eval_formula (WFF wff, Logic logic);

#endif
