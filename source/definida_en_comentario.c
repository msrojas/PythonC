/*
    Este symbol table esta basado en el hash table de Pierre-Henri Symoneaux
    Para mas informacion: https://gist.github.com/phsym/4605704

	(My english is not good)

    This symbol table is based on Pierre-Henri Symoneaux
    More info: https://gist.github.com/phsym/4605704
*/

#include "definida_en_comentario.h"

const uint8_t operador[] = "+-*/=";    

uint8_t init_definir(unsigned int size)
{
	definir = (var_definidas *)malloc(sizeof(var_definidas));
	if(definir == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return 0;
	}

	definir->table = malloc(sizeof(var_elemento *)*size);
	if(definir->table == NULL)
	{
		free(definir);
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return 0;
	}

	definir->size = size;

    uint32_t i = 0;
    for(i=0;i<size;i++)
        definir->table[i] = NULL;

}

unsigned int var_calc_hash(uint8_t * key)
{
    uint32_t h = 5381;
    while(*(key++))
        h = ((h << 5) + h) + (*key);

    return h;
}

uint8_t var_put(uint8_t * key, uint8_t token)
{
	uint32_t h = var_calc_hash(key) % definir->size;
	var_elemento * e = definir->table[h];

	while(e != NULL) //Para evitar colisiones
	{
		if(strcmp(e->var_name, key) == 0)
		{
			printf("Error: posible bug\n"); //La variable solo debe ser almacenada una vez
			return 2;
		}

		e = e->next; 
	}

	if((e = malloc(sizeof(var_elemento))) == NULL)
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

	e->next = definir->table[h];
	definir->table[h] = e;

	return 1;
}

void * var_remove(uint8_t * key)
{
    uint32_t h = var_calc_hash(key) % definir->size;
    var_elemento * e = definir->table[h];
    var_elemento * prev = NULL;

    while(e != NULL)
    {
        if(strcmp(e->var_name, key) == 0)
        {
            void * data = e->var_name;

            if(prev != NULL)
                prev->next = e->next;
			else
                definir->table[h] = e->next;
            free(e);
            e = NULL;
            return data;
        }
        prev = e;
        e = e->next;
    }

    return 0;

}

uint8_t var_get(uint8_t * key)
{
	uint32_t h = var_calc_hash(key) % definir->size;
	var_elemento * e = definir->table[h];

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

var_elemento * var_iterate(var_it * iterator)
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

    var_elemento * e = iterator->elem;
    if(e)
        iterator->elem = e->next;
    return e;
}

uint8_t * var_iterate_keys(var_it * iterator)
{
	var_elemento * e = var_iterate(iterator);
	return(e == NULL ? NULL : e->var_name);
}

void var_clear(int free_data)
{
	var_it it = VAR_ITERATOR(definir);
	uint8_t * k = var_iterate_keys(&it);

	while(k != NULL)
	{
		free_data ? free(var_remove(k)) : var_remove(k);
		k = var_iterate_keys(&it);
	}
}

void var_destroy()
{
	var_clear(1);
	free(definir->table);
	free(definir);
}

uint8_t Es_Operador(uint8_t valor)
{
	uint8_t i=0;
	uint8_t retorno = 0;

	for(i=0;i<5;i++)
	{
		if(operador[i] == valor)
		{
			retorno = 1;
			break;
		}
	}

	return retorno;
}

uint8_t comentario_parser(uint8_t * cadena, uint16_t index, uint8_t token_t)
{
    index++;
    uint16_t len = strlen(cadena);
    uint8_t temp[len+1];
    memset(temp, 0, sizeof(temp));

    uint16_t i=0;
    uint16_t indice = 0;
    uint8_t chars = 0;
    uint8_t espacios = 0;
    for(i=index;i<len;i++)
    {
        if(isspace(cadena[i]))
        {
            if(chars == 1 && espacios == 0)
                espacios++;
		}
        else if(!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != '_')
        {
            debug("\nError en linea %d: el compilador encontro un intento de definir una variable en un comentario. Sin embargo, el valor %c no es permitido\n", linea,cadena[i]);
            return 0;
        }
        else
        {
            if(chars == 0 && espacios == 0)
                chars++;
            else if(espacios == 1)
            {
                debug("\nError en linea %d: el compilador encontro un intento de definir una variable en un comentario. Sin embargo, el nombre de la variable no debe contener espacios\n", linea);
                return 0;
            }
            temp[indice++] = cadena[i];
        }
    }

    if(!check_keyword(temp))
        return 0;

	uint8_t token = ht_get(temp);
	if(token != 0)
	{
		if(token == NUMERO && token_t == CHAR)
			debug("\nError en linea %d: la variable \"%s\" ya fue declarada como entero. Por lo tanto, no puedes declarla como \"char\"\n",linea,temp);
		else if((token == CADENA || token == FLOAT) && token_t == CHAR)
			debug("\nError en linea %d: la variable \"%s\" ya fue declarada. No puedes redefinirla como \"char\"\n",linea,temp);
		else if(token == NUMERO && token_t == FLOAT)
			debug("\nError en linea %d: la variable \"%s\" ya fue declarada como entero. Por lo tanto, no puedes redefinirla como \"float\"",linea,temp);
		else if(token == FLOAT && token_t == FLOAT)
			debug("\nError en linea %d: la variable \"%s\" ya fue declarada. No puedes redefinirla como \"float\"\n",linea,temp);
		return 0;
	}
    else if(token == 0)
    {
        if(var_get(temp) != 0)
            debug("\nAviso en la linea %d: has repetido la misma declaracion, por lo cual se ha omitido. No causa conflictos, pero se recomienda que se elimine\n", linea);
        else
        {
            uint8_t id = var_put(temp, token_t);
            if(id == 0 || id == 2)
                return 0;
        }
    }

    return 1;
}

uint8_t verificar_comentario(uint8_t * cadena,uint32_t size)
{
    uint16_t len = strlen(cadena);
    uint8_t temp[len+1];
    memset(temp, 0, sizeof(temp));

    uint16_t i=0;
    uint16_t indice = 0;
    for(i=1;i<len;i++)
    {
        if(isspace(cadena[i]))
            break;
        else
            temp[indice++] = cadena[i];
    }

    if(strcmp(temp, "char") == 0)
    {
        if(definir == NULL)
            init_definir(size);
        if(!comentario_parser(cadena, i, CHAR))
            return 0;
    }
    else if(strcmp(temp, "float") == 0)
    {
        if(definir == NULL)
            init_definir(size);
        if(!comentario_parser(cadena, i, FLOAT))
            return 0;
    }

    return 1;
}

