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

uint8_t ht_put(uint8_t * key, uint8_t token, uint8_t token_function,uint8_t * value)
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
	e->token_function = token_function;
	if(value != NULL) //NUEVO
	{
	    e->value = malloc(strlen(value)+1);
	    if(e->value == NULL)
	    {
	        debug("\nError: ocurrio un inconveniente en la heap\n");
	        return 0;
	    }
	    strcpy(e->value, value);
	}
	else
	    e->value = NULL;

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
			if(e->value != NULL)
			    free(e->value);
			free(e);
			e = NULL;
			return data;
		}
		prev = e;
		e = e->next;
	}

	return 0;

}

uint16_t ht_get_len(uint8_t * key)
{
    uint32_t h = ht_calc_hash(key) % hash->size;
    hash_elem_t * e = hash->table[h];

    while(e != NULL)
    {
        if(strcmp(e->var_name, key) == 0 && e->token_function == ZERO)
        {
            uint16_t len = 0;
            if(e->value != NULL)
                len = strlen(e->value);
            else
                break;
            return len;
        }
        else if(strcmp(e->var_name, key) == 0 && e->token_function == RAW_INPUT)
        {
            uint16_t len = 0;
            len = strlen(key)+2;
            return len;
        }

        e = e->next;
    }
    debug("\nError: bug en funcion: ht_get_len\n");
    return 0;
}

uint8_t ht_get(uint8_t * key)
{
	uint32_t h = ht_calc_hash(key) % hash->size;
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

uint8_t * ht_get_value(uint8_t * key)
{
    uint32_t h = ht_calc_hash(key) % hash->size;
    hash_elem_t * e = hash->table[h];

    while(e != NULL)
    {
        if(strcmp(e->var_name, key) == 0 && e->token_function == ZERO)
        {
            uint8_t * string = NULL;
            if(e->value != NULL)
            {
                string = malloc(strlen(e->value)+1);
                if(string == NULL)
                {
                    debug("Error: ocurrio un inconveniente en la heap");
                    return NULL;
                }
                strcpy(string, e->value);
            }
            else
                break;
            return string;
        }
        else if(strcmp(e->var_name, key) == 0 && e->token_function == RAW_INPUT)
        {
            uint8_t * string = NULL;
            string = malloc(strlen(key)+3);
            if(string == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }
            //strcpy(string, key);
            sprintf(string, "\n%s", key);
            return string;
        }

        e = e->next;
    }
    debug("\nError: bug en funcion: ht_get_value\n");
    return NULL;
}

uint16_t get_len_value(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    uint16_t len = 0;
    uint16_t len_value = 0;
    uint8_t token = 0;
    lexer = lexer->next;
    lexer = lexer->next;

    size -= 2;

    for(i=0;i<size;i++)
    {
        if(lexer->token == CADENA )
        {
            len += strlen(lexer->valor);
        }
        else if(lexer->token == VARIABLE)
        {
            token = ht_get(lexer->valor);
            if(token == 0)
            {
                debug("\nErrno en linea %d: la variable no ha sido declarada: %s\n", linea,lexer->valor);
                return 0;
            }
            else if(token != CADENA)
            {
                debug("\nErrno en linea %d: la variable no es tipo cadena: %s\n", linea,lexer->valor);
                return 0;
            }
            else if(token == CADENA)
            {
                len_value = ht_get_len(lexer->valor);
                if(len_value == 0)
                    return 0;
                len += len_value;
            }
        }
        lexer = lexer->next;
    }

    return len;
}

uint8_t * agregar_valor(lexical * lexer, uint8_t size) //NUEVO
{
    uint8_t i=0;
    uint8_t token = 0;
    uint16_t len = get_len_value(lexer, size);
    if(len == 0)
        return NULL;
    uint8_t temp[len+1];
    memset(temp, 0, sizeof(temp));
    uint8_t * string = NULL;

    lexer = lexer->next;
    lexer = lexer->next;

    size -= 2;

    for(i=0;i<size;i++)
    {
        if(lexer->token == CADENA )
        {
            strcat(temp, lexer->valor);
        }
        else if(lexer->token == VARIABLE)
        {
            token = ht_get(lexer->valor);
            if(token == 0)
            {
                debug("\nErrno en linea %d: la variable no ha sido declarada: %s\n", linea,lexer->valor);
                return NULL;
            }
            else if(token != CADENA)
            {
                debug("\nErrno en linea %d: la variable no es tipo cadena: %s\n", linea,lexer->valor);
                return NULL;
            }
            else if(token == CADENA)
            {
                string = ht_get_value(lexer->valor);
                if(string == NULL)
                    return NULL;
                strcat(temp, string);
                free(string);
                string = NULL;
            }
        }
        lexer = lexer->next;
    }
    uint8_t * retorna = malloc(strlen(temp)+1);
    if(retorna == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }
    strcpy(retorna, temp);
    return retorna;
}

uint8_t update_valor(uint8_t * key, uint8_t * value)
{
    uint32_t h = ht_calc_hash(key) % hash->size;
    hash_elem_t * e = hash->table[h];

    while(e != NULL)
    {
        if(strcmp(e->var_name, key) == 0)
        {
        	if(e->token_function == ZERO)
            {
                free(e->value);
                e->value = NULL;
            }   
            e->value = malloc(strlen(value)+1);
            if(e->value == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return 0;
            }
            strcpy(e->value, value);
            if(e->token_function != ZERO)
                e->token_function = ZERO;
            return 1;
        }

        e = e->next;
    }
    debug("\nError: bug en funcion: update_valor\n");
    return 0;
}

uint8_t update_token_function(uint8_t * key, uint8_t token_function)
{
    uint32_t h = ht_calc_hash(key) % hash->size;
    hash_elem_t * e = hash->table[h];

    while(e != NULL)
    {
        if(strcmp(e->var_name, key) == 0)
        {
            if(e->token_function != token_function)
                e->token_function = token_function;
            return 1;
        }

        e = e->next;
    }
    debug("\nError: bug en funcion: update_token_function\n");
    return 0;
}

uint8_t get_token_function(uint8_t * key)
{
    uint32_t h = ht_calc_hash(key) % hash->size;
    hash_elem_t * e = hash->table[h];

    while(e != NULL)
    {
        if(strcmp(e->var_name, key) == 0)
        {            
            return e->token_function;
        }

        e = e->next;
    }

    return 0;
}
