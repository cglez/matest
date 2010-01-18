/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * logics.h
 * Cabecera con tipos de datos y funciones para el manejo de lógicas en general.
 * 
 * Copyright (C) 2008, 2009 - César González Gutiérrez <ceguel@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, 
 * Boston, MA 02111-1307, USA. 
 */

#ifndef __LOGICS_H__
#define __LOGICS_H__


/*
***	Declaraciones del preprocesador
 */

/* Algunas abreviaciones útiles */
#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))
#define DIM logic->dimension
#define MDV logic->mdv
#define MAXV logic->dimension - 1


/*
***	Tipos de datos
 */

/** Enumeración con los tipos de símbolos de una fórmula */
typedef enum
	{
		LOGICS_SYMBOL_VAR,         /**< Variable. */
		LOGICS_SYMBOL_U_CON,       /**< Conectiva unaria. */
		LOGICS_SYMBOL_B_CON,       /**< Conectiva binaria. */
		LOGICS_SYMBOL_AUX_OPEN,    /**< Símbolo auxiliar de apertura. */
		LOGICS_SYMBOL_AUX_CLOSE,   /**< Símbolo auxiliar de cierre. */
		LOGICS_SYMBOL_NONE         /**< Tipo nulo, para el control de errores. */
	}
	LogicsSymbolType;


/** Enumeración con los tipos de nodos de una fórmula bien formada */
typedef enum
	{
		LOGICS_WFF_NODE_VAR,     /**< Variable. */
		LOGICS_WFF_NODE_U_CON,   /**< Conectiva unaria. */
		LOGICS_WFF_NODE_B_CON    /**< Conectiva binaria. */
	}
	LogicsWFFNodeType;


/**
 * Nodo variable proposicional.
 * 
 * Entendemos aquí por variable proposicional a un tipo de dato que tiene
 * asociado un símbolo que la identifica, que puede tomar distintos valores
 * numéricos y que forma un grupo, una lista, con los otros elementos de la
 * misma clase que se hayan en uso. Este tipo de dato está implementado como una
 * lista enlazada simple.
 */
typedef struct _LogicsVar LogicsVar;
struct _LogicsVar
	{
		char       symbol;   /**< Nombre de la variable o letra que la designa. */
		int        value;    /**< Valor de la variable. */
		LogicsVar  *next;    /**< Puntero al siguiente elemento de la lista. */
	};

typedef LogicsVar *LogicsVarList;  /**< Declaración de listas de variables. */


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
typedef struct _LogicsUCon LogicsUCon;
struct _LogicsUCon
	{
		char        symbol;    /**< Símbolo que representa la conectiva (una letra 
		                            mayúscula en la notación polaca) */
		int         *matrix;   /**< Matriz de la conectiva: un vector de enteros
		                            que define la semántica de la conectiva; contiene
		                            los valores que ésta asignará a una variable. */
		LogicsUCon  *next;     /**< Puntero al siguiente elemento de la lista. */
	};

typedef LogicsUCon *LogicsUConList;  /**< Declaración de listas de conectivas unarias. */


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
typedef struct _LogicsBCon LogicsBCon;
struct _LogicsBCon
	{
		char       symbol;   /**< Símbolo que representa la conectiva (una letra
		                          mayúscula en la notación polaca) */
		int**      matrix;   /**< Matriz de la conectiva: una matriz de enteros que
		                          define la semántica de la conectiva; contiene los
		                          valores que ésta asignará a un par de variables. */
		LogicsBCon *next;    /**< Puntero al siguiente elemento de la lista. */
	};

typedef LogicsBCon *LogicsBConList;  /**< Declaración de listas de conectivas binarias. */


/**
 * Nodo Fórmula Bien Formada (WFF: Well Formed Formula).
 * Una fbf es una estructura tipo árbol en la que el nodo principal es la
 * conectiva principal, sus nodos hijos pueden ser o bien otras conectivas o
 * bien variables, por último los nodos hoja son siempre variables. Puede darse
 * el caso de que el árbol esté formado por un sólo elemento, un nodo hoja, el
 * caso de que sea una variable proposicional.
 *
 * Cada nodo está bien formado si, o bien
 * - el tipo de nodo es de conectiva binaria y tanto el preargumento como el
 *   postargumento apuntan a sendos nodos, o bien
 * - el tipo de nodo es de conectiva unaria, el postargumento apunta a otro
 *   nodo y el preargumento es el puntero nulo, o bien
 * - el tipo de nodo es de variable y ambos argumentos son el puntero nulo.
 */
typedef struct _LogicsWFFNode LogicsWFFNode;
struct _LogicsWFFNode
	{
		LogicsWFFNodeType type;      /**< Tipo del elemento. */
		char symbol;                 /**< Símbolo que lo representa. */
		int *value;                  /**< Puntero a su valor. */
		LogicsWFFNode *prearg;       /**< Puntero al argumento precedente. */
		LogicsWFFNode *postarg;      /**< Puntero al argumento posterior. */
	};

typedef LogicsWFFNode *LogicsWFF;   /**< Declaración de una fórmula bien formada. */


/**
 * Registro con todos los datos que definen una lógica. Una lógica, aquí, es
 * un conjunto de elementos consistentes entre sí. Consta de una dimensión para
 * las matrices, un mínimo valor designado, que será mayor que 0 y menor que
 * la dimensión, una lista de variables que pueden tomar valores entre 0 y la
 * dimensión menos 1, y sendas listas de conectivas unarias y binarias donde sus
 * matrices son de dimensión igual a la dimensión general y cada uno de sus
 * elementos toman valores entre 0 y la dimensión menos 1.
 */
typedef struct
	{
		int             dimension;   /**< Dimensión de las matrices. */
		int             mdv;         /**< Mínimo Valor Designado. */
		LogicsVarList   vars;        /**< Lista de variables proposicionales. */
		LogicsUConList  ucons;       /**< Lista de conectivas unarias. */
		LogicsBConList  bcons;       /**< Lista de conectivas binarias. */
	}
	LogicsLogic;


/*
***	Prototipos de funciones
 */

/* Funciones sobre variables */
LogicsVar* logics_var_new (char symbol, int value);
void logics_var_set_value (LogicsVar* var, int value);
int logics_var_get_value (LogicsVar* var);
bool logics_var_list_is_empty (LogicsVarList var_list);
void logics_var_list_free (LogicsVarList *var_list);
LogicsVar* logics_var_list_get_var_by_symbol (LogicsVarList var_list, char symbol);
unsigned int logics_var_list_length (LogicsVarList var_list);
void logics_var_list_add_var (LogicsVarList *var_list, LogicsVar* var);

/* Funciones sobre conectivas */
LogicsUCon* logics_ucon_list_get_ucon_by_symbol (LogicsUConList ucon_list, char symbol);
LogicsBCon* logics_bcon_list_get_bcon_by_symbol (LogicsBConList bcon_list, char symbol);
int logics_ucon_list_append_ucon (LogicsUConList *ucon_list, LogicsUCon* ucon);
int logics_bcon_list_append_bcon (LogicsBConList *bcon_list, LogicsBCon* bcon);
void logics_con_delete_by_symbol (LogicsLogic* logic, char symbol);
void logics_logic_set_default_ucons_lukasiewicz (LogicsLogic* logic);
void logics_logic_set_default_bcons_lukasiewicz (LogicsLogic* logic);
void logics_ucon_list_free (LogicsUConList *ucon_list);
void logics_bcon_list_free (LogicsBConList *bcon_list, int dimension);

/* Funciones para fórmulas bien formadas en notación polaca */
LogicsSymbolType logics_symbol_pn_get_type (char symbol, LogicsLogic* logic);
bool check_string (char formula[]);
bool logics_formula_is_wff_pn (char formula[], LogicsLogic* logic);


#endif /* __LOGICS_H__ */