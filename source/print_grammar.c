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
#include "print_grammar.h"

uint8_t is_corchetes(lexical * lexer)
{
    lexer = lexer->next;
    if(lexer->token == L_CORCHETES)
        return 0;
    else
        return 1;
}    

uint8_t * get_list_indice(lexical * lexer, uint8_t size)
{
    lexer = lexer->next;
    lexer = lexer->next;
    uint8_t * indice = NULL;
    if(lexer->token == NUMERO)
    {
        indice = malloc(strlen(lexer->valor)+1);
        if(indice == NULL)
        {
            debug("\nError: ocurrio un inconveniente en la heap\n");
            return NULL;
        }
        strcpy(indice, lexer->valor);
    }
    else if(lexer->token == VARIABLE)
    {
        uint8_t token = ht_get(lexer->valor);
        if(token == NUMERO)
        {
            indice = malloc(strlen(lexer->valor)+1);
            if(indice == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }
            strcpy(indice, lexer->valor);
        }
        else
        {
            debug("\nError en linea %d: el indice debe ser un valor numerico: %s\n", linea, lexer->valor);
            return NULL;
        }
    }
    else
    {
        debug("\nError en linea %d: el indice debe ser un valor numerico: %s\n", linea, lexer->valor);
        return NULL;
    }

    return indice;
}    

uint8_t is_only_list(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    for(i=0;i<size;i++)
    {
        if(lexer->token == NUMERO || lexer->token == FLOAT || lexer->token == CADENA || lexer->token == INT || lexer->token == FLOAT_F || lexer->token == LEN)
        {
            debug("\nError en linea %d: lamentablemente las listas en C no funcionan igual que en python. Python es un lenguaje interpretado, por lo tanto, es mas facil destruir variables y reemplazar valores. Pero C es un compilador y un compilador requiere una gramatica estricta\n\nNo puedes concatenar listas con otros valores\n", linea);
            return 0;
        }
        else if(lexer->token == CONCATENACION || lexer->token == PLUS_CONCAT)
        {
            debug("\nError en linea %d: lamentablemente las listas en C no funcionan igual que en python. Python es un lenguaje interpretado, por lo tanto, es mas facil destruir variables y reemplazar valores. Pero C es un compilador y un compilador requiere una gramatica estricta\n\nNo puedes concatenar listas con otros valores\n", linea);
            return 0;
        }
        else if(lexer->token == VARIABLE)
        {
        	uint8_t token = ht_get(lexer->valor);
        	uint8_t token_for = get_token_function(lexer->valor);
        	if(token == LIST && token_for != FOR)
        	{
                lexer = lexer->next;
                if(lexer->token != L_CORCHETES)
                {
                    debug("\nError en linea %d: falta \"[\"\n", linea);
                    return 0;
                }
                lexer = lexer->next;
                lexer = lexer->next;
                if(lexer->token != R_CORCHETES)
                {
                    debug("\nError en linea %d: falta \"]\"\n", linea);
                    return 0;
                }
            }
            else if(token == LIST && token_for == FOR)
            {
            	lexer = lexer->next;
                if(lexer->token == L_CORCHETES)
                {
                    debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
                    return 0;
                }
            }
            else
            {
                debug("\nError en linea %d: lamentablemente las listas en C no funcionan igual que en python. Python es un lenguaje interpretado, por lo tanto, es mas facil destruir variables y reemplazar valores. Pero C es un compilador y un compilador requiere una gramatica estricta\n\nNo puedes concatenar listas con otros valores\n", linea);
                return 0;
            }
        }
        lexer = lexer->next;
    }

    return 1;
}    

uint8_t check_variable_for(lexical * lexer, uint8_t size)
{
    uint8_t len = 0;
    uint8_t i=0;
    uint8_t token_for = 0;
    uint8_t token = 0;
    for(i=0;i<size;i++)
    {
        if(lexer->token == VARIABLE)
        {
            token = ht_get(lexer->valor);
            token_for = get_token_function(lexer->valor);
            if(token == CADENA && token_for == FOR)
                len += 1;
        }

        lexer = lexer->next;
    }

    return len;
}

uint8_t buscar_corchetes(lexical * lexer, uint8_t index, uint8_t size) //NUEVO
{
    uint8_t i=0;
    uint8_t ret = 0;
    uint8_t last_token = 0;
    uint8_t l_corchete = 0;
    uint8_t r_corchete = 0;
    uint8_t indice = 0;
    for(i=index;i<size;i++)
    {
        if(l_corchete == 1 && indice == 1 && r_corchete == 1)
        {
            ret++;
            return ret;
        }
        else if(lexer->token == L_CORCHETES && l_corchete == 0)
            l_corchete++;
        else if(lexer->token == R_CORCHETES && r_corchete == 0)
            r_corchete++;
        else if((lexer->token == NUMERO || lexer->token == VARIABLE) && last_token == L_CORCHETES)
        {
            if(lexer->token == VARIABLE)
            {
                uint8_t token = ht_get(lexer->valor);
                if(token == NUMERO)
                    indice++;
                else
                {
                    debug("\nError en linea %d: el indice no es un numero\n", linea);
                    return 2;
                }
            }
            else 
                indice++;
        }
        else if((lexer->token != NUMERO || lexer->token != VARIABLE) && last_token == L_CORCHETES)
        {
            debug("\nError en linea %d: el indice no es numero\n", linea);
            return 2;
        }
        else if((lexer->token == L_CORCHETES && last_token == L_CORCHETES) || (lexer->token == R_CORCHETES && last_token == R_CORCHETES)) //NUEVO
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 2;
        }
        else if((last_token == NUMERO || last_token == VARIABLE) && (indice == 1 && lexer->token != R_CORCHETES))
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 2;
        }
        else if(lexer->token == CONCATENACION || lexer->token == PLUS_CONCAT)
            break;

        last_token = lexer->token;
        lexer = lexer->next;	
    }

    if(l_corchete == 1 && indice == 1 && r_corchete == 1)
    {
        ret++;
    }

    return ret;
}    

uint8_t check_sub_indices(lexical * lexer, uint8_t size) //NUEVO
{
    uint8_t i=0;
    uint8_t ret = 0;
    uint8_t token = 0;
    uint8_t token_for = 0;
    for(i=0;i<size;i++)
    {
        if(lexer->token == VARIABLE)
        {
            token = ht_get(lexer->valor);
            token_for = get_token_function(lexer->valor);
            if(token == CADENA && token_for != FOR)
            {
                ret = buscar_corchetes(lexer,i,size);
                if(ret == 1)
                    lexer->token = CHAR_CORCHETES;
                else if(ret == 2)
                    return 0;     
            }
        }
        lexer = lexer->next;
    }

    return 1;
}

uint8_t StrCat(lexical * lexer, uint8_t * temp)
{
    uint8_t token = 0;
    uint8_t token_for = 0;
    if(lexer->token == NUMERO || lexer->token == INT)
        strcat(temp, "%d");
    else if(lexer->token == FLOAT || lexer->token == FLOAT_F)
        strcat(temp, "%f");
    else if(lexer->token == CADENA)
        strcat(temp, "%s");
    else if(lexer->token == FLOAT)
        strcat(temp, "%f");
    else if(lexer->token == CHAR_CORCHETES)
        strcat(temp, "%c");
    else if(lexer->token == VARIABLE)
    {
        token = ht_get(lexer->valor);
        token_for = get_token_function(lexer->valor);
        if(token == NUMERO)
            strcat(temp, "%d");
        else if(token == FLOAT)
            strcat(temp, "%f");
        else if(token == CADENA && token_for != FOR)
            strcat(temp, "%s");
        else if(token == CADENA && token_for == FOR)
            strcat(temp, "%c");
        else if(token == FLOAT)
            strcat(temp, "%f");
    }
}

uint8_t * get_formato(uint8_t size, lexical * lexer, uint8_t contador)
{
    uint8_t i=0;
    short formato = strlen("printf(\"%x\\n\",);")+(contador+contador)+1;
    uint8_t temp[formato];
    uint8_t token = 0;
    uint8_t token_for = 0;
    uint8_t last_token = 0;
    uint8_t parentesis = 0;
    memset(temp, 0, sizeof(temp));
	
    for(i=0;i<size;i++)
	{
        if(i > 0 && parentesis == 0)
        {
            if(last_token == CONCATENACION)
            {
                if(lexer->token != L_PARENTESIS && lexer->token != R_PARENTESIS)
                    strcat(temp, " ");
                StrCat(lexer, temp);
            }
            else if(last_token == PLUS_CONCAT)
                StrCat(lexer, temp);
        }
        else if(i == 0 || parentesis == 1)
        {
            if(lexer->token == NUMERO || lexer->token == INT)
                strcat(temp, "printf(\"%d");
            else if(lexer->token == CADENA)
                strcat(temp, "printf(\"%s");
            else if(lexer->token == FLOAT || lexer->token == FLOAT_F)
                strcat(temp, "printf(\"%f");
            else if(lexer->token == CHAR_CORCHETES)
                strcat(temp, "printf(\"%c");    		
            else if(lexer->token == VARIABLE)
            {
                token = ht_get(lexer->valor);
                token_for = get_token_function(lexer->valor);
                if(token == NUMERO)
                    strcat(temp, "printf(\"%d");
                else if(token == CADENA && token_for != FOR)
                    strcat(temp, "printf(\"%s");
                else if(token == CADENA && token_for == FOR)
                    strcat(temp, "printf(\"%c");    	
                else if(token == FLOAT)
                    strcat(temp, "printf(\"%f");
                token = 0;		
            }
            else if(lexer->token == L_PARENTESIS)
            {
                if(parentesis == 0)
                    parentesis++;
            }
            if(temp[0] != '\0')
                parentesis = 0;		
        }
        if(lexer->token != L_PARENTESIS && lexer->token != R_PARENTESIS)
            last_token = lexer->token;
        lexer = lexer->next;
    }
    strcat(temp, "\\n\",");
    uint8_t * char_formato = (uint8_t *)malloc(strlen(temp)+1);
    strcpy(char_formato, temp);

    return char_formato;	
}

uint8_t buscar_float(lexical * lexer, uint8_t index, uint8_t size)
{
    uint8_t i=0;
    uint8_t ret = 0;
    uint8_t token = 0;
    for(i=index;i<size;i++)
    {
        if(lexer->token == CADENA || lexer->token == CONCATENACION || lexer->token == VARIABLE)
        {
            if(lexer->token == VARIABLE)
            {
                token = ht_get(lexer->valor);
                if(token == FLOAT)
                {
                    ret++;
                    break;
                }
                else if(token == CADENA)
                    break;
            }
            else if(lexer->token == CADENA || lexer->token == CONCATENACION)
                break;	
        }
        else if(lexer->token == FLOAT || lexer->token == FLOAT_F)
        {
            ret++;
            break;
        } 
        lexer = lexer->next;
    }

    return ret;
}

void check_float(uint8_t size, lexical * lexer)
{
    uint8_t i=0;
    uint8_t token = 0;
    uint8_t ret = 0;
    for(i=0;i<size;i++)
    {
        if(lexer->token == NUMERO && ret == 0)
        {
            ret = buscar_float(lexer,i,size);
            if(ret == 1)
                lexer->token = FLOAT;
        }
        else if(lexer->token == VARIABLE && ret == 0)
        {
            token = ht_get(lexer->valor);
            if(token == NUMERO)
            {
                ret = buscar_float(lexer,i,size);
                if(ret == 1)
                    lexer->token = FLOAT;
            }
        }
        else if((lexer->token == CADENA || lexer->token == CONCATENACION || lexer->token == VARIABLE) && ret == 1)
        {
            if(lexer->token == VARIABLE)
            {
                token = ht_get(lexer->valor);
                if(token != NUMERO)
                    ret = 0;
            }
            else
                ret = 0;
        }
        lexer = lexer->next;
    }
}

uint8_t check_concatenacion(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    uint8_t contador = 0;

    for(i=0;i<size;i++)
    {
        if(lexer->token == CONCATENACION || lexer->token == PLUS_CONCAT)
            contador++;
        lexer = lexer->next;
    }

    return contador;
}

uint16_t get_len_numbers(lexical * lexer, uint8_t size)
{
	uint8_t i = 0;
	uint16_t len = 0;

    for(i=0;i<size;i++)
    {
        if(lexer->token == LEN)
            len += 3;
        else if(lexer->token == INT || lexer->token == FLOAT_F) //NUEVO
            len += 1;
        len += strlen(lexer->valor); 
        lexer = lexer->next;
    }

	return (len+1);
}

uint8_t * print_original_value(lexical * lexer, uint8_t size, data * datos)
{
    lexer = lexer->next;
    uint8_t * cadena_retorno = NULL;
    uint8_t * string = NULL;
    uint8_t c_concat = 0;
    uint8_t i = 0;
    size -= 1;

    switch(datos->token)
    {
        case CADENA:
        { 
            uint8_t len = 0;
            uint8_t len_cadena = 0;
            c_concat = check_concatenacion(lexer, size);
            uint8_t ret = check_sub_indices(lexer, size);
            if(ret == 0)
                return NULL;
            uint8_t len_tokens_for = check_variable_for(lexer,size);
            if(datos->declarada == 0 && c_concat == 0)
            {
                uint8_t ignora_quotes = datos->numero_de_cadenas * 2;
                len_cadena = datos->len;
                len = strlen("printf(\"\\n\");")-ignora_quotes;
            }
            else
            {
                string = get_formato(size,lexer, c_concat);
                len_cadena = get_len_numbers(lexer, size);
                len = strlen(string)+3;
            }
            uint8_t temp[len_cadena+len+len_tokens_for];
            memset(temp, 0, sizeof(temp));

            if(datos->declarada == 0 && c_concat == 0)
            { 	
                strcat(temp, "printf(\"");

                for(i=0;i<size;i++)
                { 			
                    string = remove_quotes(lexer->valor);
                    strcat(temp, string);
                    free(string);
                    lexer = lexer->next;
                }
                strcat(temp, "\\n\");");
            }
            else
            {
                strcat(temp, string);
                for(i=0;i<size;i++)
                {
                    if(lexer->token == PLUS_CONCAT)
                        strcat(temp, ",");
                    else if(lexer->token == VARIABLE)
                    {
                        uint8_t token = ht_get(lexer->valor);
                        uint8_t token_for = get_token_function(lexer->valor);
                        if(token == CADENA && token_for == FOR)
                        {
                            uint8_t temporal[strlen(lexer->valor)+3];
                            memset(temporal, 0, sizeof(temporal));
                            sprintf(temporal, "*%s", lexer->valor);
                            strcat(temp, temporal);
                        }
                        else
                            strcat(temp, lexer->valor);
                    }
                    else
                    {
                        if(lexer->token == CADENA)
                            checa_y_modifica(lexer->valor);
                        strcat(temp, lexer->valor);
                    }
                    lexer = lexer->next;
                }
                strcat(temp, ");");
            }
			//printf("%d : %d : %d\n", strlen(temp), sizeof(temp), linea);
            cadena_retorno = (uint8_t *)malloc(strlen(temp)+1);
            strcpy(cadena_retorno, temp);
            if(datos->declarada == 1 || c_concat == 1)
                free(string);

            break;
        }
		/*
		FLOAT y NUMERO se parecen demasiado. En un futuro lo implementare en una sola funcion
		para ahorrar codigo. Algo asi: verificar_numero(lexer, size, token) Donde token puede ser NUMERO o FLOAT. 
		De esa manera podre determinar el formato adecuado
		*/
        case NUMERO:
        { 
            uint16_t len = get_len_numbers(lexer, size);
            c_concat = check_concatenacion(lexer, size);
            uint8_t ret = check_sub_indices(lexer, size);
            if(ret == 0)
                return NULL;
            uint16_t formato_len = 0;
            if(c_concat > 0)
            {
                check_float(size, lexer);
                string = get_formato(size, lexer, c_concat);
                formato_len = strlen(string)+3;
            }
            else
                formato_len = strlen("printf(\"%d\\n\",);")+1;
            uint8_t temp[len+formato_len+1];
            uint8_t check_temp_balanced[len+1];
            memset(temp, 0, sizeof(temp));
            memset(check_temp_balanced, 0, sizeof(check_temp_balanced));

            if(c_concat == 0)
                strcat(temp, "printf(\"%d\\n\",");
            else
                strcat(temp, string);		

            for(i=0;i<size;i++)
            {
                if(lexer->token == PLUS_CONCAT && c_concat > 0)
                    strcat(check_temp_balanced, ",");
                else if(lexer->token == INT)
                    strcat(check_temp_balanced, "atoi");
                else if(lexer->token == LEN)
                    strcat(check_temp_balanced, "strlen");
                else if(lexer->token == FLOAT_F	)
                    strcat(check_temp_balanced, "atof");
                else
                {
                    if(lexer->token == CADENA && c_concat > 0)
                        checa_y_modifica(lexer->valor);
                    strcat(check_temp_balanced, lexer->valor);
                }
                lexer = lexer->next;
            }
            if(datos->operacion_secundaria == OPE_DE_PARENTESIS)
            {
                if(!check_balanced(check_temp_balanced, PRINT))
                {
                    debug("\nError en linea %d: los parentesis no estan balanceados\n", linea);
                    return NULL;
                }
            }

            strcat(temp, check_temp_balanced);
            strcat(temp, ");");
			//printf("%d : %d : %d\n", strlen(temp), sizeof(temp), linea);
            cadena_retorno = (uint8_t *)malloc(strlen(temp)+1);
            strcpy(cadena_retorno, temp);
            if(c_concat > 0)
                free(string);

            break;
        }
        case FLOAT:
        {
            uint16_t len = get_len_numbers(lexer, size);
            c_concat = check_concatenacion(lexer, size);
            uint8_t ret = check_sub_indices(lexer, size);
            if(ret == 0)
                return NULL;
            uint16_t formato_len = 0;
            if(c_concat > 0)
            {
                check_float(size, lexer);
                string = get_formato(size,lexer, c_concat);	
                formato_len = strlen(string)+3;
            }
            else
                formato_len = strlen("printf(\"%f\\n\",);")+1;
            
            uint8_t temp[len+formato_len];
            uint8_t check_temp_balanced[len+1];
            memset(temp, 0, sizeof(temp));
            memset(check_temp_balanced, 0, sizeof(check_temp_balanced));

            if(c_concat == 0)
                strcat(temp, "printf(\"%f\\n\",");
            else
                strcat(temp, string);		

            for(i=0;i<size;i++)
            {
                if(lexer->token == PLUS_CONCAT && c_concat > 0)
                    strcat(check_temp_balanced, ",");
                else if(lexer->token == INT)
                    strcat(check_temp_balanced, "atoi");
                else if(lexer->token == LEN)
                    strcat(check_temp_balanced, "strlen");
                else if(lexer->token == FLOAT_F	)
                    strcat(check_temp_balanced, "atof");
                else
                {
                    if(lexer->token == CADENA && c_concat > 0)
                        checa_y_modifica(lexer->valor);
                    strcat(check_temp_balanced, lexer->valor);
                }
                lexer = lexer->next;
            }
            if(datos->operacion_secundaria == OPE_DE_PARENTESIS)
            {
                if(!check_balanced(check_temp_balanced, PRINT))
                {
                    debug("\nError en linea %d: los parentesis no estan balanceados\n", linea);
                    return NULL;
                }
            }

            strcat(temp, check_temp_balanced);
            strcat(temp, ");");
			//printf("%d : %d : %d\n", strlen(temp), sizeof(temp), linea);
            cadena_retorno = (uint8_t *)malloc(strlen(temp)+1);
            strcpy(cadena_retorno, temp);
            if(c_concat > 0)
                free(string);

            break;
        }
        case LIST:
        {
           /* uint16_t len = get_len_numbers(lexer, size);
            c_concat = check_concatenacion(lexer, size);
            uint8_t ret = check_sub_indices(lexer, size);
            if(ret == 0)
                return NULL;
            uint16_t formato_len = 0;
            if(c_concat > 0)
            {
                check_float(size, lexer);
                string = get_formato(size,lexer, c_concat);	
                formato_len = strlen(string)+3;
            }
            else
                formato_len = strlen("print_indice_en_especifico()")+1;*/
            
            
            if(!is_only_list(lexer, size))
                return NULL;
            uint8_t valida = is_corchetes(lexer);
            uint16_t len = 0;
            uint8_t * indice = NULL;     
            if(valida == 0)
            {            
                indice = get_list_indice(lexer, size);
                len = strlen("print_indice_en_especifico(->next,);")+strlen(lexer->valor)+strlen(indice);
                uint8_t temp[len+1];
                memset(temp, 0, sizeof(temp));
        	}
        	else
        	{
        	    len = strlen("print_indice();")+strlen(lexer->valor);
        	}

        	uint8_t temp[len+1];
        	memset(temp, 0, sizeof(temp));

        	if(valida == 0)
        	    sprintf(temp, "print_indice_en_especifico(%s->next,%s);",lexer->valor, indice);
        	else
        	    sprintf(temp, "print_indice(%s);", lexer->valor);

            cadena_retorno = malloc(strlen(temp)+1);
            if(cadena_retorno == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }    
            strcpy(cadena_retorno, temp);

            //printf("%d : %d\n", strlen(temp), sizeof(temp));

            break;
        } 
        default:
            debug("\nError en linea %d: se desconoce: %s\n", linea,lexer->valor);
            return NULL;
            break;
    }

    return cadena_retorno;
}

data * check_print_grammar(lexical * lexer, uint8_t size)
{
    size -= 1;

    if(size == 0)
    {
        debug("\nError en linea %d: print no tiene argumentos\n", linea);
        return NULL;
    }

    lexer = lexer->next;
    data * datos = basic_grammar(lexer, size);

    if(datos == NULL)
        return NULL;

    return datos;
}
