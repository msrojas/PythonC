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

void libera_free_vars()
{
    if(size_free == 0 && free_variables != NULL)
    {
        free(free_variables);
        return;
    }

    uint8_t i=0;
    free_struct * temp;

    for(i=0;i<=size_free;i++)
    {
        temp = free_variables;
        free_variables = free_variables->next;
        if(temp->var_name != NULL)
            free(temp->var_name);
        free(temp);
    }
}

void free_vars_compile(FILE * archivo)
{
	uint8_t i=0;
	uint8_t temp[100];
	memset(temp, 0, sizeof(temp));

	free_variables = free_variables->next; //Omitimos el nodo 0 (que es el root)

	for(i=0;i<size_free;i++)
	{
		sprintf(temp, "free(%s);", free_variables->var_name);
		fprintf(archivo, "%*s\n", strlen(temp)+4,temp);
		memset(temp, 0, sizeof(temp));
		free_variables = free_variables->next;
	}
}

void acomodar_free_vars()
{
	free_variables->next = free_root;
	free_variables = free_root;
}

uint8_t agregar_free_var(uint8_t * var)
{
	free_struct * new_node = (free_struct *)malloc(sizeof(free_struct));
	if(new_node == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return 0;
	}

	new_node->var_name = (uint8_t *)malloc(strlen(var)+1);
	if(new_node->var_name == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return 0;
	}

	strcpy(new_node->var_name, var);
	free_variables->next = new_node;
	free_variables = new_node;

	return 1;
}

uint8_t init_free()
{
	free_variables = (free_struct *)malloc(sizeof(free_struct));
	if(free_variables == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return 0;
	}

	return 1;
}