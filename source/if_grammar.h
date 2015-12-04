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

#ifndef IF_GRAMMAR_H
#define IF_GRAMMAR_H

#include "compilador.h"

uint8_t busca_parentesis(lexical * lexer, uint8_t size);
uint8_t verificar_expresion(lexical * lexer, uint8_t size);
uint8_t * print_original_else(lexical * lexer, uint8_t size);
uint8_t * print_original_if(lexical * lexer, uint8_t size);
uint8_t check_if_grammar(lexical * lexer, uint8_t size);

#endif
