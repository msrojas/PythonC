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
    if(definir == NULL)
        return 0;
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

void print_como_compilar(FILE * archivo)
{
    fprintf(archivo, "/*\n");
    fprintf(archivo, "%*s\n", strlen("Si no usas te la funcion raw_input ni tampoco las listas")+4, "Si no usas te la funcion raw_input ni tampoco las listas");
    fprintf(archivo, "%*s\n\n", strlen("entonces simplemente compila de forma normal: gcc ejemplo.c -o ejemplo")+4, "entonces simplemente compila de forma normal: gcc ejemplo.c -o ejemplo");
    fprintf(archivo, "%*s\n\n", strlen("Si usas te raw_input: gcc ejemplo.c -o ejemplo -Linclude -lraw_input")+4, "Si usas te raw_input: gcc ejemplo.c -o ejemplo -Linclude -lraw_input");
    fprintf(archivo, "%*s\n\n", strlen("Si usas te las listas: gcc ejemplo.c -o ejemplo -Linclude -llist")+4, "Si usas te las listas: gcc ejemplo.c -o ejemplo -Linclude -llist");
    fprintf(archivo, "%*s\n", strlen("Si usas te las dos: gcc ejemplo.c -o ejemplo -Linclude -llist -lraw_input")+4, "Si usas te las dos: gcc ejemplo.c -o ejemplo -Linclude -llist -lraw_input");
    fprintf(archivo, "*/\n");
}

void print_strchar(FILE * archivo) //NUEVO
{
    fprintf(archivo, "void strchar(char * str, char c)\n");
    fprintf(archivo, "{\n");
    fprintf(archivo, "%*s\n", strlen("char temp[2];")+4,"char temp[2];");
    fprintf(archivo, "%*s\n\n", strlen("memset(temp, 0, sizeof(temp));")+4,"memset(temp, 0, sizeof(temp));");
    fprintf(archivo, "%*s\n", strlen("temp[0] = c;")+4,"temp[0] = c;");
    fprintf(archivo, "%*s\n", strlen("strcat(str, temp);")+4,"strcat(str, temp);");
    fprintf(archivo, "}\n");
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

uint8_t verificar_funciones(uint8_t * cadena, FILE * archivo_output)
{
    uint16_t len = strlen(cadena);
    uint8_t temp[len+1];
    memset(temp, 0, sizeof(temp));
    uint16_t i = 0;
    uint8_t chars = 0;
    uint8_t espacios = 0;
    uint16_t indice = 0;

    for(i=1;i<len;i++)
    {
        if(isspace(cadena[i]))
        {
            if(chars == 1 && espacios == 0)
                espacios++;
        }
        else
        {
            if(chars == 0)
                chars++;
            else if(chars == 1 && espacios == 1)
            {
                if(strcmp(temp, "raw_input") == 0 || strcmp(temp, "list") == 0)
                {
                    debug("\nError: la definicion de funciones no debe llevar variables\n", linea);
                    return 0;
                }
                else
                    return 1;
            }
            temp[indice++] = cadena[i];
        }
    }

    uint8_t id = 0;
    if(strcmp(temp, "raw_input") == 0)
    {
        id = var_get(temp);
        if(id != 0)
        {
            debug("\nAviso en liea %d: has repetido la declaracion #raw_input mas de una vez, por lo cual se ha omitido. No causa conflictos, pero se recomienda que se elimine\n", linea);
        }
        else
        {   
            id = var_put(temp, RAW_INPUT);
            if(id == 0 || id == 2)
                return 0;
            fprintf(archivo_output, "#include \"include/raw_input.h\"\n");
        }    
    }
    else if(strcmp(temp, "list") == 0)
    {
        id = var_get(temp);
        if(id != 0)
            debug("\nAviso en linea %d: has repetido la declaracion #list mas de una vez, por lo cual se ha omitido. No causa conflictos, pero se recomienda que se elimine\n", linea);
        else
        {
            id = var_put(temp, LIST);
            if(id == 0 || id == 2)
                return 0;
            fprintf(archivo_output, "#include \"include/list.h\"\n");
        }
    }

    return 1;

}

uint8_t buscar_funciones(FILE * archivo, FILE * archivo_output)
{
    uint8_t temp[200];
    memset(temp, 0, sizeof(temp));
    uint16_t len = 0;

    while(fgets(temp, sizeof(temp), archivo) != NULL)
    {
        len = strlen(temp);
        if(temp[len-1] == '\n')
            temp[len-1] = '\0';
        if(temp[0] == '#')
        {
            if(!verificar_funciones(temp, archivo_output))
                return 0;
        }

        memset(temp, 0, sizeof(temp));
    }

    return 1;
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
    else if(indice == 0)
    {
        debug("\nError en linea %d: debes ingresar el nombre de una variable\n", linea);
        return 0;
    }

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

uint8_t * comentario_free(uint8_t * cadena, uint16_t index) //NUEVO
{
    uint16_t len = strlen(cadena);

    uint8_t temp[len+1];
    uint8_t c1[2];
    uint8_t c2[2];
    uint8_t chars = 0;
    uint8_t espacios = 0;
    uint16_t indice = 0;

    memset(temp, 0, sizeof(temp));
    memset(c1, 0, sizeof(c1));
    memset(c2, 0, sizeof(c2));

    uint8_t i = 0;
    for(i=index;i<len;i++)
    {
        if(isspace(cadena[i]))
        {
            if(chars == 1 && espacios == 0)
                espacios++;
        }
        else if(cadena[i] == '(')
        {
            if(c1[0] != '\0')
            {
                debug("\nError en linea %d: Parentesis desbalanceados\n", linea);
                return NULL;
            }
            c1[0] = cadena[i];
        }
        else if(cadena[i] == ')')
        {
            if(c2[0] != '\0')
            {
                debug("\nError en linea %d: Parentesis desbalanceados\n", linea);
                return NULL;
            }
            c2[0] = cadena[i];
        }
        else if(!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != '_' && cadena[i] && cadena[i] != '(' && cadena[i] != ')')
        {
            debug("\nError en linea %d: el compilador encontro un intento de definir una variable en un comentario. Sin embargo, el valor %c no es permitido\n", linea, cadena[i]);
            return NULL;
        }
        else
        {
            if(chars == 0 && espacios == 0)
                chars++;
            else if(espacios == 1)
            {
                debug("\nError en linea %d: el compilador encontro un intento de definir free en un comentario. Sin embargo, free no debe contener espacios\n", linea);
                return NULL;
            }
            temp[indice++] = cadena[i];
        }
    }

    uint16_t formato = strlen(temp)+strlen("free\n=NULL;");
    uint16_t len_formato = formato+1+strlen(c1)+1+strlen(c2)+1+strlen(temp)+1+1+1;
    uint8_t * retorna = malloc(len_formato);
    if(retorna == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }

    uint8_t token = ht_get(temp);
    uint8_t token_comentario = var_get(temp);
    if(temp[0] == '\0')
    {
        debug("\nError en linea %d: falta parametro en \"free\"\n", linea);
        return NULL;
    }
    else if(token == 0)
    {
        debug("\nError en linea %d: la variable no existe: %s\n", linea, temp);
        return NULL;
    }
    else if((token != CADENA && token != LIST) || token_comentario == CHAR)
    {
        debug("\nError en linea %d: la variable no es un puntero: %s\n", linea, temp);
        return NULL;
    }

    if(!free_libera(temp, 0))
        return NULL;

    if(token == LIST)
    {
        retorna = realloc(retorna, len_formato+strlen("destroy_list(->next);")+1);
        if(retorna == NULL)
        {
            debug("\nError: ocurrio un inconveniente en la heap\n");
            return NULL;
        }
        sprintf(retorna, "destroy_list(%s->next);\nfree%s%s%s;\n%s=NULL;",temp,c1,temp,c2,temp);
    }
    else
        sprintf(retorna, "free%s%s%s;\n%s=NULL;",c1,temp,c2,temp);

    return retorna;
}

uint8_t verificar_comentario(uint8_t * cadena, datos_comentarios * datos)
{
    if(datos->token == CHAR)
    {
        if(!comentario_parser(cadena, datos->index, CHAR))
            return 0;
    }
    else if(datos->token == FLOAT)
    {
        if(!comentario_parser(cadena, datos->index, FLOAT))
            return 0;
    }

    return 1;
}


