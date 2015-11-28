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

#include "compilador.h"

const uint8_t operadores[] = "+-*/=";

void debug(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void liberacion_general()
{
    ht_destroy();
    if(free_variables != NULL)
    {
        acomodar_free_vars();
        libera_free_vars();
    }
    if(definir != NULL)
        var_destroy();
}

void libera_tokens(lexical ** lexer, uint8_t size)
{
    if(size == 0)
    {
        free(*lexer);
        return;
    }

    uint8_t i=0;
    lexical * temp;

    for(i=0;i<=size;i++)
    {
        temp = *lexer;
        *lexer = (*lexer)->next;
        if(temp->valor != NULL)
            free(temp->valor);
        free(temp);
    }
}

void acomodar_nodos(lexical ** lexer, lexical ** root)
{
    (*lexer)->next = *root;
    *lexer = *root;
}

uint8_t agregar_token(lexical ** lexer, uint8_t * temp, uint8_t r_token)
{
    lexical * next_node = (lexical *)malloc(sizeof(lexical));
    if(next_node == NULL)
    {
        debug("Error: ocurrio un inconveniente en la heap\n");
        return 0; 
    }

    next_node->valor = (uint8_t *)malloc(strlen(temp)+1);
    if(next_node->valor == NULL)
    {
        debug("Error: ocurrio un inconveniente en la heap\n");
        return 0;
    }

    strcpy(next_node->valor, temp);
    next_node->token = r_token;
    (*lexer)->next = next_node;
    *lexer = next_node;

    return 1;
}

void print_tokens(lexical * lexer, uint8_t size)
{
    uint8_t i = 0;
    for(i=0;i<=size;i++)
    {
		if(lexer->token != 0)
		{
			printf("%s ", lexer->valor); 
			switch(lexer->token)
			{
				case VARIABLE:
				    printf("VARIABLE\n");
					break;
				case CADENA:
					printf("CADENA\n");
					break;
				case NUMERO:
					printf("NUMERO\n");	
					break;
				case IGUAL:
					printf("IGUAL\n");
					break;
				case OPERADOR:
					printf("OPERADOR\n");
					break;
				case L_PARENTESIS:
					printf("L_PARENTESIS\n");
					break;
				case R_PARENTESIS:
					printf("R_PARENTESIS\n");
					break;			
				default:
					printf("DESCONOCIDO\n");
					break;		
			}
		}
		lexer = lexer->next;
	}
}

lexical * init_lexer()
{
    lexical * temp = (lexical *)malloc(sizeof(lexical));
    if(temp == NULL)
    {
        debug("\nError: ocurrio un inconveniente en la heap\n");
        return NULL;
    }

    return temp;
}

void libera_stack(balanced ** stack)
{
    balanced * temp;

    while(*stack != NULL)
    {
        temp = *stack;
        *stack = temp->next;
        free(temp);
	}
}

uint8_t check_pair(uint8_t value1, uint8_t value2)
{
    if(value1 == '(' && value2 == ')')
        return 1;
    else
        return 0;
}

uint8_t pop(balanced ** temp)
{
    uint8_t valor = 0;
    balanced * top;

    if(temp == NULL)
    {
        fprintf(stderr, "Los parentesis no estan balancedos\n");
        return 0;
    }

    top = *temp;
    valor = top->data;
    *temp = top->next;
    free(top);

    return valor;
}

uint8_t push(balanced ** temp, uint8_t new_data)
{
    balanced * new_node = (balanced *)malloc(sizeof(balanced));

    if(new_node == NULL)
    {
        fprintf(stderr, "Error: ocurrio un problema con la heap\n");
        return 0;
    }

    new_node->data = new_data;
    new_node->next = (*temp);
    (*temp) = new_node;

    return 1;
}

uint8_t check_balanced(uint8_t * cadena, uint8_t token)
{
    short indice = 0;

    if(token == VARIABLE)
    {
        while(cadena[indice] != '=')
            indice++;
    }

    balanced * stack = NULL;
    short i = 0;
    short len = strlen(cadena);
    uint8_t temp = 0;

    for(i=indice;i<len;i++)
    {
        if(cadena[i] == '(')
        {
            if(!push(&stack, cadena[i]))
                return 0;
        }
        else if(cadena[i] == ')')
        {
            if(stack == NULL)
                return 0;

            temp = pop(&stack);
            if(temp == 0)
                return 0;
            if(!check_pair(temp, cadena[i]))
            {
                if(stack != NULL)
                    libera_stack(&stack);
                return 0;
            }
        }
    }

    if(stack == NULL)
        return 1;
    else
    { 
        if(stack != NULL)
            libera_stack(&stack);
        return 0;
	}	
}

uint8_t * remove_quotes(uint8_t * cadena)
{
    short len_cadena = strlen(cadena);
    uint8_t quote = cadena[0];
    uint8_t temp[len_cadena+1];
    uint8_t indice = 0;
    short i = 0;

    memset(temp, 0, sizeof(temp));

    for(i=1;i<len_cadena;i++)
    {
        if(quote == cadena[i])
            break;
        else
            temp[indice++] = cadena[i];
	}

    uint8_t * char_retorna = (uint8_t *)malloc(strlen(temp)+1);
    strcpy(char_retorna, temp);

    return char_retorna;
}

uint8_t * semantic_analyzer(lexical ** lexer, int len, uint8_t size)
{
    *lexer = (*lexer)->next; //omitimos el token 0 (que es el root)
    data * datos;
    uint8_t * cadena_retorno = NULL;
	
    switch((*lexer)->token)
    {
        case VARIABLE:
            datos = check_variable_grammar(*lexer, size);
            if(datos == NULL)
                return NULL;
            cadena_retorno = print_origanl_variable(datos, *lexer, len, size);
            if(cadena_retorno == NULL)
            {
                free(datos);
                return NULL;
            }
            free(datos);
            break;
        case PRINT:
            datos = check_print_grammar(*lexer, size);
            if(datos == NULL)
                return NULL;
            cadena_retorno = print_original_value(*lexer, size, datos);
            if(cadena_retorno == NULL)
            { 
                free(datos);
                return NULL;
            }
            free(datos);
            break;
        default:
            debug("\nError en linea %d: se desconoce le tipo: %s\n", linea,(*lexer)->valor);
            return NULL;
    }

    datos = NULL;
    return cadena_retorno;
}

uint8_t es_parentesis(uint8_t valor)
{
    if(valor == '(' || valor == ')')
        return 1;
    else
        return 0;
}

uint8_t es_operador(uint8_t valor)
{
    uint8_t i=0;
    uint8_t retorno = 0;

    for(i=0;i<5;i++)
    {
        if(operadores[i] == valor)
        {
            retorno = 1;
            break;
        }
    }

    return retorno;
}

uint8_t check_keyword(uint8_t * token)
{
    uint8_t retorno = 1;

    if(strcmp(token, "char") == 0 || strcmp(token, "uint8_t") == 0 || strcmp(token, "int8_t") == 0)
    {
        debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
        retorno = 0;
	}
	else if(strcmp(token, "short") == 0 || strcmp(token, "uint16_t") == 0 || strcmp(token, "int16_t") == 0)
	{
		debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
		retorno = 0;
	}
	else if(strcmp(token, "int") == 0 || strcmp(token, "uint32_t") == 0 || strcmp(token, "int32_t") == 0)
	{
		debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
		retorno = 0;
	}	
	else if(strcmp(token, "long") == 0 || strcmp(token, "uint64_t") == 0 || strcmp(token, "int64_t") == 0)
	{
		debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
		retorno = 0;
	}
	else if(strcmp(token, "double") == 0 || strcmp(token, "uint128_t") == 0 || strcmp(token, "int128_t") == 0)
	{
		debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
		retorno = 0;
	}
	else if(strcmp(token, "float") == 0 || strcmp(token, "memset") == 0 || strcmp(token, "strlen") == 0)
	{
		debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
		retorno = 0;
	}
	else if(strcmp(token, "if") == 0 || strcmp(token, "else") == 0 || strcmp(token, "do") == 0)
	{
		debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
		retorno = 0;
	}
	else if(strcmp(token, "while") == 0 || strcmp(token, "for") == 0 || strcmp(token, "struct") == 0)
	{
		debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
		retorno = 0;
	}
	else if(strcmp(token, "typedef") == 0 || strcmp(token, "break") == 0 || strcmp(token, "continue") == 0)
	{
		debug("\nError en linea %d: el nombre \"%s\" es un keyword reservado\n", linea, token);
		retorno = 0;
	}

    return retorno;
}

uint8_t check_quotes(uint8_t * token, lexical ** lexer, uint8_t i)
{
    uint8_t simple_quotes = 0;
    uint8_t double_quotes = 0;
    uint8_t indice = 0;
    uint8_t len = strlen(token);
    uint8_t temp[len+1];

    memset(temp, 0, sizeof(temp));

    while(1)
    {
        if(token[i] == '\"' && simple_quotes == 0)
            double_quotes++;
        else if(token[i] =='\'' && double_quotes == 0)
            simple_quotes++;

        temp[indice++] = token[i];

        if(simple_quotes == 2 || double_quotes == 2 || i == len)
            break;

        i++;
    }

    if(simple_quotes < 2 && double_quotes < 2)
    {
        debug("\nError en linea %d: las comillas no estan balanceadas: %s\n", linea,temp);
        return 0;
    }

    if(!agregar_token(&*lexer, temp, CADENA))
        return 0;

    return i;
}

uint8_t get_token_number(uint8_t * token, lexical ** lexer, uint8_t i)
{
    uint8_t indice = 0;
    uint8_t decimal = 0;
    uint8_t len = strlen(token);
    uint8_t temp[len+1];

    memset(temp, 0, sizeof(temp));

    while(1)
    {
        if(i == len || es_operador(token[i]) || es_parentesis(token[i]) || token[i] == ',')
        {
            i--;
            break;
        }
        else if(token[i] == '.')
        {
            temp[indice++] = token[i];
            decimal++;
            if(decimal > 1)
            {
                debug("\nError en linea %d: error de decimal: %s\n", linea, token);
                return 0;
            }
        }
        else if(!isdigit(token[i]) && !es_operador(token[i]) && !es_parentesis(token[i]) && token[i] != ',' && token[i] != '.')
        {
            if(isspace(token[i]))
                break;
            debug("\nError en linea %d: el valor %c no es permitido\n", linea,token[i]);
            return 0;
        }
        else
            temp[indice++] = token[i];
        i++;
    }

    if(decimal == 1)
    {
        if(!agregar_token(&*lexer, temp, FLOAT))
            return 0;
    }
    else if(!agregar_token(&*lexer, temp, NUMERO))
        return 0;

    return i;
} 

uint8_t get_token_funcion_o_var(uint8_t * token, lexical ** lexer, uint8_t i)
{
    uint8_t indice = 0;
    uint8_t len = strlen(token);
    uint8_t temp[len+1];
    uint8_t r_token = 0;
 
    memset(temp, 0, sizeof(temp));

    while(1)
    {
        if(len == i || es_operador(token[i]) || es_parentesis(token[i]) || token[i] == ',')
        {
            i--;
            break;
        }
        else if(!isalpha(token[i]) && !isdigit(token[i]) && !es_operador(token[i]) && !es_parentesis(token[i]) && token[i] != '_' && token[i] != ',')
        {
            if(isspace(token[i]))
                break;
            debug("\nError en linea %d: el valor %c no es permitido\n", linea,token[i]);
            return 0;
        }
        else
           temp[indice++] = token[i];
		
        i++;	
    }

    if(!check_keyword(temp))
        return 0;

    if(strcmp(temp, "print") == 0)
        r_token = PRINT;
    else
        r_token = VARIABLE;

    if(!agregar_token(&*lexer, temp, r_token))
        return 0;

    return i;
}

int len_sin_espacios(uint8_t * cadena)
{
    uint8_t i = 0;

    while(*cadena)
    {
        if(!isspace(*cadena))
            i++;
        *cadena++;
    }

    return i;
}

uint8_t * parser(uint8_t * cadena)
{
    uint8_t i = 0;
    int len = strlen(cadena);
    uint8_t temp[10];
    uint8_t operador = 0;
    uint8_t size = 0;

    memset(temp, 0, sizeof(temp));

    lexical * root;
    lexical * lexer = init_lexer();
    if(lexer == NULL)
        return 0;

    lexer->valor = NULL;
    lexer->token = 0;

    root = lexer;

    while(1)
    {
        if(isspace(cadena[i]))
        {
			/*Por el momento nada. Pero se tiene planeado usar
			para verificar indentaciones
			*/
        }
		else
		{
            if(isalpha(cadena[i]))
                i = get_token_funcion_o_var(cadena, &lexer, i);
            else if(isdigit(cadena[i]))
                i = get_token_number(cadena,&lexer, i);
            else if(es_operador(cadena[i]))
	        {
                temp[0] = cadena[i];
                if(cadena[i] == '=')
                {
                    if(!agregar_token(&lexer, temp, IGUAL))
                    {
                        acomodar_nodos(&lexer, &root);
                        libera_tokens(&lexer, size);
                        liberacion_general();
                        return NULL;
                    }
                }
                else
                {	
                    if(!agregar_token(&lexer, temp, OPERADOR))
                    {
                        acomodar_nodos(&lexer, &root);
                        libera_tokens(&lexer, size);
                        liberacion_general();
                        return NULL;
                    }
                }
                operador++;
                memset(temp, 0, sizeof(temp));
            }
            else if(es_parentesis(cadena[i]))
            {
                temp[0] = cadena[i];
                if(cadena[i] == '(')
                {
                    if(!agregar_token(&lexer, temp, L_PARENTESIS))
                    {
                        acomodar_nodos(&lexer, &root);
                        libera_tokens(&lexer, size);
                        liberacion_general();
                        return NULL;
                    }
                }
                else
                {	
                    if(!agregar_token(&lexer, temp, R_PARENTESIS))
                    {
                        acomodar_nodos(&lexer, &root);
                        libera_tokens(&lexer, size);
                        liberacion_general();
                        return NULL;
                    }
                }
                operador++;
                memset(temp, 0, sizeof(temp));
            }
            else if(cadena[i] == ',')
            {
                temp[0] = cadena[i];
                if(!agregar_token(&lexer, temp, CONCATENACION))
                {
                    acomodar_nodos(&lexer, &root);
                    libera_tokens(&lexer, size);
                    liberacion_general();
                    return NULL;
                }
            }
            else if(cadena[i] == '\"' || cadena[i] == '\'')
                i = check_quotes(cadena,&lexer, i);
            else
            {	
                if(i != len)
                { 
                    debug("\nError: caracter invalido: %c\n", cadena[i]);
                    acomodar_nodos(&lexer, &root);
                    libera_tokens(&lexer, size);
                    liberacion_general();
                    return NULL;
                }
                else if(i == len)
                    break;			
            }

            if(i == 0 && operador == 0)
            {
                acomodar_nodos(&lexer, &root);
                libera_tokens(&lexer, size);
                liberacion_general();
                return NULL;
            }
            size++;
            operador = 0;

        }

        i++;

        if(i == len)
            break;

    }

    len = len_sin_espacios(cadena);

    acomodar_nodos(&lexer, &root);
    uint8_t * cadena_retorno = semantic_analyzer(&lexer, len, size);
    if(cadena_retorno == NULL)
    {
		libera_tokens(&lexer, size);
		liberacion_general();
		return 0;
    }
	
    libera_tokens(&lexer, size);

    return cadena_retorno;
}