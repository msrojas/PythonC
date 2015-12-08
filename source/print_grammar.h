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
#ifndef PRINT_GRAMMAR_H
#define PRINT_GRAMMAR_H

#include "compilador.h"

uint8_t buscar_corchetes(lexical * lexer, uint8_t index, uint8_t size);
uint8_t check_sub_indices(lexical * lexer, uint8_t size);
uint8_t StrCat(lexical * lexer, uint8_t * temp);
uint8_t * get_formato(uint8_t size, lexical * lexer, uint8_t contador);
uint8_t buscar_float(lexical * lexer, uint8_t index, uint8_t size);
void check_float(uint8_t size, lexical * lexer);
uint8_t check_concatenacion(lexical * lexer, uint8_t size);
short get_len_numbers(lexical * lexer, uint8_t size);
uint8_t * print_original_value(lexical * lexer, uint8_t size, data * datos);
data * check_print_grammar(lexical * lexer, uint8_t size);
	
#endif
