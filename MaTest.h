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


#ifndef MATEST_H
  #define MATEST_H


/*
***  Preprocessor declarations 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define VERSION "1.0 beta"
#define MAX_FORMULA_LENGHT BUFSIZ


/*
***  Data definition
*/

char answer[BUFSIZ];                    /* keyword entry                           */
/* int dim, mdv; */                     /* dimmension and minimun designated value */
/* char opt; */                         /* selected option                         */
char the_formula[MAX_FORMULA_LENGHT];   /* character string for the formula        */


/* Symbol types enumeration */
typedef enum
  {
    VAR,
    UCON,
    DCON,
    NONE
  }
  SymbolType;


/* Evaluation types enumeration */
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
  FIFO;
*/

/* Variable node */
typedef struct _node_var
  {
    char name;
    int value;
    struct _node_var *next;
  }
  typeVar;

typedef typeVar *variable;
typedef typeVar *varList;


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


/* Dyadic connective node */
typedef struct _node_dya_con
  {
    char name;
    int **matrix;
    struct _node_dya_con *next;
  }
  dyaConType;

typedef dyaConType *dyaCon;
typedef dyaConType *dyaConList;


/* Global struct */
typedef struct _logic
  {
    int dimmension, mdv;
    char formula[MAX_FORMULA_LENGHT];
    varList vars;
    unyConList unyconns;
    dyaConList dyaconns;
  }
  logicType;
  
typedef logicType *Logic;
Logic the_logic;


/*
***  Function prototypes
*/

/* Connective related functions prototypes */
bool is_unary_connective (char ch, unyConList *list);
bool is_dyadic_connective (char ch, dyaConList *list);
int add_unary_connective (char ch, unyConList *list, int *mtx);
int add_dyadic_connective (char ch, dyaConList *list, int **mtx);
void set_default_unycons (Logic logic);
void set_default_dyacons (Logic logic);
void print_uny_matrix (unyCon connective);
void print_dya_matrix (dyaCon connective);
int write_uny_matrix (unyCon connective, char filename[]);
int write_dya_matrix (dyaCon connective, char filename[]);

/* Variable related functions prototypes */
void add_var (varList *ls, char var);
void Borrar (varList *ls, char var);

int is_empty_list (varList ls);

void del_list (varList *);
void print_list (varList ls);
bool is_in_list (varList *ls, char var);


/* Polk well formed formula related functions prototypes */
bool check_string (char formul[]);
bool is_wff_pk (char formul[]);


/* User related functions prototypes */
char readin (char *str, char *pattern);
void clear_scr (void);
void menu_header (void);
void menu_info (void);
void menu_options (void);
void menu_dimmension (void);
void menu_index (void);


#endif

