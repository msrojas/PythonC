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
#ifndef FREE_LIB_H
#define FREE_LIB_H

#include "compilador.h"

typedef struct free_struct
{
	uint8_t * var_name;
	struct free_struct * next;
}free_struct;

extern free_struct * free_variables;
extern free_struct * free_root;
extern uint32_t size_free;

void libera_free_vars();
void free_vars_compile(FILE * archivo);
void acomodar_free_vars();
uint8_t agregar_free_var(uint8_t * var);
uint8_t init_free();

#endif