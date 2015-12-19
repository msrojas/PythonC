#include "for_grammar.h"

uint8_t * get_rango_stop(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    lexer = lexer->next;
    lexer = lexer->next;
    lexer = lexer->next;
    lexer = lexer->next;
    size -= 4; 

    uint16_t len = get_len_numbers(lexer, size);
    uint8_t temp[len];
    memset(temp, 0, sizeof(temp));

    for(i=0;i<size;i++)
    {
        if(lexer->token == CONCATENACION)
        {
            i++;
            break;
        }
        lexer = lexer->next;
    }

    lexer = lexer->next;
    size -= i;

    for(i=0;i<size;i++)
    {
        if(lexer->token == LEN)
            strcat(temp, "strlen");
        else
            strcat(temp, lexer->valor);
        lexer = lexer->next;
    }

    uint16_t len_var = strlen(temp);
    temp[len_var - 2] = '\0';

    uint8_t * retorna = malloc(strlen(temp)+1);
    if(retorna == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }
    return strcpy(retorna, temp);
}

uint8_t verificar_rango_stop(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    lexer = lexer->next;
    lexer = lexer->next;
    lexer = lexer->next;
    lexer = lexer->next;
    size -= 4;

    uint8_t ret = 0;

    for(i=0;i<size;i++)
    {
        if(lexer->token == CONCATENACION)
        {
            ret++;
            break;
        }

        lexer = lexer->next;
    }

    return ret;
}

uint8_t * get_rango_origen(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    lexer = lexer->next;
    lexer = lexer->next;
    lexer = lexer->next;
    lexer = lexer->next;
    size -= 4; 

    uint16_t len = get_len_numbers(lexer, size);
    uint8_t temp[len];
    uint8_t concatenacion = 0;
    memset(temp, 0, sizeof(temp));

    for(i=0;i<size;i++)
    {
        if(lexer->token == CONCATENACION || lexer->token == DOS_PUNTOS)
        {
            if(lexer->token == CONCATENACION)
                concatenacion++;
            break;
        }
        else if(lexer->token == LEN)
            strcat(temp, "strlen");
        else
            strcat(temp, lexer->valor);
        lexer = lexer->next;
    }

    uint16_t len_var = strlen(temp);
    if(concatenacion == 0)
       temp[len_var - 1] = '\0';
    else
        temp[len_var] = '\0';

    uint8_t * retorna = malloc(strlen(temp)+1);
    if(retorna == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }
    return strcpy(retorna, temp);
}

uint16_t get_len_list_origen(lexical * lexer, uint8_t size)
{
    lexer = lexer->next;
    lexer = lexer->next;

    uint16_t len = strlen(lexer->valor);
    return len;
}

uint16_t get_var_origen(lexical * lexer, uint8_t size)
{
    lexer = lexer->next;
    lexer = lexer->next;

    uint16_t len = strlen("&[0]")+strlen(lexer->valor);
    return len;    
}

uint8_t * print_original_for(lexical * lexer, uint8_t size, uint8_t token_type)
{
	lexer = lexer->next;
    size -= 1;
	uint16_t len_type = 0;
    uint8_t token = ht_get(lexer->valor);
    uint16_t len_var = strlen(lexer->valor);
    uint8_t temp_cadena[len_var+1];
    memset(temp_cadena, 0, sizeof(temp_cadena));
    strcpy(temp_cadena, lexer->valor);

    uint8_t * cadena_retorno = NULL;

    switch(token_type)
    {
        case CADENA:
        {
            if(token == 0)
                len_type = strlen("char * =NULL;\n")+len_var;
            else
                len_type = strlen("free();\n=NULL;\n")+len_var+len_var;
            uint16_t len_var_origen = get_var_origen(lexer,size); 
            uint16_t len_var_incremento = strlen("*++")+len_var;
            uint16_t formato_for = len_type+strlen("for(;*!=\'\\0\';)\n{")+len_var+len_var_origen+len_var+len_var_incremento;
            uint8_t temp[formato_for+2];
            memset(temp, 0, sizeof(temp));

            if(token == 0)
                sprintf(temp, "char * %s=NULL;\nfor(%s=", lexer->valor,lexer->valor);
            else
                sprintf(temp, "free(%s);\n%s=NULL;\nfor(%s=", lexer->valor,lexer->valor,lexer->valor);
            lexer = lexer->next;
            lexer = lexer->next;
            sprintf(temp, "%s&%s[0];*%s!=\'\\0\';*%s++)\n{", temp, lexer->valor,temp_cadena,temp_cadena);

            cadena_retorno = malloc(strlen(temp)+1);
            if(cadena_retorno == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }

            strcpy(cadena_retorno, temp);
            if(token == 0)
            {
                if(ht_put(temp_cadena, token_type, FOR,NULL) == 2) //nuevo
                {
                    printf("Errno\n");
                    free(cadena_retorno);
                    return NULL;
                }
            }
            else if(token != 0) //NUEVO
            {
                if(!update_token_function(temp_cadena,FOR))
                {
                    free(cadena_retorno);
                    return NULL;
                }
            }

            break;
        }
        case LIST:
        {
            len_type = strlen("struct list * =NULL;\n")+len_var;
            uint16_t len_var_origen = get_len_list_origen(lexer,size);
            uint16_t formato_for = len_type+strlen("for(=->next;!=NULL;=->next)\n{")+len_var+len_var_origen+len_var+len_var+len_var;
            uint8_t temp[formato_for+1];
            memset(temp, 0, sizeof(temp));

            if(token == 0)
                sprintf(temp, "struct list * %s=NULL;\nfor(%s=", lexer->valor,lexer->valor);
            else
            {
                debug("\nError en linea %d: la lista ya fue declarada: %s\n", linea, lexer->valor);
                return NULL;
            }

            lexer = lexer->next;
            lexer = lexer->next;
            sprintf(temp, "%s%s->next;%s!=NULL;%s=%s->next)\n{", temp, lexer->valor, temp_cadena, temp_cadena, temp_cadena);

            cadena_retorno = malloc(strlen(temp)+1);
            if(cadena_retorno == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }
            strcpy(cadena_retorno, temp);

            if(token == 0)
            {
                if(ht_put(temp_cadena, token_type, FOR,NULL) == 2) //nuevo
                {
                    printf("Errno\n");
                    free(cadena_retorno);
                    return NULL;
                }
            }

            //printf("%d : %d\n", strlen(temp), sizeof(temp));
            
            break;
        }
        case RANGE:
        {
            if(token == 0)
                len_type = strlen("int =0;\n")+len_var;

            uint8_t  * rango_origen = get_rango_origen(lexer, size); 
            uint8_t verificar = verificar_rango_stop(lexer, size);
            uint16_t formato_for = 0;
            uint8_t * rango_stop = NULL;
            if(verificar == 0)
                formato_for = len_type+strlen("for(=0;<;++)\n{")+len_var+len_var+strlen(rango_origen)+len_var;
            else
            {
                rango_stop = get_rango_stop(lexer, size);
                formato_for = len_type+strlen("for(=;<;++)\n{")+len_var+strlen(rango_origen)+len_var+strlen(rango_stop)+len_var;
            }

            uint8_t temp[formato_for+1];
            memset(temp, 0, sizeof(temp));

            if(verificar == 0 && token == 0)
                sprintf(temp, "int %s=0;\nfor(%s=0;%s<%s;%s++)\n{", temp_cadena, temp_cadena,temp_cadena,rango_origen,temp_cadena);
            else if(verificar == 0 && token != 0)
                sprintf(temp, "for(%s=0;%s<%s;%s++)\n{", temp_cadena,temp_cadena,rango_origen,temp_cadena);
            else if(verificar == 1 && token == 0)
                sprintf(temp, "int %s=0;\nfor(%s=%s;%s<%s;%s++)\n{", temp_cadena, temp_cadena,rango_origen,temp_cadena,rango_stop,temp_cadena);
            else if(verificar == 1 && token != 0)
                sprintf(temp, "for(%s=%s;%s<%s;%s++)\n{", temp_cadena, rango_origen,temp_cadena,rango_stop,temp_cadena);

            cadena_retorno = malloc(strlen(temp)+1);
            if(cadena_retorno == NULL)
            {
                debug("\nError: ocurrio un inconveniente en la heap\n");
                return NULL;
            }

            strcpy(cadena_retorno, temp);


            if(token == 0)
            {
                if(ht_put(temp_cadena, NUMERO, FOR,NULL) == 2)
                {
                    free(cadena_retorno);
                    return NULL;
                }
            }
            free(rango_origen);
            if(rango_stop != NULL)
                free(rango_stop);

            //printf("%d : %d\n", strlen(temp), sizeof(temp));
            break;
        }
        default:
            debug("\nError en linea %d: valor DESCONOCIDO wey\n");
            break;
    }

    return cadena_retorno;
}

uint8_t check_range_grammar(lexical * lexer, uint8_t size)
{
    uint8_t i=0;
    uint8_t last_token = 0;
    uint8_t temp_token = 0;
    uint8_t concatenacion = 0;

    for(i=0;i<size;i++)
    {
        if(lexer->token != L_PARENTESIS && last_token == RANGE)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }
        else if(lexer->token == RANGE && last_token != 0)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == CADENA && (temp_token == 0 || (temp_token == INT && last_token != L_PARENTESIS)))
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == VARIABLE)
        {
            if(last_token == VARIABLE)
            { 
                debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
                return 0;
            }
            uint8_t token = ht_get(lexer->valor);
            if(token != CADENA && temp_token == LEN)
            {
                debug("\nError en linea %d: la variable debe ser tipo int: %s\n", linea, lexer->valor);
                return 0;
            }
            else if(token == 0)
            {
                debug("\nError en linea %d: la variable no ha sido declarada: %s\n",linea, lexer->valor);
                return 0;
            }

        }
        else if(lexer->token == NUMERO && last_token == NUMERO)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == R_PARENTESIS && last_token == L_PARENTESIS)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == RAW_INPUT || lexer->token == IF || lexer->token == ELSE || lexer->token == ELIF || lexer->token == WHILE || lexer->token == FOR)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == MAYOR_QUE || lexer->token == MENOR_QUE || lexer->token == IGUAL)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == R_CORCHETES || lexer->token == L_CORCHETES)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == R_PARENTESIS && temp_token == INT && last_token != CADENA)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == CONCATENACION && last_token == CONCATENACION)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == R_PARENTESIS && temp_token == INT && last_token == CADENA)
        {
            temp_token = 0;
        }
        else if(lexer->token == R_PARENTESIS && temp_token == LEN && (last_token == CADENA || last_token == VARIABLE))
        {
            temp_token = 0;
        }
        else if(lexer->token == INT || lexer->token == LEN)
        {
            temp_token = lexer->token;
        }
        else if(lexer->token == DOS_PUNTOS && last_token != R_PARENTESIS)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(lexer->token == R_PARENTESIS && last_token == CONCATENACION)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        if(lexer->token == CONCATENACION)
            concatenacion++;

        last_token = lexer->token;
        lexer = lexer->next;
    }

    if(last_token != DOS_PUNTOS)
    {
        debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
        return 0;
    }
    else if(concatenacion > 1)
    {
        debug("\nError en linea %d: por el momento, solo se permite dos rangos: range(init,stop)\n", linea);
        return 0;
    }


    return 1;
}

uint8_t check_for_grammar(lexical * lexer, uint8_t size)
{
    size -= 1;
    if(size == 0)
    {
        debug("\nError en linea %d: faltan parametros en for\n", linea);
        return 0;
    }

    lexer = lexer->next;
    if(lexer->token != VARIABLE)
    {
        debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
        return 0;
    }
    
    uint8_t token_type = 0;
    uint8_t variable_for = ht_get(lexer->valor);
    uint8_t variable_for_en_comentario = var_get(lexer->valor);

    uint8_t temp_cadena[strlen(lexer->valor)+1];
    memset(temp_cadena, 0, sizeof(temp_cadena));

    strcpy(temp_cadena, lexer->valor);

    lexer = lexer->next;
    if(lexer->token != IN)
    {
        debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
        return 0;
    }

    lexer = lexer->next;
    if(lexer->token == VARIABLE)
    {
        uint8_t token_var = ht_get(lexer->valor);
        if(token_var == NUMERO || token_var == FLOAT)
        {
            debug("\nError en linea %d: los valores numericos no son iterables: %s\n", linea, lexer->valor);
            return 0;
        }
        else if(token_var == CADENA)
        {
            if(variable_for != 0 && variable_for != CADENA)
            {
                debug("\nError en linea %d: la variable no es de tipo CADENA: %s\n", linea,temp_cadena);
                return 0;
            }
            else if((variable_for != 0 || variable_for == 0) && variable_for_en_comentario == CHAR)
            {
                debug("\nError en linea %d: la variable \"%s\" fue declarada en un comentario como \"char\"\n", linea, temp_cadena);
                return 0;
            }
            else if((variable_for == CADENA || variable_for == 0) && variable_for_en_comentario == 0)
            {
                token_type = CADENA;
            }
        }
        else if(token_var == LIST)
        {
            if(variable_for != 0 || variable_for_en_comentario != 0)
            {
                debug("\nError en linea %d: la variable \"%s\" ya fue declarada antes\n", linea, temp_cadena);
                return 0;
            }
            else
                token_type = LIST;
        }
        else if(token_var == 0)
        {
            debug("\nError en linea %d: la variable \"%s\" no ha sido declarada\n", linea, lexer->valor);
            return 0;
        }
    }
    else if(lexer->token == RANGE)
    {
        if((variable_for != 0 && variable_for != NUMERO) || variable_for_en_comentario == CHAR)
        {
            debug("\nError en linea %d: la variable no es numerica: %s\n", linea, temp_cadena);
            return 0;
        }
        size -= 2;
        if(!check_range_grammar(lexer, size))
            return 0;
        token_type = RANGE;
        return token_type;
    }
    else
    {
        debug("\nError en linea %d: se desconoce: %s\n", linea, lexer->valor);
        return 0;
    }

    lexer = lexer->next;
    if(lexer->token != DOS_PUNTOS)
    {
        debug("\nError en linea %d: faltan dos puntos en el for\n");
        return 0;
    }

    return token_type;
}
