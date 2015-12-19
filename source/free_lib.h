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

typedef struct free_elem_t
{
    struct free_elem_t * next;
    uint8_t * var_name;
    uint8_t liberada;
    uint8_t token;
}free_elem_t;

typedef struct free_table_t
{
    uint32_t size;
    free_elem_t ** table;
}free_table_t;

typedef struct free_elem_it
{
    free_table_t * ht;
    uint32_t index;
    free_elem_t * elem;
}free_elem_it;

extern free_table_t * free_hash;

#define FREE_ITERATOR(ht) {ht, 0, ht->table[0]}

free_table_t * free_create(uint32_t size);
uint32_t free_calc_hash(uint8_t * key);
uint8_t free_libera(uint8_t * key,uint8_t liberar);
uint8_t free_put(uint8_t * key, uint8_t liberar,uint8_t token);
void * free_remove(uint8_t * key);
uint8_t free_get(uint8_t * key);
free_elem_t * free_iterate(free_elem_it * iterator);
uint8_t * free_iterate_keys(free_elem_it * iterator);
void free_clear(int free_data);
void free_destroy();
void free_vars_compile(FILE * archivo);

#endif
