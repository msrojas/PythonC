/*	
    Traducir codigo python a codigo c
    Copyright (C) 2015  Alejandro Torres Hernandez

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    Email: alez011@hotmail.com
*/    
#ifndef COMPILADOR_H
#define COMPILADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdarg.h>

typedef struct list_tokens
{
	uint8_t * valor;
	uint8_t token;
	struct list_tokens * next;
}lexical;

typedef struct _data
{
	uint8_t token;
	uint16_t len;
	uint8_t operacion_secundaria;
	uint8_t numero_de_cadenas;
	uint8_t declarada;
}data;

typedef struct struct_balanced
{
	uint8_t data;
	struct struct_balanced * next;
}balanced;

typedef struct symbol_table_sizes
{
	uint32_t symbol_variable;
	uint32_t symbol_comentario_var;
}symbol_table_sizes;

#include "variable_grammar.h"
#include "print_grammar.h"
#include "sintaxis_basica.h"
#include "symbol_table.h"
#include "free_lib.h"
#include "definida_en_comentario.h"

extern uint32_t linea;
extern uint8_t fixed_width;

enum {VARIABLE=1,CADENA,NUMERO,IGUAL,OPERADOR,PRINT,R_PARENTESIS,L_PARENTESIS,OPE_DE_PARENTESIS,CONCATENACION,PLUS_CONCAT,CHAR,FLOAT,RAW_INPUT,INT,FLOAT_F,LEN}tokens;

void debug(const char * format, ...);
void liberacion_general();
void libera_tokens(lexical ** lexer, uint8_t size);
void acomodar_nodos(lexical ** lexer, lexical ** root);
uint8_t agregar_token(lexical ** lexer, uint8_t * temp, uint8_t r_token);
void print_tokens(lexical * lexer, uint8_t size);
lexical * init_lexer();
void libera_stack(balanced ** stack);
uint8_t check_pair(uint8_t value1, uint8_t value2);
uint8_t pop(balanced ** temp);
uint8_t push(balanced ** temp, uint8_t new_data);
uint8_t check_balanced(uint8_t * cadena, uint8_t token);
uint8_t * remove_quotes(uint8_t * cadena);
uint8_t * semantic_analyzer(lexical ** lexer, int len, uint8_t size);
uint8_t es_operador(uint8_t valor);
uint8_t check_keyword(uint8_t * token);
uint8_t check_quotes(uint8_t * token, lexical ** lexer, uint8_t i);
uint8_t get_token_number(uint8_t * token, lexical ** lexer, uint8_t i);
uint8_t get_token_funcion_o_var(uint8_t * token, lexical ** lexer, uint8_t i);
int len_sin_espacios(uint8_t * cadena);
uint8_t * parser(uint8_t * cadena);

#endif
