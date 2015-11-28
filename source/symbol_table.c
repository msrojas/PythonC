/*
    Este symbol table esta basado en el hash table de Pierre-Henri Symoneaux
    Para mas informacion: https://gist.github.com/phsym/4605704

	(My english is not good)

    This symbol table is based on Pierre-Henri Symoneaux
    More info: https://gist.github.com/phsym/4605704
*/

#include "symbol_table.h"

void print_token(uint8_t token)
{
	switch(token)
	{
		case CADENA:
			printf("CADENA\n");
			break;
		case NUMERO:
			printf("NUMERO\n");
			break;
		default:
			printf("DESCONOCIDO\n");
			break;		
	}
}

hashtable_t * ht_create(unsigned int size)
{
	hash = (hashtable_t *)malloc(sizeof(hashtable_t));
	if(hash == NULL)
		return NULL;

	if((hash->table = malloc(size*sizeof(hash_elem_t*))) == NULL)
	{
		free(hash);
		return NULL;
	}

	hash->size = size;

	unsigned int i = 0;
	for(i=0;i<size;i++)
		hash->table[i] = NULL;
}

unsigned int ht_calc_hash(uint8_t * key)
{
	unsigned int h = 5381;
	while(*(key++))
		h = ((h << 5) + h) + (*key);

	return h;
}

uint8_t ht_put(uint8_t * key, uint8_t token)
{
	unsigned int h = ht_calc_hash(key) % hash->size;
	hash_elem_t * e = hash->table[h];

	while(e != NULL) //Para evitar colisiones
	{
		if(strcmp(e->var_name, key) == 0)
		{
			printf("Error: posible bug\n"); //La variable solo debe ser almacenada una vez
			return 2;
		}

		e = e->next; 
	}

	if((e = malloc(sizeof(hash_elem_t))) == NULL)
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
	e->token = token;

	e->next = hash->table[h];
	hash->table[h] = e;

	return 1;
}

void * ht_remove(uint8_t * key)
{
	unsigned int h = ht_calc_hash(key) % hash->size;
	hash_elem_t * e = hash->table[h];
	hash_elem_t * prev = NULL;

	while(e != NULL)
	{
		if(strcmp(e->var_name, key) == 0)
		{
			void * data = e->var_name;

			if(prev != NULL)
				prev->next = e->next;
			else
				hash->table[h] = e->next;
			free(e);
			e = NULL;
			return data;
		}
		prev = e;
		e = e->next;
	}

	return 0;

}

uint8_t ht_get(uint8_t * key)
{
	unsigned int h = ht_calc_hash(key) % hash->size;
	hash_elem_t * e = hash->table[h];

	while(e != NULL)
	{
		if(strcmp(e->var_name, key) == 0)
		{
			return e->token;
		}

		e = e->next;
	}

	return 0;
}

hash_elem_t * ht_iterate(hash_elem_it * iterator)
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

	hash_elem_t * e = iterator->elem;
	if(e)
		iterator->elem = e->next;
	return e;
}

uint8_t * ht_iterate_keys(hash_elem_it * iterator)
{
	hash_elem_t * e = ht_iterate(iterator);
	return(e == NULL ? NULL : e->var_name);
}

void ht_clear(int free_data)
{
	hash_elem_it it = HT_ITERATOR(hash);
	uint8_t * k = ht_iterate_keys(&it);

	while(k != NULL)
	{
		free_data ? free(ht_remove(k)) : ht_remove(k);
		k = ht_iterate_keys(&it);
	}
}

void ht_destroy()
{
	ht_clear(1);
	free(hash->table);
	free(hash);
}