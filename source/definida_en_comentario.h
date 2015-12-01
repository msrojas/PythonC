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
#ifndef DEFINIDA_EN_COMENTARIO_H
#define DEFINIDA_EN_COMENTARIO_H

#include "compilador.h"

typedef struct var_elemento
{
	struct var_elemento * next;
	uint8_t * var_name;
	uint8_t token;
}var_elemento;

typedef struct var_definidas
{
	unsigned int size;
	var_elemento ** table;
}var_definidas;

typedef struct var_it
{
	var_definidas * ht;
	unsigned int index;
	var_elemento * elem;
}var_it;

extern var_definidas * definir;

#define VAR_ITERATOR(ht) {ht, 0, ht->table[0]}

uint8_t init_definir(unsigned int size);
unsigned int var_calc_hash(uint8_t * key);
uint8_t var_put(uint8_t * key, uint8_t token);
void * var_remove(uint8_t * key);
uint8_t var_get(uint8_t * key);
var_elemento * var_iterate(var_it * iterator);
uint8_t * var_iterate_keys(var_it * iterator);
void var_clear(int free_data);
void var_destroy();
void print_raw_input(FILE * archivo);
uint8_t Es_Operador(uint8_t valor);
uint8_t verificar_raw_input(uint8_t * cadena, FILE * archivo_output);
uint8_t buscar_funciones(FILE * archivo, FILE * archivo_output);
uint8_t comentario_parser(uint8_t * cadena, uint16_t index, uint8_t token_t);
uint8_t verificar_comentario(uint8_t * cadena,unsigned int size);

#endif
