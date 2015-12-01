#include "raw_input_grammar.h"

uint8_t * print_original_input(lexical * lexer, uint8_t size, uint8_t token)
{
    uint8_t i=0;
    uint8_t * retorno = NULL;
    uint8_t token_var = ht_get(lexer->valor);
    uint8_t temp_cadena[strlen(lexer->valor)+1];
    memset(temp_cadena, 0, sizeof(temp_cadena));
    strcpy(temp_cadena, lexer->valor);

    switch(token)
    {
        case CADENA:
        {
            uint16_t len = get_len_numbers(lexer, size);
            uint16_t formato = 0;
            if(token_var == 0)
                formato = strlen("char * ;")+len+2;
            else
                formato = strlen("free();\n;")+strlen(lexer->valor)+len+2;
            uint8_t temp[formato];
            memset(temp, 0, sizeof(temp));

            if(token_var == 0)
                sprintf(temp, "char * %s",lexer->valor);
            else
            {
                sprintf(temp, "free(%s);\n", lexer->valor);
                strcat(temp, lexer->valor);
            }
            lexer = lexer->next;
            size -= 1;
            uint8_t last_token = 0;

            for(i=0;i<size;i++)
            {
                if(last_token == L_PARENTESIS && lexer->token == R_PARENTESIS)
                    strcat(temp, "\"\"");
                strcat(temp, lexer->valor);
                last_token = lexer->token;
                lexer = lexer->next;
            
            }
            strcat(temp, ";");
            retorno = (uint8_t *)malloc(strlen(temp)+1);
            if(retorno == NULL)
            {
                debug("\nError: ocurrion un inconveniente en la heap\n");
                return NULL;
            }
            strcpy(retorno, temp);

            if(token_var == 0)
            {
                if(ht_put(temp_cadena, CADENA) == 2)
                {
                    printf("Errno\n");
                    free(retorno);
                    return NULL;
                }
                if(free_variables == NULL && free_root == NULL)
                {
                    if(!init_free())
                    {
                        free(retorno);
                        return NULL;
                    }
                    free_variables->var_name = NULL;
                    free_root = free_variables;
                }
                if(!agregar_free_var(temp_cadena))
                { 
                    free(retorno);
                    return NULL;
                }
                size_free++;
            }

            //printf("%d : %d\n", strlen(temp), sizeof(temp));

            break;
        }
        case INT:
        {
            uint16_t len = get_len_numbers(lexer, size);
            uint16_t formato = 0;
            if(token_var == 0)
                formato = strlen("int * ;")+len+2;
            else
                formato = strlen(";")+len+2;
            uint8_t temp[formato+1];
            memset(temp, 0, sizeof(temp));

            if(token_var == 0)
                sprintf(temp, "int %s",lexer->valor);
            else
                strcat(temp, lexer->valor);
            lexer = lexer->next;
            size -= 1;
            uint8_t last_token = 0;

            for(i=0;i<size;i++)
            {
                if(last_token == L_PARENTESIS && lexer->token == R_PARENTESIS)
                    strcat(temp, "\"\"");
                else if(lexer->token == INT)
                    strcat(temp, "atoi");
                else 
                    strcat(temp, lexer->valor);
                last_token = lexer->token;
                lexer = lexer->next;            
            }

            strcat(temp, ";");
            retorno = (uint8_t *)malloc(strlen(temp)+1);
            if(retorno == NULL)
            {
                debug("\nError: ocurrion un inconveniente en la heap\n");
                return NULL;
            }
            strcpy(retorno, temp);

            if(token_var == 0)
            {
                if(ht_put(temp_cadena, NUMERO) == 2)
                {
                    printf("Errno\n");
                    free(retorno);
                    return NULL;
                }
            }

            break;
        }
        case FLOAT_F:
        {
            uint16_t len = get_len_numbers(lexer, size);
            uint16_t formato = 0;
            if(token_var == 0)
                formato = strlen("float * ;")+len+2;
            else
                formato = strlen(";")+len+2;
            uint8_t temp[formato+1];
            memset(temp, 0, sizeof(temp));

            if(token_var == 0)
                sprintf(temp, "float %s",lexer->valor);
            else
                strcat(temp, lexer->valor);
            lexer = lexer->next;
            size -= 1;
            uint8_t last_token = 0;

            for(i=0;i<size;i++)
            {
                if(last_token == L_PARENTESIS && lexer->token == R_PARENTESIS)
                    strcat(temp, "\"\"");
                else if(lexer->token == FLOAT_F)
                    strcat(temp, "atof");
                else 
                    strcat(temp, lexer->valor);
                last_token = lexer->token;
                lexer = lexer->next;            
            }

            strcat(temp, ";");
            retorno = (uint8_t *)malloc(strlen(temp)+1);
            if(retorno == NULL)
            {
                debug("\nError: ocurrion un inconveniente en la heap\n");
                return NULL;
            }
            strcpy(retorno, temp);

            if(token_var == 0)
            {
                if(ht_put(temp_cadena, FLOAT) == 2)
                {
                    printf("Errno\n");
                    free(retorno);
                    return NULL;
                }
            }

            break;
        }
        default:
            debug("\nError en linea %d: se desconoce el tipo %s\n",linea, lexer->valor);
            break;
    }

    return retorno;
}

uint8_t check_sintax_input(lexical * lexer, uint8_t size, uint8_t token_type)
{
    uint8_t i=0;
    uint8_t last_token = 0;
    uint8_t l_parentesis = 0;
    uint8_t r_parentesis = 0;

    for(i=0;i<size;i++)
    {
        if(lexer->token != L_PARENTESIS && last_token == RAW_INPUT)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }
        else if(last_token == L_PARENTESIS && (lexer->token != CADENA && lexer->token != R_PARENTESIS))
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }
        else if(last_token == CADENA && lexer->token != R_PARENTESIS)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }
        else if(last_token == R_PARENTESIS && lexer->token != R_PARENTESIS)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea,lexer->valor);
            return 0;
        }
        if(lexer->token == L_PARENTESIS)
            l_parentesis++;
        else if(lexer->token == R_PARENTESIS)
            r_parentesis++;

        last_token = lexer->token;
        lexer = lexer->next;
    }

    if(last_token != R_PARENTESIS)
    {
    	debug("\nError en linea %d: sintaxis invalida\n", linea);
        return 0;
    }
    else if(l_parentesis > 1 || (token_type == INT && (r_parentesis == 1 || r_parentesis > 2)) || (token_type == CADENA && r_parentesis > 1))
    {
        debug("\nError en linea %d: sintaxis invalida. Error de parentesis\n", linea);
        return 0;
    }

    return 1;

}

uint8_t check_raw_input_grammar(lexical * lexer, uint8_t size, uint8_t token_type)
{
    uint8_t token = ht_get(lexer->valor);
    uint8_t temp[strlen(lexer->valor)+1];
    memset(temp, 0, sizeof(temp));
    strcpy(temp, lexer->valor);

    uint8_t token_comentario = var_get(lexer->valor);
    uint8_t token_raw = var_get("raw_input");
    if(token_comentario == CHAR)
    {
        debug("\nError en linea %d: las funcion raw_input debe ser manejada por un puntero, no por una variable \"#char\"\n", linea);
        return 0;
    }
    else if(token_raw == 0)
    {
        debug("\nError en linea %d: la funcion raw_input no fue definida. Debes definirla antes en un comentario: #raw_input\n", linea);
        return 0;
    }

    lexer = lexer->next;
    if(lexer->token != IGUAL)
    {
        debug("\nError en linea %d: no hay signo igual\n", linea);
        return 0;
    }

    lexer = lexer->next;
    size -= 2;

    if(size == 0)
    {
        debug("\nError en linea %d: falta valor\n",linea);
        return 0;
    }
    if(token_type == INT)
    {
        if(token != 0 && token != NUMERO)
        {
            debug("\nError en linea %d: la variable no es tipo entero: %s\n", linea, temp);
            return 0;
        }

        if(lexer->token != INT)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        lexer = lexer->next;
        if(lexer->token != L_PARENTESIS)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        lexer = lexer->next;
        size -= 2;
        if(size == 0)
        {
            debug("\nError en linea %d: falta valor\n",linea);
            return 0;
        }
    }
    else if(token_type == FLOAT_F)
    {
        if(token_comentario == 0)
        {
            debug("\nError en linea %d: la variable \"%s\" tiene operacion con decimales. Debes definir la variable en un comentario como float para poder usarla como tal. Ejemplo\n#float nombre_variable\nAgrega esa linea antes de declarar la variable\n",linea, lexer->valor);
            return 0;
        }
        if(token != 0 && token != FLOAT)
        {
            debug("\nError en linea %d: la variable no es tipo float: %s\n", linea, temp);
            return 0;
        }

        if(lexer->token != FLOAT_F)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        lexer = lexer->next;
        if(lexer->token != L_PARENTESIS)
        {
            debug("\nError en linea %d: sintaxis invalida: %s\n", linea, lexer->valor);
            return 0;
        }
        lexer = lexer->next;
        size -= 2;
        if(size == 0)
        {
            debug("\nError en linea %d: falta valor\n",linea);
            return 0;
        }
    }
    else if(token_type == CADENA)
    {
        if(token != 0 && token != CADENA)
        {
            debug("\nError en linea %d: la variable no es de tipo cadena: %s\n", linea, temp);
            return 0;
        }
    }

    uint8_t ret = check_sintax_input(lexer, size, token_type);
    if(ret == 0)
        return 0;

    return token_type;
    
}