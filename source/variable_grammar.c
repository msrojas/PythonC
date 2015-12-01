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

uint8_t * get_strcpy_string(lexical * lexer, uint8_t size, uint8_t * var)
{
	uint16_t len = get_len_strcpy(lexer, size, strlen(var));
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
            string = get_cpy_string(lexer->valor, i, var);
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

uint8_t * get_malloc_string(lexical * lexer, uint8_t size)
{
	uint16_t len = get_len_malloc(lexer, size);
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
			string = get_string(lexer->valor, i);
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

uint16_t get_len_strcpy(lexical * lexer, uint8_t size, uint8_t len_var)
{
	uint8_t i=0;
	uint8_t last_token = 0;
	uint16_t len = 0;
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
			len += strlen(lexer->valor);
		}
		last_token = lexer->token;
		lexer = lexer->next;
	}

	return len;
}

uint16_t get_len_malloc(lexical * lexer, uint8_t size)
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
        if((lexer->token == CADENA || lexer->token == VARIABLE) && (last_token == PLUS_CONCAT || last_token == 0 || last_token == CADENA))
        { 
            len += formato;
            if(lexer->token == CADENA)
                checa_y_modifica(lexer->valor);
            len += strlen(lexer->valor);
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
        lexer = lexer->next;      
    }
    return len;
}

uint8_t * print_origanl_variable(data * datos, lexical * lexer, int len_cadena, uint8_t size)
{
    uint8_t * cadena_retorno = NULL;
    uint8_t len_type = 0;
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
            uint8_t token = ht_get(lexer->valor);
            uint8_t token_comentario = var_get(lexer->valor);
            uint16_t bytes = 0;
			
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
                len_cadena = get_len_malloc(lexer, size);
                if(len_cadena == 0)
                    return NULL;
                len_var = get_len_strcpy(lexer, size, len_var);
                if(token == 0)
                { 
                    if(fixed_width == 0)
                        len_type = strlen("char * =malloc();")+len_cadena+len_var+2;
                    else
                        len_type = strlen("int8_t * =malloc();")+len_cadena+len_var+2;
                }
                else
                    len_type = strlen("free();\n=malloc();")+strlen(temp_cadena)+len_cadena+len_var+1;
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
                uint8_t * malloc_string = get_malloc_string(lexer, size);
                if(malloc_string == NULL)
                    return NULL;
                uint8_t * strcpy_string = get_strcpy_string(lexer, size, temp_cadena);
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
                    sprintf(temp, "free(%s);\n%s=malloc(%s);%s", temp_cadena,temp_cadena,malloc_string,strcpy_string);	

                if(free_variables == NULL && free_root == NULL)
                {
                    if(!init_free())
                    {
                        free(malloc_string);
                        free(strcpy_string);
                        return NULL;
                    }
                    free_variables->var_name = NULL;
                    free_root = free_variables;
                }
                if(token == 0)
                { 
                    if(!agregar_free_var(temp_cadena))
                    {
                        free(malloc_string);
                        free(strcpy_string);
                        return NULL;
                    }
                    size_free++;
                }
                free(malloc_string);
                free(strcpy_string);
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
                if(ht_put(temp_cadena, datos->token) == 2)
                {
                    printf("Errno\n");
                    free(cadena_retorno);
                    return NULL;
                }
            }

			//printf("%d : %d\n", strlen(temp), sizeof(temp));
            break;
        }
		/*
		FLOAT y NUMERO se parecen demasiado. En un futuro lo implementare en una sola funcion
		para ahorrar codigo. Algo asi: verificar_numero(lexer, size, token) Donde token puede ser NUMERO o FLOAT. 
		De esa manera podre determinar el formato adecuado
		*/
        case NUMERO:
        { 
            if(datos->declarada == 0)
            {
                if(fixed_width == 0)
                    len_type = strlen("int ;")+1;
                else
                    len_type = strlen("int32_t ;")+1;
            }
            else
                len_type = strlen(";")+1;
            uint16_t len_funciones = get_funciones_extras(lexer,size);
            uint8_t temp[len_cadena+len_type];
            memset(temp, 0, sizeof(temp));

            if(datos->declarada == 0)
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
            for(i=0;i<size;i++)
            {
                if(lexer->token == INT)
                    strcat(temp, "atoi");
                else if(lexer->token == LEN)
                    strcat(temp, "strlen");
                else if(lexer->token == FLOAT_F	)
                    strcat(temp, "atof");
                else
                    strcat(temp, lexer->valor);
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
                if(ht_put(temp_cadena, datos->token) == 2)
                {
                   free(cadena_retorno);
                   return NULL;
                }
            }

			//printf("%d : %d\n", strlen(temp), sizeof(temp));
            break;
        }
        case FLOAT:
        {
            uint8_t token_comentario = var_get(lexer->valor);
            if(token_comentario == 0)
            {
                debug("\nError en linea %d: la variable \"%s\" tiene operacion con decimales. Debes definir la variable en un comentario como float para poder usarla como tal. Ejemplo\n#float nombre_variable\nAgrega esa linea antes de declarar la variable\n",linea, lexer->valor);
                return NULL;
            }
            if(datos->declarada == 0)
                len_type = strlen("float ;")+1;
            else
                len_type = strlen(";")+1;

            uint16_t len_funciones = get_funciones_extras(lexer,size);
            uint8_t temp[len_cadena+len_type];
            memset(temp, 0, sizeof(temp));

            if(datos->declarada == 0)
                sprintf(temp, "float %s", lexer->valor);
            else
                strcat(temp, lexer->valor);

            size -= 1;
            lexer = lexer->next;
            for(i=0;i<size;i++)
            {
                if(lexer->token == INT)
                    strcat(temp, "atoi");
                else if(lexer->token == LEN)
                    strcat(temp, "strlen");
                else if(lexer->token == FLOAT_F	)
                    strcat(temp, "atof");
                else
                    strcat(temp, lexer->valor);
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
                if(ht_put(temp_cadena, datos->token) == 2)
                {
                    free(cadena_retorno);
                    return NULL;
                }
            }	
			//printf("%d : %d\n", strlen(temp), sizeof(temp));
            break;
        }	
        default:
            printf("DESCONOCIDO\n");
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

    data * datos = basic_grammar(lexer, size);

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
