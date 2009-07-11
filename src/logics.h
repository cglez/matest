/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * logics.h
 * Cabecera con tipos de datos y funciones para el manejo de lógicas en general.
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


#ifndef _LOGICS_H_
#define _LOGICS_H_


/*
***	Declaraciones del preprocesador
 */

/* Algunas abreviaciones útiles */
#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))
#define DIM logic->dim
#define MDV logic->mdv


/*
***	Tipos de datos
 */

/** Enumeración con los tipos de símbolos de una fórmula */
typedef enum _logic_symbol_kind
	{
		VARIABLE,  /**< Variable. */
		UNYCON,    /**< Conectiva unaria. */
		BINCON,    /**< Conectiva binaria. */
		OPENAUX,   /**< Símbolo auxiliar de apertura. */
		CLOSEAUX,  /**< Símbolo auxiliar de cierre. */
		NONE       /**< Tipo nulo, para el control de errores. */
	}
	LogicSymbKind;


/** Enumeración con los tipos de elementos de una fórmula bien formada */
typedef enum _logic_atom_kind
	{
		VAR,       /**< Variable. */
		UCON,      /**< Conectiva unaria. */
		BCON       /**< Conectiva binaria. */
	}
	LogicAtomKind;


/**
 * Nodo variable proposicional.
 * 
 * Entendemos aquí por variable proposicional a un tipo de dato que tiene
 * asignado un símbolo que la identifica, que puede tomar distintos valores
 * numéricos y que forma un grupo, una lista, con los otros elementos de la
 * misma clase que se hayan en uso. Este tipo de dato está implementado como una
 * lista enlazada simple.
 */
typedef struct _logic_var_node
	{
		char name;                     /**< Nombre de la variable o letra que la designa. */
		int value;                     /**< Valor de la variable. */
		struct _logic_var_node *next;  /**< Puntero al siguiente elemento de la lista. */
	}
	LogicVarType;

typedef LogicVarType *LogicVar;      /**< Declaración de una variable proposicional. */
typedef LogicVarType *LogicVarList;  /**< Declaración de una lista de variables. */


/**
 * Nodo conectiva unaria.
 * 
 * Una conectiva unaria aquí es una estructura que contiene el símbolo con el
 * que designamos a dicha conectiva y un vector de valores numéricos que define
 * la semántica del operador del siguiente modo: el valor que asigna el operador
 * a su argumento cuando éste vale n, es el valor del n-ésimo subíndice, esto
 * es, \f$ v(\Delta n) = \Delta_n \f$, siendo \f$ \Delta \f$ una conectiva
 * cualquiera y \f$ n \f$ un valor cualquiera.
 * 
 * Las conectivas así definidas están implementadas a su vez como una lista
 * enlazada simple para su mejor manejo.
 */
typedef struct _logic_uny_con_node
	{
		char name;     /**< Símbolo que representa la conectiva (una letra mayúscula en la notación polaca) */
		int *matrix;   /**< Matriz de la conectiva: un vector de enteros que define la semántica de la conectiva; contiene los valores que ésta asignará a una variable. */
		struct _logic_uny_con_node *next;	 /**< Puntero al siguiente elemento de la lista. */
	}
	LogicUConType;

typedef LogicUConType *LogicUCon;      /**< Declaración de conectivas unarias. */
typedef LogicUConType *LogicUConList;  /**< Declaración de listas de conectivas unarias. */


/**
 * Nodo conectiva binaria.
 * 
 * Una conectiva binaria aquí es una estructura que contiene el símbolo con el
 * que designamos a dicha conectiva y una matriz de valores numéricos que define
 * la semántica del operador del siguiente modo: el valor que asigna el operador
 * a sus argumentos cuando éstos valen n y m es el valor del elemento de la fila
 * n y columna m, esto es, \f$ v(n \Delta m) = \Delta_{nm} \f$, siendo la
 * expresión en notación estándar, \f$ \Delta \f$ una conectiva cualquiera y
 * \f$ n, m \f$ dos valores cualesquiera.
 * 
 * Las conectivas así definidas están implementadas a su vez como una lista
 * enlazada simple para su mejor manejo.
 */
typedef struct _logic_bin_con_node
	{
		char name;     /**< Símbolo que representa la conectiva (una letra mayúscula en la notación polaca) */
		int **matrix;  /**< Matriz de la conectiva: una matriz de enteros que define la semántica de la conectiva; contiene los valores que ésta asignará a un par de variables. */
		struct _logic_bin_con_node *next;  /**< Puntero al siguiente elemento de la lista. */
	}
	LogicBConType;

typedef LogicBConType *LogicBCon;      /**< Declaración de conectivas binarias. */
typedef LogicBConType *LogicBConList;  /**< Declaración de listas de conectivas binarias. */


/**
 * Nodo Fórmula Bien Formada (WFF: Well Formed Formula).
 * Una fbf es una estructura tipo árbol 
 */
typedef struct _logic_wff_node
	{
		LogicAtomKind kind;               /**< Tipo del elemento. */
		char name;                        /**< Símbolo que lo representa. */
		int *value;                       /**< Puntero a su valor. */
		struct _logic_wff_node *prearg;   /**< Puntero al argumento precedente. */
		struct _logic_wff_node *postarg;  /**< Puntero al argumento posterior. */
	}
	LogicWFFtype;

typedef LogicWFFtype *LogicAtom;  /**< Declaración de un elemento de una fbf. */
typedef LogicWFFtype *LogicWFF;   /**< Declaración de una fórmula bien formada. */


/**
 * Struct with all data that defines one logic
 */
typedef struct _logic
	{
		int dim;              /**< Dimensión de las matrices. */
		int mdv;              /**< Mínimo Valor Designado. */
		LogicVarList Vars;    /**< Lista de variables proposicionales. */
		LogicUConList UCons;  /**< Lista de conectivas unarias. */
		LogicBConList BCons;  /**< Lista de conectivas binarias. */
	}
	LogicType;

typedef LogicType *Logic;


/*
***	Function prototypes
 */

/* Connective related function prototypes.
 * Functions present in file connectives.c */
LogicUCon search_UCon (LogicUConList list, char name);
LogicBCon search_BCon (LogicBConList list, char name);
int add_UCon (LogicUConList *list, char name, int *mtx, int dimension);
int add_BCon (LogicBConList *list, char name, int **mtx, int dimension);
void del_connective (Logic logic, char name);


/* Variable related function prototypes.
 * Functions present in file variables.c */
bool is_empty_var_list (LogicVarList list);
void del_var_list (LogicVarList *list);
LogicVar search_var (LogicVarList list, char variable);
void add_var (LogicVarList *list, char variable);
void set_var_value (LogicVar var, int value);
int var_value (LogicVar var);


/* Well formed formulas in polish notation related function prototypes.
 * Functions present in file wffs_pn.c */
LogicSymbKind symbol_kind_pn (char symbol, Logic logic);
bool check_string (char formula[]);
bool is_wff_pn (char formula[], Logic logic);


#endif
