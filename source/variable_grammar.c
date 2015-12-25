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
#include "variable_grammar.h"

uint8_t * get_name(lexical * lexer, uint8_t size, uint8_t * var_name) //NUEVO
{
    lexer = lexer->next;
    lexer = lexer->next;
    uint8_t i=0;
    size -= 2;
    for(i=0;i<size;i++)
    {
        if(strcmp(lexer->valor, var_name) == 0)
        {
            uint8_t * string = ht_get_value(var_name);
            if(string == NULL)
                return NULL;
            return string;
        }
        lexer = lexer->next;
    }

    return NULL;
}

uint8_t * get_char_indice(lexical * lexer) //NUEVO
{
    lexer = lexer->next;
    lexer = lexer->next;
    uint8_t * temp = (uint8_t *)malloc(strlen(lexer->valor)+1);
    if(temp == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }
    strcpy(temp, lexer->valor);
    return temp;
}

uint16_t get_len_indice(lexical * lexer) //NUEVO
{
    lexer = lexer->next;
    lexer = lexer->next;
    uint16_t len = strlen(lexer->valor);
    len += 2;
    return len;
}

uint8_t * get_cpy_indice(lexical * lexer, uint8_t index, uint8_t * var) //NUEVO
{
    uint8_t * get_indice = get_char_indice(lexer);
    if(get_indice == NULL)
        return NULL;
    uint16_t len = strlen("\nstrchar(,);")+strlen(var)+strlen(lexer->valor)+strlen(get_indice)+2;
    uint8_t * temp = (uint8_t *)malloc(len+1);
    if(temp == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }

    sprintf(temp, "\nstrchar(%s,%s[%s]);", var, lexer->valor, get_indice);    
    free(get_indice);
    return temp;
}

uint8_t * get_cpy_string(uint8_t * valor, uint8_t index, uint8_t * var)
{
	uint16_t len = strlen("\nstrcpy(,);")+strlen(var)+strlen(valor);
	uint8_t * temp = (uint8_t *)malloc(len+1);
	if(temp == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return NULL;
	}	
    if(index == 0)
        sprintf(temp, "\nstrcpy(%s,%s);", var, valor);
    else
        sprintf(temp, "\nstrcat(%s,%s);", var, valor);
	//temp[len+1] = '\0';

    return temp;

}

uint8_t * get_cpy_string_value(uint8_t * valor, uint8_t index, uint8_t * var)
{
	uint16_t len = 0;
	uint16_t i = 0;
	uint16_t indice = 0;
	uint16_t len_value = strlen(valor);
    for(i=0;i<len_value;i++)
    {
        if(valor[i] == '\n')
        {
            len += strlen("\nstrcpy(,);")+strlen(var)+indice;
            indice = 0;
        }
        else
            indice++;
    }

	len += strlen("\nstrcpy(,);")+strlen(var)+indice;
	uint8_t * temp = (uint8_t *)malloc(len+1);
	if(temp == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return NULL;
	}

    uint8_t temp_value[len_value+1];
    memset(temp_value, 0, sizeof(temp_value));
	indice = 0;	
	uint8_t validar = 0;
	for(i=0;i<len_value;i++)
	{
		if(valor[i] == '\n')
		{
            if(index == 0)
                sprintf(temp, "\nstrcpy(%s,%s);", var, temp_value);
            else
                sprintf(temp, "\nstrcat(%s,%s);", var, temp_value);
		    indice = 0;
		    validar = 1;
		    memset(temp_value, 0, sizeof(temp_value));
		}
		else
		    temp_value[indice++] = valor[i];
	}
	if(index == 0 && validar == 0)
        sprintf(temp, "\nstrcpy(%s,%s);", var, temp_value);
    else if(index == 0 && validar == 1)
        sprintf(temp, "%s\nstrcat(%s,%s);", temp, var, temp_value);
    else if(index != 0 && validar == 1)
        sprintf(temp, "%s\nstrcat(%s,%s);", temp,var, temp_value);
	else if(index != 0 && validar == 0)
	    sprintf(temp, "\nstrcat(%s,%s);", var, temp_value);
	//temp[len+1] = '\0';
    return temp;

}

uint8_t * get_strcpy_string(lexical * lexer, uint8_t size, uint8_t * var, uint8_t * value)
{
	uint16_t len = get_len_strcpy(lexer, size, var, value);
	uint8_t i=0;
	uint8_t last_token = 0;
	lexer = lexer->next;
	lexer = lexer->next;
	uint8_t * string = NULL;
	uint8_t temp[len+1];
	memset(temp, 0, sizeof(temp));

	size -= 2;
    for(i=0;i<size;i++)
    {
        if((lexer->token == CADENA || lexer->token == VARIABLE) && (last_token == PLUS_CONCAT || last_token == 0 || last_token == CADENA))
        { 
            if(strcmp(lexer->valor, var) == 0 && value != NULL) //NUEVO
                string = get_cpy_string_value(value, i, var);
            else
                string = get_cpy_string(lexer->valor, i, var);
            if(string == NULL)
                return NULL;
            strcat(temp, string);
            free(string);
            string = NULL;
        }
        else if(lexer->token == CHAR_CORCHETES && (last_token == PLUS_CONCAT || last_token == 0  || last_token == CADENA)) //NUEVO
        {   
            string = get_cpy_indice(lexer, i, var);
            if(string == NULL)
                return NULL;
            strcat(temp, string);
            free(string);
            string = NULL;
        }
		last_token = lexer->token;
		lexer = lexer->next;
	}

	uint8_t * retorno = (uint8_t *)malloc(strlen(temp)+1);
	if(retorno == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return NULL;
	}
	strcpy(retorno, temp);
	return retorno;
}

uint8_t * get_string_indice(lexical * lexer, uint8_t index) //NUEVO
{
    uint8_t len = 0;
    if(index == 0)
        len = strlen("1+1");
    else
        len = strlen("+1+1");
    uint8_t * temp = (uint8_t *)malloc(len);
    if(temp == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }
    if(index == 0)
        strcpy(temp, "1+1");
    else
        strcpy(temp, "+1+1");
    return temp;
}

uint8_t * get_string(uint8_t * valor, uint8_t index)
{
	uint16_t len = 0;
	if(index == 0)
		len = strlen("strlen()+1")+strlen(valor);
	else
		len = strlen("+strlen()+1")+strlen(valor);
	uint8_t * temp = (uint8_t *)malloc(len+1);
	if(temp == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return NULL;
	}
	if(index == 0)
		sprintf(temp, "strlen(%s)+1", valor);
	else
		sprintf(temp, "+strlen(%s)+1", valor);
	//temp[len+1] = '\0';
	return temp;
}

uint8_t * get_string_value(uint8_t * valor, uint8_t index)
{
    uint16_t len_value = strlen(valor);
    uint16_t i = 0;
    uint16_t len = 0;
    uint16_t indice = 0;
    for(i=0;i<len_value;i++)
    {
        if(valor[i] == '\n')
        {
            if(index == 0)
                len += strlen("strlen()+1")+indice;
            else
                len += strlen("+strlen()+1")+indice;
            indice = 0;
        }
        else
            indice++;
    }

	if(index == 0 && len == 0)
		len += strlen("strlen()+1")+indice;
	else
		len += strlen("+strlen()+1")+indice;
	uint8_t * temp = (uint8_t *)malloc(len+1);
	if(temp == NULL)
	{
		debug("\nError: ocurrio un inconveniente en la heap\n");
		return NULL;
	}

	uint8_t temp_value[len_value+1];
	indice = 0;
	uint8_t validar = 0;
	memset(temp_value, 0, sizeof(temp_value));
	for(i=0;i<len_value;i++)
	{
        if(valor[i] == '\n')
        {
        	if(index == 0)
		        sprintf(temp, "strlen(%s)+1", temp_value);
	        else
		        sprintf(temp, "+strlen(%s)+1", temp_value);
		    memset(temp_value, 0, sizeof(temp_value));
		    indice = 0;
            validar = 1;
        }
        else
            temp_value[indice++] = valor[i];
	}
	if(index == 0 && validar == 0)
        sprintf(temp, "strlen(%s)+1", temp_value);
    else if(index == 0 && validar == 1)
    	sprintf(temp, "%s+strlen(%s)+1", temp,temp_value);
    else if(index != 0 && validar == 1)
        sprintf(temp, "%s+strlen(%s)+1", temp,temp_value);
    else if(index != 0 && validar == 0)
        sprintf(temp, "+strlen(%s)+1",temp_value);
	//temp[len+1] = '\0';
	return temp;
}

uint8_t * get_malloc_string(lexical * lexer, uint8_t size, uint8_t * var, uint8_t * value)
{
	uint16_t len = get_len_malloc(lexer, size, value, var);
	uint8_t i=0;
	uint8_t last_token = 0;
	lexer = lexer->next;
	lexer = lexer->next;
	uint8_t temp[len+1];
	uint8_t * string = NULL;
	memset(temp, 0, sizeof(temp));

	size -=2;	

	for(i=0;i<size;i++)
	{
		if((lexer->token == CADENA || lexer->token == VARIABLE) && (last_token == PLUS_CONCAT || last_token == 0  || last_token == CADENA))
		{ 
			if(lexer->token == VARIABLE && strcmp(lexer->valor,var) == 0 && var != NULL) //NUEVO
				string = get_string_value(value,i);
			else
				string = get_string(lexer->valor, i);
			if(string == NULL)
				return NULL;
			strcat(temp, string);
			free(string);
            string = NULL;
        }
        else if(lexer->token == CHAR_CORCHETES && (last_token == PLUS_CONCAT || last_token == 0  || last_token == CADENA)) //NUEVO
        { 
            string = get_string_indice(lexer, i);
            if(string == NULL)
                return NULL;
            strcat(temp, string);
            free(string);
            string = NULL;
        }

        last_token = lexer->token;
        lexer = lexer->next;
    }

    uint8_t * retorno = (uint8_t *)malloc(strlen(temp)+1);
    if(retorno == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }
    strcpy(retorno, temp);
    return retorno;
}

uint16_t str_len_cpy(uint8_t * value)
{
    uint16_t len = strlen(value);
    uint8_t formato = strlen("\nstrcpy(,);");
    uint16_t retornar_len = 0;
    uint16_t indice = 0;
    uint16_t i = 0;
    for(i=0;i<len;i++)
    {
        if(value[i] == '\n')
        {
            retornar_len += formato+indice;  
            indice = 0;
        }
        else
            indice++;
    }
    retornar_len += formato+indice;
    return retornar_len;
}


uint16_t get_len_strcpy(lexical * lexer, uint8_t size, uint8_t * var, uint8_t * value) //NUEVO
{
	uint8_t i=0;
	uint8_t last_token = 0;
	uint16_t len = 0;
	uint8_t len_var = strlen(var);
	uint8_t formato = strlen("\nstrcpy(,);");
	lexer = lexer->next;
	lexer = lexer->next;

	size -= 2;
	for(i=0;i<size;i++)
	{
		if((lexer->token == CADENA || lexer->token == VARIABLE) && (last_token == PLUS_CONCAT || last_token == 0 || last_token == CADENA))
		{ 
			len += formato;
			len += len_var;
			if(lexer->token == VARIABLE && strcmp(lexer->valor, var) == 0 && var != NULL) //NUEVO
               len += str_len_cpy(value);
			else
                len += strlen(lexer->valor);
		}
		else if(lexer->token == CHAR_CORCHETES && (last_token == PLUS_CONCAT || last_token == 0 || last_token == CADENA)) //NUEVO
        {
            len += strlen("\nstrchar(,);");
            len += len_var;
            len += strlen(lexer->valor);
            len += get_len_indice(lexer);
        }
		last_token = lexer->token;
		lexer = lexer->next;
	}

	return len;
}

uint16_t str_len_malloc(uint8_t * value)
{
    uint16_t len = strlen(value);
    uint8_t formato = strlen("strlen()+1");
    uint16_t retornar_len = 0;
    uint16_t indice = 0;
    uint16_t i = 0;
    for(i=0;i<len;i++)
    {
        if(value[i] == '\n')
        {
            retornar_len += formato+indice;  
            indice = 0;
        }
        else
            indice++;
    }
    retornar_len += formato+indice;
    return retornar_len;
}

uint16_t get_len_malloc(lexical * lexer, uint8_t size, uint8_t * value, uint8_t * var)
{
	uint8_t i=0;
	uint8_t last_token = 0;
	uint16_t len = 0;
	uint8_t formato = strlen("strlen()+1");
	lexer = lexer->next;
	lexer = lexer->next;

	size -=2;

    for(i=0;i<size;i++)
    {
        if((lexer->token == CADENA || lexer->token == VARIABLE) && (last_token == PLUS_CONCAT || last_token == 0 || last_token == CADENA)) //NUEVO
        { 
            len += formato;
            if(lexer->token == CADENA) //NUEVO
            {
                checa_y_modifica(lexer->valor);
                len += strlen(lexer->valor); 
            }
            else if(lexer->token == VARIABLE && (strcmp(lexer->valor, var) == 0) && var != NULL)
                len += str_len_malloc(value);
            else
                len += strlen(lexer->valor);
        }
        else if(lexer->token == CHAR_CORCHETES && (last_token == PLUS_CONCAT || last_token == 0 || last_token == CADENA)) //NUEVO
        {
            if(i == 0)
                len += strlen("1+1");
            else
                len += strlen("+1+1");
        }
        else if(lexer->token == CONCATENACION)
        {
            debug("\nError en linea %d: aun no se ha implementado almacenar variables con la concatenacion de la \"coma\". Si quieres concatenar cadenas, utiliza el signo \"+\"\n", linea);
            return 0;
        }
        last_token = lexer->token;
        lexer = lexer->next;
    }

    return len;
}

uint8_t get_len_concat(lexical * lexer, uint8_t size) //NUEVO
{
    uint8_t i=0;
    uint8_t len = 0;
    lexer = lexer->next;
    lexer = lexer->next;

    for(i=0;i<size;i++)
    {
        if(lexer->token == PLUS_CONCAT)
            len += 1;
        lexer = lexer->next;
    }
    return len;
}

uint8_t get_number_of_bytes(uint16_t len)
{
    if(len < 10)
        return 1;
    else if(len < 100)
        return 2;
    else if(len < 1000)
        return 3;
    else if(len < 10000)
        return 4;
}

void checa_y_modifica(uint8_t * cadena)
{
    uint16_t len = strlen(cadena);

    if(cadena[0] == '\'')
    { 
        cadena[0] = '\"';
        cadena[len-1] = '\"';
    }
}

uint16_t get_bytes(lexical * lexer, uint8_t size)
{
	lexer = lexer->next;
	lexer= lexer->next;
	size -= 2;
	uint8_t i=0;	
	uint8_t * string = NULL;
	uint16_t len = 0;

	for(i=0;i<size;i++)
	{
		string = remove_quotes(lexer->valor);
		len += strlen(string);
		free(string);
		lexer = lexer->next;
	}

	return (len +=1);
}

uint16_t get_funciones_extras(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    uint16_t len = 0;
    for(i=0;i<size;i++)
    {
        if(lexer->token == LEN)
            len += 3;
        else if(lexer->token == INT || lexer->token == FLOAT_F)
            len += 1;
        else if(lexer->token == VARIABLE)
        {
            uint8_t token = ht_get(lexer->valor);
            if(token == LIST)
            {
                uint8_t validar = is_corchetes(lexer);
                if(validar == 0)
                {
                    uint8_t * indice = get_list_indice(lexer);
                    if(indice == NULL)
                        return 0;
                    len += strlen("get_indice_en_especifico_ttype(->,)")+strlen(indice)+strlen(lexer->valor);
                    free(indice);
                }
            }
        }
        lexer = lexer->next;      
    }
    return len;
}

uint16_t get_len_parametros(lexical * lexer, uint8_t size) //nuevo
{
    uint8_t i=0;
    uint16_t len = 0;
    lexer = lexer->next;
    lexer = lexer->next;
    lexer = lexer->next;
    size -= 3;

    for(i=0;i<size;i++)
    {
        if(lexer->token == NUMERO) 
            len += 5; //INT y ','
        else if(lexer->token == CADENA)
            len += 6; //CHAR y ','
        else if(lexer->token == FLOAT)
            len += 7; //FLOAT y ','
        lexer = lexer->next;
    }

    return len;
}

uint8_t get_numero_elemento(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    uint8_t len = 0;
    uint8_t last_token = 0;
    lexer = lexer->next;
    lexer = lexer->next;
    lexer = lexer->next;
    size -= 3;

    for(i=0;i<size;i++)
    {
        if(lexer->token == CONCATENACION)
        	len++;
        last_token = lexer->token;
        lexer = lexer->next;
    }

    return (len+1);
}

uint8_t * add_elemento(lexical * lexer)
{
    uint8_t * string = NULL;
    uint8_t temp[strlen(lexer->valor)+8];
   // printf("%sa\n", lexer->valor);

    switch(lexer->token)
    {
        case NUMERO:
            sprintf(temp, ",INT,%s", lexer->valor);
            break;
        case CADENA:
            sprintf(temp, ",CHAR,%s",lexer->valor);
            break;
        case FLOAT:
            sprintf(temp, ",FLOAT,%s",lexer->valor);
            break;
        default:
            printf("DESCONOCIDO we\n");
            break;        
    }

    string = malloc(strlen(temp)+1);
    if(string == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }
    strcpy(string, temp);

    return string;
}

uint8_t * print_origanl_variable(data * datos, lexical * lexer, int len_cadena, uint8_t size)
{
    uint8_t * cadena_retorno = NULL;
    uint16_t len_type = 0;
    uint8_t i = 0;
    uint8_t token = ht_get(lexer->valor);
    uint8_t len_var = strlen(lexer->valor);
    uint8_t temp_cadena[len_var+1];
    memset(temp_cadena, 0, sizeof(temp_cadena));
    strcpy(temp_cadena, lexer->valor);

    switch(datos->token)
    {
        case CADENA:
        { 
        	uint8_t * value = NULL; //NUEVO
            uint8_t token = ht_get(lexer->valor);
            uint8_t token_comentario = var_get(lexer->valor);
            uint8_t token_free = 0; //NUEVO
            uint16_t bytes = 0;
            uint8_t * validar = NULL; //NUEVO
			
            if(token == 0 && datos->declarada == 0 && token_comentario == CHAR)
            { 
                bytes = get_bytes(lexer, size);
                uint8_t len_bytes = get_number_of_bytes(datos->len);
                uint16_t resto = (len_cadena > datos->len) ? len_cadena - datos->len : datos->len - len_cadena;
                if(fixed_width == 0)
                    len_type = strlen("char [];")+len_bytes+resto+1;
                else
                    len_type = strlen("int8_t [];")+len_bytes+resto+1;
            }
            else if(token == 0 && datos->declarada == 1 && token_comentario == CHAR)
            {
                debug("\nError en linea %d: la variable \"%s\" la definiste como char en un comentario. Por lo tanto, no puedes asignarle variables. Si lo quieres hacer, borra el comentario donde la definiste para que el compilador la trate como puntero\n", linea,lexer->valor);
                return NULL;
            }
            else if((token == 0 || token != 0) && (datos->declarada == 1 || datos->declarada == 0) && token_comentario == 0)
            {
            	token_free = free_get(temp_cadena);
                uint8_t ret = check_sub_indices(lexer, size); //NUEVO
                uint8_t len_concatenaciones = get_len_concat(lexer, size); //NUEVO
                if(ret == 0)
                    return NULL;
                if(token == 0)
                { 
                	len_cadena = get_len_malloc(lexer, size, NULL, temp_cadena);
                    if(len_cadena == 0)
                        return NULL;
                    len_var = get_len_strcpy(lexer, size, temp_cadena, NULL);
                    if(fixed_width == 0)
                        len_type = strlen("char * =malloc();")+len_cadena+len_var+len_concatenaciones+2;
                    else
                        len_type = strlen("int8_t * =malloc();")+len_cadena+len_var+len_concatenaciones+2;
                }
                else
                {
                	validar = get_name(lexer, size, temp_cadena);
                    if(validar == NULL)
                    {
                    	len_cadena = get_len_malloc(lexer, size, NULL, temp_cadena);
                        if(len_cadena == 0)
                            return NULL;
                        len_var = get_len_strcpy(lexer, size, temp_cadena, NULL);
                    	if(token_free == 1) //NUEVO
                            len_type = strlen("free();\n=NULL;\n=malloc();")+strlen(temp_cadena)+strlen(temp_cadena)+len_cadena+len_var+len_concatenaciones+2;
                        else
                        	len_type = strlen("=malloc();")+len_cadena+len_var+len_concatenaciones+2;
                    }
                    else
                    {
                    	len_cadena = get_len_malloc(lexer, size, validar, temp_cadena);
                        if(len_cadena == 0)
                            return NULL;
                        len_var = get_len_strcpy(lexer, size, temp_cadena, validar);
                    	if(token_free == 0) //NUEVO
                    	{
                    		debug("\nError en linea %d: la variable \"%s\" no se pude concatenar ya que tiene un valor NULO\n", linea, temp_cadena);
                    	    return NULL;
                    	}
                        len_type = strlen("=realloc();")+strlen(temp_cadena)+len_cadena+len_var+len_concatenaciones+2;
                    }
                }
                len_cadena = strlen(temp_cadena)+1;
            }
            else if(token != 0 && (datos->declarada == 1 || datos->declarada == 0) && token_comentario == CHAR)
            {
				debug("\nError en linea %d: la variable \"%s\" fue definida como char en un comentario, por lo cual, no puedes redefinirla. Si lo quieres hacer, borra el comentario donde la definiste para que el compilador la trate como puntero\n", linea, lexer->valor);
				return NULL;
            }
            uint8_t temp[len_cadena+len_type];
            memset(temp, 0, sizeof(temp));
            if(token == 0 && datos->declarada == 0 && token_comentario == CHAR)
            {
                if(fixed_width == 0)
                    sprintf(temp, "char %s[%d]", lexer->valor, bytes);
                else
                    sprintf(temp, "int8_t %s[%d]", lexer->valor, bytes);
            }
            else if((token == 0 || token != 0) && (datos->declarada == 1 || datos->declarada == 0) && token_comentario == 0)
            {
                uint8_t * malloc_string = get_malloc_string(lexer, size, temp_cadena, validar);
                if(malloc_string == NULL)
                    return NULL;
                uint8_t * strcpy_string = get_strcpy_string(lexer, size, temp_cadena, validar);
                if(strcpy_string == NULL)
                {
                    free(malloc_string);
                    return NULL;
                }
                if(token == 0)
                {
                    if(fixed_width == 0)
                        sprintf(temp, "char * %s=malloc(%s);%s",temp_cadena,malloc_string,strcpy_string);
                    else
                        sprintf(temp, "int8_t * %s=malloc(%s);%s",temp_cadena,malloc_string,strcpy_string);
                }
                else
                {
                    if(validar == NULL)
                    {
                        if(token_free == 1)
                            sprintf(temp, "free(%s);\n%s=NULL;\n%s=malloc(%s);%s", temp_cadena,temp_cadena,temp_cadena,malloc_string,strcpy_string);	
                        else
                            sprintf(temp, "%s=malloc(%s);%s", temp_cadena,malloc_string,strcpy_string);
                    }
                    else
                        sprintf(temp, "%s=realloc(%s,%s);%s", temp_cadena,temp_cadena,malloc_string,strcpy_string);
                }

                if(token == 0) //NUEVO
                { 
                	uint8_t ret_val = free_put(temp_cadena, 1, CADENA);
                    if(ret_val == 0 || ret_val == 2)
					{
						free(malloc_string);
						free(strcpy_string);
						return NULL;
                    }
                   
                }
                if(token != 0) //NUEVO
                {
                    if(token_free == 0)
                    {
                        if(!free_libera(temp_cadena, 1))
                        {
                            free(malloc_string);
                            free(strcpy_string);
                            return NULL;
                        }
                    }
                }
                free(malloc_string);
                free(strcpy_string);
                if(validar != NULL)
                    free(validar);
                value = agregar_valor(lexer, size);
                if(value == NULL)
                    return NULL;
            }
            size -= 1;

            if(token == 0 && datos->declarada == 0 && token_comentario == CHAR)
            { 
                lexer = lexer->next;
                for(i=0;i<size;i++)
                {
                    checa_y_modifica(lexer->valor);
                    strcat(temp, lexer->valor);
                    lexer = lexer->next;
                }
                sprintf(temp, "%s;", temp);
            }

            cadena_retorno = (uint8_t *)malloc(strlen(temp)+1);
            if(cadena_retorno == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }
            strcpy(cadena_retorno, temp);

            if(token == 0)
            {
				if(ht_put(temp_cadena, datos->token, ZERO,value) == 2) //nuevo
				{
					printf("Errno\n");
					free(cadena_retorno);
					return NULL;
				}
            }
            else if(token != 0 && value != NULL) //NUEVO
            {
                if(!update_valor(temp_cadena, value))
                {
                    free(cadena_retorno);
                    return NULL;
                }
            }
            if(value != NULL)
                free(value);
			//printf("%d : %d : %d\n", strlen(temp), sizeof(temp), linea);
            break;
        }
		/*
		FLOAT y NUMERO se parecen demasiado. En un futuro lo implementare en una sola funcion
		para ahorrar codigo. Algo asi: verificar_numero(lexer, size, token) Donde token puede ser NUMERO o FLOAT. 
		De esa manera podre determinar el formato adecuado
		*/
        case NUMERO:
        { 
        	uint8_t token_var = ht_get(lexer->valor);
            if(token_var == 0)
            {
                if(fixed_width == 0)
                    len_type = strlen("int ;")+1;
                else
                    len_type = strlen("int32_t ;")+1;
            }
            else
                len_type = strlen(";")+1;
            uint16_t len_funciones = get_funciones_extras(lexer,size);
            if(!buscar_lista_elemento(lexer,size))
                return NULL;
            uint8_t temp[len_cadena+len_type+len_funciones+1];
            memset(temp, 0, sizeof(temp));

            if(token_var == 0)
            {
                if(fixed_width == 0)
                    sprintf(temp, "int %s", lexer->valor);
                else
                    sprintf(temp, "int32_t %s", lexer->valor);
            }
            else
                strcat(temp, lexer->valor);

            size -= 1;

            lexer = lexer->next;
            uint8_t temp_token = 0;
            uint8_t last_token = 0;
            for(i=0;i<size;i++)
            {
                if(last_token == R_CORCHETES && temp_token != 0)
                    temp_token = 0;
                if(lexer->token == INT)
                    strcat(temp, "atoi");
                else if(lexer->token == LEN)
                    strcat(temp, "strlen");
                else if(lexer->token == FLOAT_F	)
                    strcat(temp, "atof");
                else if(lexer->token == GET_INDICE_INT)
                {
                    strcat(temp, "get_indice_en_especifico_int(");
                    if(!concatena_resultado(lexer, temp))
                        return NULL;
                    temp_token = GET_INDICE_INT;    	
                }
                else
                {
                	if(temp_token == 0)
                        strcat(temp, lexer->valor);
                }
                last_token = lexer->token;
                lexer = lexer->next;
            }

            if(datos->operacion_secundaria == OPE_DE_PARENTESIS)
            {
				if(!check_balanced(temp, VARIABLE))
				{
					debug("\nError en linea %d los parentesis no estan balanceados\n", linea);
					return NULL;
				}
            }
			
            sprintf(temp, "%s;", temp);
            cadena_retorno = (uint8_t *)malloc(strlen(temp)+1);
            if(cadena_retorno == NULL)
            {
				debug("\nError: ocurrio un inconveniente en la heap\n");
				return NULL;
            }
            strcpy(cadena_retorno, temp);

            if(token == 0)
            {
				if(ht_put(temp_cadena, datos->token, ZERO,NULL) == 2)
				{
					free(cadena_retorno);
					return NULL;
				}
            }
            else if(token != 0)
            {
                uint8_t token_function = get_token_function(temp_cadena);
                if(token_function == FOR)
                {
                    if(!update_token_function(temp_cadena,ZERO))
                    {
                        free(cadena_retorno);
                        return NULL;
                    }
                }	
            }

			//printf("%d : %d : %d\n", strlen(temp), sizeof(temp), linea);
            break;
        }
        case FLOAT:
        {
            uint8_t token_comentario = var_get(lexer->valor);
            uint8_t token_var = ht_get(lexer->valor);
            if(token_comentario == 0)
            {
				debug("\nError en linea %d: la variable \"%s\" tiene operacion con decimales. Debes definir la variable en un comentario como float para poder usarla como tal. Ejemplo\n#float nombre_variable\nAgrega esa linea antes de declarar la variable\n",linea, lexer->valor);
				return NULL;
            }
            if(token_var == 0)
                len_type = strlen("float ;")+1;
            else
                len_type = strlen(";")+1;

            uint16_t len_funciones = get_funciones_extras(lexer,size);
            if(!buscar_lista_elemento(lexer,size))
                return NULL;
            uint8_t temp[len_cadena+len_type+len_funciones+1];
            memset(temp, 0, sizeof(temp));

            if(token_var == 0)
                sprintf(temp, "float %s", lexer->valor);
            else
                strcat(temp, lexer->valor);

            size -= 1;
            lexer = lexer->next;
            uint8_t temp_token = 0;
            uint8_t last_token = 0;
            for(i=0;i<size;i++)
            {
                if(last_token == R_CORCHETES && temp_token != 0)
                    temp_token = 0;
                if(lexer->token == INT)
                    strcat(temp, "atoi");
                else if(lexer->token == LEN)
                    strcat(temp, "strlen");
                else if(lexer->token == FLOAT_F	)
                    strcat(temp, "atof");
                else if(lexer->token == GET_INDICE_INT)
                {
                    strcat(temp, "get_indice_en_especifico_int(");
                    if(!concatena_resultado(lexer, temp))
                        return NULL;
                    temp_token = GET_INDICE_INT;    	
                }
                else if(lexer->token == GET_INDICE_FLOAT)
                {
                    strcat(temp, "get_indice_en_especifico_float(");
                    if(!concatena_resultado(lexer, temp))
                        return NULL;
                    temp_token = GET_INDICE_FLOAT;    	
                }
                else
                {
                	if(temp_token == 0)
                        strcat(temp, lexer->valor);
                }
                last_token = lexer->token;
                lexer = lexer->next;
            }

            if(datos->operacion_secundaria == OPE_DE_PARENTESIS)
            {
                if(!check_balanced(temp, VARIABLE))
                {
                    debug("\nError en linea %d los parentesis no estan balanceados\n", linea);
                    return NULL;
                }
            }

            sprintf(temp, "%s;", temp);
            cadena_retorno = (uint8_t *)malloc(strlen(temp)+1);
            if(cadena_retorno == NULL)
            {
                debug("\nError en linea %d: ocurrio un inconveniente en la heap\n", linea);
                return NULL;
            }
            strcpy(cadena_retorno, temp);

            if(token == 0)
            {
                if(ht_put(temp_cadena, datos->token, ZERO,NULL) == 2)
                {
                    free(cadena_retorno);
                    return NULL;
                }
            }	
			//printf("%d : %d : %d\n", strlen(temp), sizeof(temp), linea);
            break;
        }
        case LIST:
        {
        	//printf("list\n");
            uint8_t token_comentario = var_get(lexer->valor);
            uint8_t token_var = ht_get(lexer->valor);
            if(token_comentario != 0)
            {
                debug("\nError en linea %d: la variable fue declarada en un comentario como char o float\n",linea);
                return NULL;
            }
            uint16_t len_extra = get_len_parametros(lexer, size);
            uint16_t numero_elemento = get_numero_elemento(lexer, size);
            uint8_t bytes = get_number_of_bytes(numero_elemento)+1; 
            uint16_t formato_init = strlen("struct list * =init_list();\n")+len_var;
            if(token_var == 0)
            	len_type = formato_init+strlen("agregar_elementos();")+len_var+bytes+len_extra+1;
            else
            {
                debug("\nError en linea %d: la lista ya fue declarada. No puedes redefinirla\n", linea);
                return NULL;
            }

            uint8_t temp[len_cadena+len_type+1];
            memset(temp, 0, sizeof(temp));

            sprintf(temp, "struct list * %s=init_list();\nagregar_elementos", lexer->valor);
            lexer = lexer->next;
            lexer = lexer->next;
            if(lexer->token == L_CORCHETES)
                strcat(temp, "(");
            else
            {
                debug("\nError en linea %d: falta \"[\"\n", linea);
                return NULL;
            }
            strcat(temp, temp_cadena);
            char string_numero_elemento[bytes+2];
            sprintf(string_numero_elemento, ",%d", numero_elemento);
            strcat(temp, string_numero_elemento);
            lexer = lexer->next;

            size -= 3;
            uint8_t * string = NULL;
            uint8_t last_token = 0;
            lista_tokens * token_lista = NULL;
            token_lista = init_token_lista();

            for(i=0;i<size;i++)
            {
                if(lexer->token == R_CORCHETES)
                    strcat(temp, ")");
                else
                {
                	if((lexer->token == NUMERO || lexer->token == FLOAT || lexer->token == VARIABLE || lexer->token == CADENA) && (last_token == 0 || last_token == CONCATENACION))
                    { 
                        string = add_elemento(lexer);
                        strcat(temp, string);
                        free(string);
                        string = NULL;
                        if(!InsertTokenList(token_lista, lexer->token))
                            return NULL;
                    }
                }
                last_token = lexer->token;
                lexer = lexer->next;
            }
            strcat(temp, ";");
            cadena_retorno = malloc(strlen(temp)+1);
            if(cadena_retorno == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }
            strcpy(cadena_retorno, temp);

            if(token == 0)
            {
                if(ht_put(temp_cadena, datos->token, ZERO,NULL) == 2)
                {
                    free(cadena_retorno);
                    return NULL;
                }
                uint8_t ret_val = free_put(temp_cadena, 1, LIST);
                if(ret_val == 0 || ret_val == 2)
			    {
				    free(cadena_retorno);
					return NULL;
                }
            }
            if(!agregar_list_token(temp_cadena, token_lista->next))
            	return NULL;

            delete_list(token_lista->next);
            free(token_lista);

            break;
        }    	
        default:
            printf("DESCONOCIDO...\n");
            break;	
    }

    return cadena_retorno;
}

uint8_t check_int_o_float(lexical * lexer, uint8_t size)
{
    if(size == 1 || size == 2)
        return 0;
    lexer = lexer->next;
    lexer = lexer->next;
    if(lexer->token == RAW_INPUT)
        return 1;
    else
        return 0;
}

data * check_variable_grammar(lexical * lexer, uint8_t size)
{
    uint8_t token = ht_get(lexer->valor);
    uint8_t token_comentario = var_get(lexer->valor);
    uint8_t temp[strlen(lexer->valor)+1];
    memset(temp, 0, sizeof(temp));
    strcpy(temp, lexer->valor);

    lexer = lexer->next;

    if(lexer->token != IGUAL)
    {
		debug("\nError en linea %d: no hay signo igual:  %s\n", linea,lexer->valor);
		return NULL;
    }

    lexer = lexer->next;
    size -= 2;

    if(size == 0)
    {
		debug("\nError en linea %d: falta valor\n", linea);
		return NULL;
    }

    if(lexer->token == RAW_INPUT)
    {
        data * datos = (data *)malloc(sizeof(data));
        if(datos == NULL)
        {
            debug("\nError: ocurrio un inconveniente en la heap\n");
            return NULL;
        }
        datos->declarada = RAW_INPUT;    
        return datos;
    }
    else if(lexer->token == INT || lexer->token == FLOAT_F)
    {
        if(check_int_o_float(lexer, size))
        {
            data * datos = (data *)malloc(sizeof(data));
            if(datos == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }
            if(lexer->token == INT)
                datos->declarada = INT;
            else
                datos->declarada = FLOAT_F;
            return datos;
        }
    	
    }

    data * datos = basic_grammar(lexer, size, VARIABLE);

    if(datos == NULL)
        return NULL;
    else if(token != 0)
    {
        if((datos->token == NUMERO && token == CADENA) || (datos->token == CADENA && token == NUMERO) || (datos->token == FLOAT && token == CADENA) || (datos->token == CADENA && token == FLOAT))
        {
            debug("\nError en linea %d: la variable \"%s\" ", linea,temp);
            switch(token)
            {
                case CADENA:
                    debug("es un tipo cadena, pero esta siendo reemplazada con un tipo distinto\n");
                    break;
                case NUMERO:
                    debug("es un tipo numero, pero esta siendo reemplazada con un tipo distinto\n");
                    break;
                case FLOAT:
                    debug("es un tipo decimal, pero esta siendo reemplazada con un tipo distinto\n");
                    break;    
                default:
                    debug("no se conoce (posible bug. Favor de avisar a la pagina)\n");	
                    break;	
            }
            free(datos);
            datos = NULL;
            return NULL;
        }
        datos->declarada = 1;
    }
    else if(token == 0 && token_comentario != 0)
    {
        if((datos->token == NUMERO || datos->token == FLOAT) && token_comentario == CHAR)
        {
            debug("\nError en linea %d: definiste la variable \"%s\" como char en un comentario, pero dicha variable la has declarado como numerica\n", linea, temp);
            free(datos);
            datos = NULL;
            return NULL;
        }
        else if(datos->token == CADENA && token_comentario == FLOAT)
        {
            debug("\nError en linea %d: definiste la variable \"%s\" como float en un comentario, pero dicha variable la has declarado como char\n", linea, temp);
            free(datos);
            datos = NULL;
            return NULL;
        }
        else if(datos->token == NUMERO && token_comentario == FLOAT)
            datos->token = FLOAT;
    }

    return datos;
}
