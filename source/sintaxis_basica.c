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
#include "sintaxis_basica.h"

data * basic_grammar(lexical * lexer, uint8_t size)
{
    data * datos = (data *)malloc(sizeof(data));
    if(datos == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }

    uint8_t i = 0;
    uint8_t last_token = 0;
    uint8_t token = 0;
    uint8_t temp_token = 0;
    uint8_t cuenta_lparentesis = 0;
    uint8_t r_corhcetes = 0;
    uint8_t l_corchetes = 0;
    uint8_t temp[150];
    memset(temp, 0, sizeof(0));

    uint8_t ope_de_parentesis = 0;
    uint8_t operacion = 0;
    uint16_t len_valor = 0;

    datos->numero_de_cadenas = 0;
    datos->declarada = 0;
    uint8_t ret_token = lexer->token;
    if(ret_token == CADENA)
    {
        len_valor = strlen(lexer->valor) + 1;
        datos->numero_de_cadenas++;
    }

    for(i=0;i<size;i++)
    {
        if((lexer->token == OPERADOR || lexer->token == IGUAL || lexer->token == CONCATENACION || lexer->token == RAW_INPUT || lexer->token == R_CORCHETES) && last_token == 0)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            free(datos);
            return NULL;
        }
        else if(lexer->token == RAW_INPUT && last_token != 0)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return NULL;
        }
        else if(lexer->token == VARIABLE && last_token == 0)
        {
            token = ht_get(lexer->valor);
            if(token == 0)
            {
                debug("\nError en linea %d: la variable \"%s\" no ha sido declarada\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            datos->declarada = 1;
            ret_token = token;
        }
        else if(lexer->token == VARIABLE && i == cuenta_lparentesis && ret_token == L_PARENTESIS)
        {
            token = ht_get(lexer->valor);
            if(token == 0)
            {
                debug("\nError en linea %d: la variable \"%s\" no ha sido declarada\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(token == CADENA)
            {
                debug("\nError en linea %d: sintaxis invalida. La variable %s es una cadena y tiene una operacion de parentesis\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            ret_token = token;
        }

        else if(lexer->token == NUMERO && last_token != 0)
        { 
            if(last_token == CADENA || last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == PLUS_CONCAT || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(ret_token == L_PARENTESIS && i == cuenta_lparentesis)
            {
                if(ope_de_parentesis == 0)
                {
                    ret_token = NUMERO;
                    ope_de_parentesis = 1;
                }
            } 
            else if(temp_token == INT || temp_token == FLOAT_F || temp_token == LEN)
            {
                debug("\nError en linea %d: la funcion trabaja solo con cadenas: %s\n", linea, lexer->valor);
                free(datos);
                return NULL;
            }
        }

        else if(lexer->token == FLOAT && last_token != 0)
        { 
            if(last_token == CADENA || last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == PLUS_CONCAT || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(ret_token == L_PARENTESIS && i == cuenta_lparentesis)
            {
                if(ope_de_parentesis == 0)
                {
                    ret_token = FLOAT;
                    ope_de_parentesis = 1;
                }
            }
            else if(temp_token == INT || temp_token == FLOAT_F || temp_token == LEN)
            {
                debug("\nError en linea %d: la funcion trabaja solo con cadenas: %s\n", linea, lexer->valor);
                free(datos);
                return NULL;
            }
            else if(operacion == 1)
            {
                ret_token = FLOAT;
            }
        }
	
	else if(lexer->token == INT && (last_token != 0 || last_token == 0))
        {
            if(last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(ret_token == L_PARENTESIS && i == cuenta_lparentesis)
            {
                if(ope_de_parentesis == 0)
                {
                    ope_de_parentesis = 1;
                }
            }
            if(ret_token != FLOAT)
                ret_token = NUMERO;
            temp_token = INT;
        }	
        else if(lexer->token == FLOAT_F && (last_token != 0 || last_token == 0))
        {
            if(last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(ret_token == L_PARENTESIS && i == cuenta_lparentesis)
            {
                if(ope_de_parentesis == 0)
                {
                    ope_de_parentesis = 1;
                }
            }
            ret_token = FLOAT;
            temp_token = FLOAT_F;
        }
        else if(lexer->token == LEN && (last_token != 0 || last_token == 0))
        {
            if(last_token == NUMERO || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(ret_token == L_PARENTESIS && i == cuenta_lparentesis)
            {
                if(ope_de_parentesis == 0)
                {
                    ope_de_parentesis = 1;
                }
            }
            if(ret_token != FLOAT)
                ret_token = NUMERO;
            temp_token = LEN;
        }
        
        else if(lexer->token == CADENA && last_token != 0)
        {
            if(last_token == NUMERO || (last_token == L_PARENTESIS && (temp_token != INT && temp_token != FLOAT_F && temp_token != LEN)) || last_token == R_PARENTESIS || last_token == VARIABLE || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(last_token == CADENA)
            {
                len_valor += strlen(lexer->valor) + 1;
                datos->numero_de_cadenas++;
            }
            else if(last_token == OPERADOR && strcmp(temp, "+") != 0)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(last_token == OPERADOR && (ret_token == NUMERO || ret_token == FLOAT))
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if(last_token == L_PARENTESIS && (temp_token == INT || temp_token == FLOAT_F || temp_token == LEN))
                temp_token = 0;
        }
        else if(lexer->token == VARIABLE && last_token != 0)
        {
            if(last_token == CADENA || last_token == NUMERO || last_token == VARIABLE || last_token == R_PARENTESIS || last_token == FLOAT || last_token == INT || last_token == FLOAT_F || last_token == LEN)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else
            {
                token = ht_get(lexer->valor);
                if(token == 0)
	        {
                    debug("\nError en linea %d: la variable \"%s\" no ha sido declarada\n", linea,lexer->valor);
                    free(datos);
                    return NULL;
                }
                else if((ret_token == NUMERO && temp_token == 0 && token == CADENA && last_token != CONCATENACION && last_token != PLUS_CONCAT) || (ret_token == CADENA && last_token != CONCATENACION && (token == NUMERO || token == FLOAT)) || (ret_token == FLOAT && temp_token == 0 && token == CADENA && last_token != CONCATENACION && last_token != PLUS_CONCAT))
                {
                    debug("\nError en linea %d: la variable \"%s\" es otro tipo de dato\n", linea,lexer->valor);
                    free(datos);
                    return NULL;
                }
                else if((token == NUMERO || token == FLOAT) && last_token == PLUS_CONCAT)
                {
                    debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                    free(datos);					
                    return NULL;
                }
                else if((token == NUMERO || token == FLOAT) && (temp_token == INT || temp_token == FLOAT_F || temp_token == LEN))
                {
                    debug("\nError en linea %d: la funcion trabaja solo con cadenas: %s\n", linea, lexer->valor);
                    free(datos);
                    return NULL;
                }
                else if(operacion == 1 && token == FLOAT)
                {
                    ret_token = FLOAT;
                }
                if(last_token == L_PARENTESIS && (temp_token == INT || temp_token == FLOAT_F || temp_token == LEN))
                    temp_token = 0;
            }
        }
        else if(lexer->token == OPERADOR && last_token != 0)
	{
            if(last_token == OPERADOR || last_token == L_PARENTESIS)
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }

            else if((last_token == CADENA) && (strcmp(lexer->valor, "*") != 0 && strcmp(lexer->valor, "+") != 0))
            {
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
                free(datos);
                return NULL;
            }
            else if((last_token == CADENA || last_token == R_CORCHETES) && strcmp(lexer->valor, "+") == 0)
            {
                lexer->token = PLUS_CONCAT;
            }
            else if(last_token == VARIABLE && strcmp(lexer->valor, "+") == 0)
            {
                token = ht_get(temp);
                if(token == CADENA)
                {
                    lexer->token = PLUS_CONCAT;
                }
            }
            else if(last_token == NUMERO || last_token == VARIABLE)
            {
                if(last_token == VARIABLE)
                {
                    token = ht_get(temp);
                    if(token == NUMERO)
                    {
                        if(operacion == 0)
                        {
                            operacion++;
                        }
                    }
                }
                else if(operacion == 0)
                {
                    operacion++;
                }
            }

        }
        else if(lexer->token == IGUAL && last_token != 0)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            free(datos);
            return NULL;
        }
        else if(lexer->token == L_PARENTESIS && (last_token == NUMERO || last_token == CADENA || last_token == VARIABLE || last_token == FLOAT))
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            free(datos);
            return NULL;
        }
        else if(ret_token == L_PARENTESIS && lexer->token == L_PARENTESIS)
        {
            if(cuenta_lparentesis == i)
            {
                cuenta_lparentesis++;
            }
        }
        else if(lexer->token == R_PARENTESIS && last_token == L_PARENTESIS)
        {
            debug("\nError en linea %d: no hay expresion dentro de los parentesis\n", linea);
            free(datos);
            return NULL;
        }
        else if(last_token == CONCATENACION && lexer->token == L_PARENTESIS)
        {
            cuenta_lparentesis = i;
            cuenta_lparentesis++;
            ret_token = L_PARENTESIS;
        }
        else if(((ret_token == NUMERO || ret_token == FLOAT) && (lexer->token == R_PARENTESIS || lexer->token == L_PARENTESIS)))
        {
             if(ope_de_parentesis == 0)
             {
                  ope_de_parentesis = 1;
	     }
        }
        else if(lexer->token == CONCATENACION && (last_token == OPERADOR || last_token == L_PARENTESIS || last_token == CONCATENACION))
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            free(datos);
            return NULL;
        }
        else if((lexer->token == L_CORCHETES && (last_token == L_CORCHETES || last_token == R_CORCHETES || last_token == CADENA)) || (lexer->token == R_CORCHETES && last_token == R_CORCHETES)) //NUEVO
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return NULL;
        }
        else if(lexer->token == L_CORCHETES)
            l_corchetes++;
        else if(lexer->token == R_CORCHETES)
            r_corhcetes++;

        memset(temp, 0, sizeof(temp));
        last_token = lexer->token;
        strcpy(temp, lexer->valor);
        lexer = lexer->next;
    }

    if(last_token == OPERADOR || last_token == CONCATENACION || last_token == L_PARENTESIS || last_token == PLUS_CONCAT || last_token == INT || last_token == FLOAT_F || last_token == LEN || last_token == L_CORCHETES)
    {
        debug("\nError en linea %d: sintaxis invalida de operador\n",linea);
        free(datos);
        return NULL;
    }
    else if(l_corchetes > 0 && r_corhcetes == 0)
    {
        debug("\nError en linea %d: falta caracter \"]\"\n", linea);
        free(datos);
        return NULL;
    }

    datos->token = ret_token;
    datos->len = len_valor;

    if(ope_de_parentesis > 0)
    {
        datos->operacion_secundaria = OPE_DE_PARENTESIS;
    }
    else
    {
        datos->operacion_secundaria = 0;
    }

    return datos;
}
