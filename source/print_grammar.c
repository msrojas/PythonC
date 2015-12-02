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

uint8_t StrCat(lexical * lexer, uint8_t * temp)
{
    uint8_t token = 0;
    if(lexer->token == NUMERO || lexer->token == INT)
        strcat(temp, "%d");
    else if(lexer->token == FLOAT || lexer->token == FLOAT_F)
        strcat(temp, "%f");
    else if(lexer->token == CADENA)
        strcat(temp, "%s");
    else if(lexer->token == FLOAT)
        strcat(temp, "%f");
    else if(lexer->token == VARIABLE)
    {
        token = ht_get(lexer->valor);
        if(token == NUMERO)
            strcat(temp, "%d");
        else if(token == FLOAT)
            strcat(temp, "%f");
        else if(token == CADENA)
            strcat(temp, "%s");
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
            else if(lexer->token == VARIABLE)
            {
                token = ht_get(lexer->valor);
                if(token == NUMERO)
                    strcat(temp, "printf(\"%d");
                else if(token == CADENA)
                    strcat(temp, "printf(\"%s");
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

short get_len_numbers(lexical * lexer, uint8_t size)
{
    uint8_t i = 0;
    short len = 0;

    for(i=0;i<size;i++)
    {
        if(lexer->token == LEN)
            len += 3;
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
            uint8_t temp[len_cadena+len];
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
			//printf("%d : %d\n", strlen(temp), sizeof(temp));
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
            uint16_t formato_len = 0;
            if(c_concat > 0)
            {
                check_float(size, lexer);
                string = get_formato(size, lexer, c_concat);
                formato_len = strlen(string)+3;
            }
            else
                formato_len = strlen("printf(\"%d\\n\",);")+1;
            uint8_t temp[len+formato_len];
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
			//printf("%d : %d\n", strlen(temp), sizeof(temp));
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
			//printf("%d : %d\n", strlen(temp), sizeof(temp));
            cadena_retorno = (uint8_t *)malloc(strlen(temp)+1);
            strcpy(cadena_retorno, temp);
            if(c_concat > 0)
                free(string);

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
