/*****************************************************************************\
#                                                                             #
#                             ---   logic.h   ---                             #
#             Header for general logic data types and functions               #
#       Copyright (C) 2008, César González Gutiérrez <ceguel@gmail.com>       #
#                                                                             #
#    This file is free software: you can redistribute it and/or modify        #
#    it under the terms of the GNU General Public License as published by     #
#    the Free Software Foundation, either version 3 of the License, or        #
#    (at your option) any later version.                                      #
#                                                                             #
#    This file is distributed in the hope that it will be useful,             #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#    GNU General Public License for more details.                             #
#                                                                             #
#    You should have received a copy of the GNU General Public License        #
#    along with this file.  If not, see <http://www.gnu.org/licenses/>.       #
#                                                                             #
#                                                                             #
\*****************************************************************************/


#ifndef _LOGICS_H_
#define _LOGICS_H_

/*
***  Data types definition
*/

/* Symbol types and Atom types for well formed formulas */
typedef enum _logic_symbol_kind
  {
    VARIABLE,
    UNYCON,
    BINCON,
    OPENAUX,
    CLOSEAUX,
    NONE
  }
  LogicSymbKind;

typedef enum _logic_atom_kind
  {
    VAR,
    UCON,
    BCON
  }
  LogicAtomKind;


/* Variable node */
typedef struct _logic_var_node
  {
    char name;
    int value;
    struct _logic_var_node *next;
  }
  LogicVarType;

typedef LogicVarType *LogicVar;
typedef LogicVarType *LogicVarList;


/* Unary connective node */
typedef struct _logic_uny_con_node
  {
    char name;
    int *matrix;
    struct _logic_uny_con_node *next;
  }
  LogicUConType;

typedef LogicUConType *LogicUCon;
typedef LogicUConType *LogicUConList;


/* Binary connective node */
typedef struct _logic_bin_con_node
  {
    char name;
    int **matrix;
    struct _logic_bin_con_node *next;
  }
  LogicBConType;

typedef LogicBConType *LogicBCon;
typedef LogicBConType *LogicBConList;


/* Well Formed Formula node */
typedef struct _logic_atom_node
  {
    LogicAtomKind kind;
    char name;
    int *value;
    struct _logic_atom_node *prearg;
    struct _logic_atom_node *postarg;
  }
  LogicAtomType;

typedef LogicAtomType *LogicAtom;
typedef LogicAtomType *LogicWFF;


/* Struct with all data that defines one logic */
typedef struct _logic
  {
    int dim;
    int mdv;
    LogicVarList Vars;
    LogicUConList UCons;
    LogicBConList BCons;
  }
  LogicType;

typedef LogicType *Logic;


/*
***  Function prototypes
*/

/* Connective related function prototypes.
   Functions present in file connectives.c */
LogicUCon search_UCon (LogicUConList list, char name);
LogicBCon search_BCon (LogicBConList list, char name);
int add_UCon (LogicUConList *list, char name, int *mtx, int dimension);
int add_BCon (LogicBConList *list, char name, int **mtx, int dimension);
void del_connective (Logic logic, char name);


/* Variable related function prototypes.
   Functions present in file variables.c */
bool is_empty_var_list (LogicVarList list);
void del_var_list (LogicVarList *list);
LogicVar search_var (LogicVarList list, char variable);
void add_var (LogicVarList *list, char variable);
void set_var_value (LogicVar var, int value);
int var_value (LogicVar var);


/* Well formed formulas in polish notation related function prototypes.
   Functions present in file wffs_pn.c */
LogicSymbKind symbol_kind_pn (char symbol, Logic logic);
bool check_string (char formula[]);
bool is_wff_pn (char formula[], Logic logic);


#endif

