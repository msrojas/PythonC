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

#include "if_grammar.h"

uint8_t busca_parentesis(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    for(i=0;i<size;i++)
    {
        if(lexer->token == L_PARENTESIS || lexer->token == R_PARENTESIS)
            return 1;
        lexer = lexer->next;
    }

    return 0;
}

uint8_t verificar_expresion(lexical * lexer, uint8_t size)
{
    lexer = lexer->next;
    size -= 1;

    uint8_t i=0;
    uint8_t expresion1 = 0;
    uint8_t * valor_exp1 = NULL;
    uint8_t expresion2 = 0;
    uint8_t * valor_exp2 = NULL;
    uint8_t operador1 = 0;
    uint8_t operador2 =0;

    for(i=0;i<size;i++)
    {
        if(lexer->token != MAYOR_QUE && lexer->token != MENOR_QUE && lexer->token != IGUAL && lexer->token != DOS_PUNTOS && lexer->token != DISTINTO)
        {
            if(expresion1 == 0)
            {
                expresion1 = lexer->token;
                valor_exp1 = (uint8_t *)malloc(strlen(lexer->valor)+1);
                if(valor_exp1 == NULL)
                {
                    debug("\nError: ocurrio un inconveniente en la heap\n");
                    return 0;
                }
                strcpy(valor_exp1, lexer->valor);
            }
            else if(expresion2 == 0)
            {
                expresion2 = lexer->token;
                valor_exp2 = (uint8_t *)malloc(strlen(lexer->valor)+1);
                if(valor_exp2 == NULL)
                {
                    debug("\nError: ocurrio un inconveniente en la heap\n");
                    return 0;
                }
                strcpy(valor_exp2, lexer->valor);
            }
        }
        else if(lexer->token == MAYOR_QUE || lexer->token == MENOR_QUE || lexer->token == IGUAL || lexer->token == DISTINTO)
        {
            if(operador1 == 0)
                operador1 = lexer->token;
            else if(operador2 == 0)
                operador2 = lexer->token;
        }
        else if(expresion1 != 0 && expresion2 != 0)
        {
            if((expresion1 == NUMERO || expresion1 == FLOAT) && expresion2 == CADENA)
            {
                debug("\nError en linea %d: en C, no se pueden comparar numeros con cadenas de caracteres\n", linea);
                free(valor_exp1);
                free(valor_exp2);
                return 0;
            }
            else if(expresion1 == CADENA && (expresion2 == NUMERO || expresion2 == FLOAT))
            {
                debug("\nError en linea %d: en C, no se pueden comparar numeros con cadenas de caracteres\n", linea);
                free(valor_exp1);
                free(valor_exp2);
                return 0;
            }
            else if(expresion1 == VARIABLE && expresion2 != VARIABLE)
            {
                uint8_t token = ht_get(valor_exp1);
                if((token == NUMERO || token == FLOAT) && expresion2 == CADENA)
                {
                    debug("\nError en linea %d: en C, no se pueden comparar numeros con cadenas de caracteres\n", linea);
                    free(valor_exp1);
                    free(valor_exp2);
                    return 0;
                }
                else if(token == CADENA && (expresion2 == NUMERO || expresion2 == FLOAT))
                {
                    debug("\nError en linea %d: en C, no se pueden comparar numeros con cadenas de caracteres\n", linea);
                    free(valor_exp1);
                    free(valor_exp2);
                    return 0;
                }
                else if(token == CADENA && expresion2 == CADENA)
                {
                	free(valor_exp1);
                    free(valor_exp2);
                    return 2;
                }
            }
            else if(expresion2 == VARIABLE && expresion1 != VARIABLE)
            {
                uint8_t token = ht_get(valor_exp2);
                if((token == NUMERO || token == FLOAT) && expresion1 == CADENA)
                {
                    debug("\nError en linea %d: en C, no se pueden comparar numeros con cadenas de caracteres\n", linea);
                    free(valor_exp1);
                    free(valor_exp2);
                    return 0;
                }
                else if(token == CADENA && (expresion2 == NUMERO || expresion1 == FLOAT))
                {
                    debug("\nError en linea %d: en C, no se pueden comparar numeros con cadenas de caracteres\n", linea);
                    free(valor_exp1);
                    free(valor_exp2);
                    return 0;
                }
                else if(token == CADENA && expresion1 == CADENA)
                {
                	free(valor_exp1);
                    free(valor_exp2);
                    return 2;
                }
            }
            else if(expresion1 == CADENA && expresion2 == CADENA)
            {
                if((operador1 == IGUAL || operador1 == DISTINTO) && operador2 == IGUAL)
                {
                    free(valor_exp1);
                    free(valor_exp2);
                    return 2;
                }
                else 
                {
                    debug("\nError en linea %d: las cadenas de caracteres solo pueden ser usadas con el operador: \"==\"\n", linea);
                    free(valor_exp1);
                    free(valor_exp2);
                    return 0;
                }
            }
            else if(expresion1 == VARIABLE && expresion2 == VARIABLE)
            {
                uint8_t token1 = ht_get(valor_exp1);
                uint8_t token2 = ht_get(valor_exp2);
                if((token1 == NUMERO || token1 == FLOAT) && token2 == CADENA)
                {
                    debug("\nError en linea %d: en C, no se pueden comparar numeros con cadenas de caracteres\n", linea);
                    free(valor_exp1);
                    free(valor_exp2);
                    return 0;
                }
                else if(token2 == CADENA && (token1 == NUMERO || token2 == FLOAT))
                {
                    debug("\nError en linea %d: en C, no se pueden comparar numeros con cadenas de caracteres\n", linea);
                    free(valor_exp1);
                    free(valor_exp2);
                    return 0;
                }
                else if(token1 == CADENA && token2 == CADENA)
                {
                	if((operador1 == IGUAL || operador1 == DISTINTO) && operador2 == IGUAL)
                    {
                        free(valor_exp1);
                        free(valor_exp2);
                        return 2;
                    }
                    else 
                    {
                        debug("\nError en linea %d: las cadenas de caracteres solo pueden ser usadas con el operador: \"==\"\n", linea);
                        free(valor_exp1);
                        free(valor_exp2);
                        return 0;
                    }
                }
            }
            free(valor_exp1);
            free(valor_exp2);
            valor_exp1 = NULL;
            valor_exp2 = NULL;
        }
        lexer = lexer->next;
    }

    if(valor_exp1 != NULL)
    	free(valor_exp1);
    if(valor_exp2 != NULL)
    	free(valor_exp2);

    return 1;
}

uint8_t * print_original_else(lexical * lexer, uint8_t size)
{
    if(size > 2 || size == 1)
    {
        debug("\nError en linea %d: sintaxis invalida\n",linea);
        return NULL;
    }

    uint16_t len = get_len_numbers(lexer, size);

    if(lexer->token != ELSE)
    {
        debug("\nError en linea %d: sintaxis invalida\n", linea);
        return NULL;
    }

    uint8_t temp[len+2];
    memset(temp, 0, sizeof(temp));

    strcat(temp, lexer->valor);

    lexer = lexer->next;
    if(lexer->token != DOS_PUNTOS)
    {
        debug("\nError en linea %d: sintaxis invalida: %s\n",linea,lexer->valor);
        return NULL;
    }

    sprintf(temp, "%s\n{", temp);
    uint8_t * retorna = (uint8_t *)malloc(strlen(temp)+1);
    if(retorna == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }

    return strcpy(retorna, temp);
}

uint8_t * print_original_if(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    uint8_t ret = verificar_expresion(lexer, size);
    if(ret == 0)
    	return NULL;

    uint8_t formato = 0;
    uint16_t len = get_len_numbers(lexer, size);
    if(ret == 2)
        formato = strlen("(strcmp(,)==0\n");
    else		
        formato = strlen("(\n");
    uint8_t temp[formato+len+1]; 
    uint8_t parentesis = busca_parentesis(lexer, size);
    memset(temp, 0, sizeof(temp));

    strcat(temp, lexer->valor);
    strcat(temp, "(");
    if(ret == 2)
    {
        strcat(temp, "strcmp(");
    }
    lexer = lexer->next;
   	size -= 1;

   	uint8_t last_token = 0;
   	uint8_t token_t = 0;
    for(i=0;i<size;i++)
    {
    	if(lexer->token == INT)
            strcat(temp, "atoi");
        else if(lexer->token == LEN)
            strcat(temp, "strlen");
        else if(lexer->token == FLOAT_F	)
            strcat(temp, "atof");
        else if(lexer->token == DOS_PUNTOS)
            strcat(temp, ")");
        else if(ret == 2 && (lexer->token == IGUAL || lexer->token == DISTINTO))
        {
             if(last_token == CADENA || last_token == VARIABLE)
            {
                strcat(temp, ",");
                token_t = lexer->token;
            }
        }         
        else
        {
            strcat(temp, lexer->valor);
        }
        last_token = lexer->token;
        lexer = lexer->next;
    }

    if(parentesis > 0)
    {
        if(!check_balanced(temp, IF))
        {
            debug("\nError en linea %d: los parentesis no estan balanceados\n", linea);
            return NULL;
        }
    }
    
    if(ret == 2)
    {
        if(token_t == IGUAL)
            strcat(temp, "==0)");
        else
            strcat(temp, "!=0)");
    }

    sprintf(temp, "%s\n{", temp);

    uint8_t * retorna = (uint8_t *)malloc(strlen(temp)+1);
    if(retorna == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }	
    strcpy(retorna, temp);

    return retorna;
}

uint8_t check_if_grammar(lexical * lexer, uint8_t size)
{
    lexer = lexer->next;
    size -= 1;

    if(size == 0)
    {
    	debug("\nError en linea %d: no hay expresion\n", linea);
    	return 0;
    }

    uint8_t i=0;
    uint8_t last_token = 0;
    uint8_t temp_token = 0;
    uint8_t token = 0;
    for(i=0;i<size;i++)
    {
        if((lexer->token == MAYOR_QUE || lexer->token == MENOR_QUE || lexer->token == R_PARENTESIS || lexer->token == OPERADOR || lexer->token == IGUAL || lexer->token == CONCATENACION || lexer->token == PLUS_CONCAT || lexer->token == DOS_PUNTOS || lexer->token == DISTINTO) && last_token == 0)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        } 
        else if(lexer->token == PRINT || lexer->token == CONCATENACION)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }
        else if(lexer->token == NUMERO && last_token != 0)
        { 
            if(last_token == CADENA || last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == PLUS_CONCAT || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN || last_token == DOS_PUNTOS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
            else if(temp_token == INT || temp_token == FLOAT_F || temp_token == LEN)
            {
                debug("\nError en linea %d: la funcion trabaja solo con cadenas: %s\n", linea, lexer->valor);
                return 0;
            }
        }
        else if(lexer->token == FLOAT && last_token != 0)
        {
            if(last_token == CADENA || last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == PLUS_CONCAT || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN || last_token == DOS_PUNTOS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
            else if(temp_token == INT || temp_token == FLOAT_F || temp_token == LEN)
            {
                debug("\nError en linea %d: la funcion trabaja solo con cadenas: %s\n", linea, lexer->valor);
                return 0;
            }
        }
        else if(lexer->token == INT && (last_token !=0 || last_token == 0))
        {
            if(last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN || last_token == DOS_PUNTOS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
            temp_token = INT;
        }
        else if(lexer->token == FLOAT_F && (last_token != 0 || last_token == 0))
        {
            if(last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN || last_token == DOS_PUNTOS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
            temp_token = FLOAT_F;
        }
        else if(lexer->token == LEN && (last_token !=0 || last_token == 0))
        {
            if(last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN || last_token == DOS_PUNTOS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
            temp_token = LEN;
        }
        else if(lexer->token == CADENA && last_token != 0)
        {
            if(last_token == NUMERO || (last_token == L_PARENTESIS && (temp_token != INT && temp_token != FLOAT_F && temp_token != LEN)) || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN || last_token == DOS_PUNTOS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
            else if(last_token == OPERADOR)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
            else if(last_token == L_PARENTESIS && (temp_token == INT || temp_token == FLOAT_F || temp_token == LEN))
                temp_token = 0;
        }
        else if(lexer->token == VARIABLE && last_token != 0)
        {
            if(last_token == CADENA || last_token == NUMERO || last_token == VARIABLE || last_token == R_PARENTESIS || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN || last_token == DOS_PUNTOS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
            else
            {
                token = ht_get(lexer->valor);
                if(token == 0)
                {
                    debug("\nError en linea %d: la variable \"%s\" no ha sido declarada\n", linea,lexer->valor);
                    return 0;
                }
                else if((token == NUMERO || token == FLOAT) && (temp_token == INT || temp_token == FLOAT_F || temp_token == LEN))
                {
                    debug("\nError en linea %d: la funcion trabaja solo con cadenas: %s\n", linea, lexer->valor);
                    return 0;
                }
                else if(last_token == L_PARENTESIS && (temp_token == INT || temp_token == FLOAT_F || temp_token == LEN))
                    temp_token = 0;
            }	
        }
        else if(lexer->token == OPERADOR && last_token != 0)
        {
            if(last_token == CADENA || last_token == OPERADOR || last_token == L_PARENTESIS || last_token == MENOR_QUE || last_token == MAYOR_QUE || last_token == IGUAL || last_token == DOS_PUNTOS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                return 0;
            }
        }
        else if(lexer->token == L_PARENTESIS && (last_token == NUMERO || last_token == CADENA || last_token == VARIABLE || last_token == FLOAT))
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }
        else if((lexer->token == MENOR_QUE || lexer->token == MAYOR_QUE) && (last_token == IGUAL || last_token == DISTINTO))
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }
        else if(lexer->token == DISTINTO && (last_token == MAYOR_QUE || last_token == MENOR_QUE || last_token == IGUAL || last_token == OPERADOR))
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }

        last_token = lexer->token;
        lexer = lexer->next;
    }

    if(last_token != DOS_PUNTOS)
    {
    	debug("\nError en linea %d: sintaxis invalida\n", linea);
        return 0;
    }

    return 1;
}
