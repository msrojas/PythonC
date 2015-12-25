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
#ifndef VARIABLE_GRAMMAR_H
#define VARIABLE_GRAMMAR_H

#include "compilador.h"

uint8_t * get_name(lexical * lexer, uint8_t size, uint8_t * var_name); //NUEVO
uint8_t * get_char_indice(lexical * lexer);
uint16_t get_len_indice(lexical * lexer);
uint8_t * get_cpy_indice(lexical * lexer, uint8_t index, uint8_t * var);
uint8_t * get_cpy_string(uint8_t * valor, uint8_t index, uint8_t * var);
uint8_t * get_strcpy_string(lexical * lexer, uint8_t size, uint8_t * var, uint8_t * value); //NUEVO
uint8_t * get_string_indice(lexical * lexer, uint8_t index);
uint8_t * get_string(uint8_t * valor, uint8_t index);
uint8_t * get_malloc_string(lexical * lexer, uint8_t size, uint8_t * var, uint8_t * value);//NUEVO
uint16_t get_len_strcpy(lexical * lexer, uint8_t size, uint8_t * var, uint8_t * value);//NUEVO
uint16_t get_len_malloc(lexical * lexer, uint8_t size, uint8_t * value, uint8_t * var); //NUEVO
uint8_t get_len_concat(lexical * lexer, uint8_t size); //NUEVO
uint8_t get_number_of_bytes(uint16_t len);
void checa_y_modifica(uint8_t * cadena);
uint16_t get_bytes(lexical * lexer, uint8_t size);
uint16_t get_funciones_extras(lexical * lexer, uint8_t size);
uint16_t get_len_parametros(lexical * lexer, uint8_t size);
uint8_t get_numero_elemento(lexical * lexer, uint8_t size);
uint8_t * add_elemento(lexical * lexer);
uint8_t * print_origanl_variable(data * datos, lexical * lexer, int len_cadena, uint8_t size);
uint8_t check_int_o_float(lexical * lexer, uint8_t size);
data * check_variable_grammar(lexical * lexer, uint8_t size);

#endif
