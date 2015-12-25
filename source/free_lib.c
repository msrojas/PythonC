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
#include "free_lib.h"

free_table_t * free_create(uint32_t size)
{
	free_hash = (free_table_t *)malloc(sizeof(free_table_t));
	if(free_hash == NULL)
		return NULL;

	if((free_hash->table = malloc(size*sizeof(free_elem_t*))) == NULL)
	{
		free(free_hash);
		return NULL;
	}

	free_hash->size = size;

	uint32_t i = 0;
	for(i=0;i<size;i++)
		free_hash->table[i] = NULL;
}

uint32_t free_calc_hash(uint8_t * key)
{
	uint32_t h = 5381;
	while(*(key++))
		h = ((h << 5) + h) + (*key);

	return h;
}

uint8_t free_libera(uint8_t * key,uint8_t liberar)
{
    uint32_t h = free_calc_hash(key) % free_hash->size;
    free_elem_t * e = free_hash->table[h];

    while(e != NULL)
    {
        if(strcmp(e->var_name, key) == 0)
        {
            e->liberada = liberar;
            return 1;
        }
        e = e->next;
    }

    debug("\nError: la variable no existe: %s\n", key);

    return 0;
}

uint8_t free_put(uint8_t * key, uint8_t liberar, uint8_t token)
{
	uint32_t h = free_calc_hash(key) % free_hash->size;
	free_elem_t * e = free_hash->table[h];

	while(e != NULL) //Para evitar colisiones
	{
		if(strcmp(e->var_name, key) == 0)
		{
			printf("Error: posible bug\n"); //La variable solo debe ser almacenada una vez
			return 2;
		}

		e = e->next; 
	}

	if((e = malloc(sizeof(free_elem_t))) == NULL)
	{
		printf("Error: ocurrio un inconveniente en la heap\n");
		return 0;
	}

	e->var_name = (uint8_t *)malloc(strlen(key)+1);
	if(e->var_name == NULL)
	{
		printf("Error: ocurrio un inconveniente en la heap\n");
		return 0;
	}

	strcpy(e->var_name, key);
	e->liberada = liberar;
	e->token = token;

	e->next = free_hash->table[h];
	free_hash->table[h] = e;

	return 1;
}

void * free_remove(uint8_t * key)
{
	uint32_t h = free_calc_hash(key) % free_hash->size;
	free_elem_t * e = free_hash->table[h];
	free_elem_t * prev = NULL;

	while(e != NULL)
	{
		if(strcmp(e->var_name, key) == 0)
		{
			void * data = e->var_name;

			if(prev != NULL)
				prev->next = e->next;
			else
				free_hash->table[h] = e->next;
			free(e);
			e = NULL;
			return data;
		}
		prev = e;
		e = e->next;
	}

	return 0;

}

uint8_t free_get(uint8_t * key)
{
	uint32_t h = free_calc_hash(key) % free_hash->size;
	free_elem_t * e = free_hash->table[h];

	while(e != NULL)
	{
		if(strcmp(e->var_name, key) == 0)
		{
			return e->liberada;
		}

		e = e->next;
	}

	return 0;
}

free_elem_t * free_iterate(free_elem_it * iterator)
{
	while(iterator->elem == NULL)
	{
		if(iterator->index < iterator->ht->size - 1)
		{
			iterator->index++;
			iterator->elem = iterator->ht->table[iterator->index];
		}
		else
			return NULL;
	}

	free_elem_t * e = iterator->elem;
	if(e)
		iterator->elem = e->next;
	return e;
}

uint8_t * free_iterate_keys(free_elem_it * iterator)
{
	free_elem_t * e = free_iterate(iterator);
	return(e == NULL ? NULL : e->var_name);
}

void free_clear(int free_data)
{
	free_elem_it it = FREE_ITERATOR(free_hash);
	uint8_t * k = free_iterate_keys(&it);

	while(k != NULL)
	{
		free_data ? free(free_remove(k)) : free_remove(k);
		k = free_iterate_keys(&it);
	}
}

void free_destroy()
{
	free_clear(1);
	free(free_hash->table);
	free(free_hash);
}

void free_vars_compile(FILE * archivo)
{
	uint8_t i=0;
	uint8_t temp[150];
	memset(temp, 0, sizeof(temp));

	free_elem_it it = FREE_ITERATOR(free_hash);
	free_elem_t * e = free_iterate(&it);
	while(e != NULL)
	{
		if(e->token == LIST && e->liberada == 1)
		{
		    sprintf(temp, "destroy_list(%s->next);", e->var_name);
		    fprintf(archivo, "%*s\n", strlen(temp)+4,temp);
		    memset(temp, 0, sizeof(temp));
		}
		if(e->liberada == 1)
		{
            	    sprintf(temp, "free(%s);", e->var_name);
		    fprintf(archivo, "%*s\n", strlen(temp)+4,temp);
		    memset(temp, 0, sizeof(temp));
		}
		e = free_iterate(&it);
	}
	if(list_existe)
	    fprintf(archivo, "%*s\n", strlen("liberacion_extra();")+4, "liberacion_extra();");
}
