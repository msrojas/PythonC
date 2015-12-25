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
#include <unistd.h>

hashtable_t * hash = NULL;
free_table_t * free_hash = NULL;
uint32_t linea = 1;
uint8_t fixed_width = 0;
var_definidas * definir = NULL;
uint8_t indentacion = 0;
uint8_t list_existe = 0;

void error_delete_file(uint8_t * name)
{
    uint8_t len_archivo = 0;
    len_archivo = strlen(name);

    uint8_t temp_name[len_archivo+1];
    memset(temp_name, 0, sizeof(temp_name));

    strcat(temp_name,name);
    strcat(temp_name, ".c");

    if(unlink(temp_name) == -1)
        debug("[-] Error al remover el archivo .c\n");
}

void agregar_info_final(FILE * archivo)
{
    if(indentacion > 0)
    {
        uint8_t i=0;
        uint8_t temp_indentacion = indentacion;
        for(i=0;i<indentacion;i++)
        {
        	fprintf(archivo, "%*s\n", 4*temp_indentacion+1, "}");
        	temp_indentacion--;
        }
    }

    if(free_hash != NULL)
    {   
        free_vars_compile(archivo);
        free_destroy();
    }

    fprintf(archivo, "\n%*s\n", strlen("return 0;")+4,"return 0;");
    fprintf(archivo, "}");
}

void print_output_in_archivo(FILE * archivo, output_code * codigo)
{
    uint16_t len = strlen(codigo->cadena_retorno);
    uint16_t i=0;
    uint8_t indice = 0; 
    uint8_t temp[len+1];
    memset(temp, 0, sizeof(temp));

    for(i=0;i<len;i++)
    {
        if(codigo->cadena_retorno[i] == '\n')
        {   
            if(temp[0] == '}' && codigo->temp_indentacion > 0)
            {
                fprintf(archivo, "%*s\n", codigo->temp_indentacion*4+1, "}");
                codigo->temp_indentacion--;
            }
            else 
            {
                fprintf(archivo, "%*s\n", indice+codigo->indentaciones,temp);
            }
            indice = 0;
            memset(temp, 0, sizeof(temp));
        }
        else
            temp[indice++] = codigo->cadena_retorno[i];
    }

    fprintf(archivo, "%*s\n", indice+codigo->indentaciones,temp);
}

symbol_table_sizes * get_size_file(FILE * archivo_output)
{
    uint32_t c = 0;
    uint16_t linea_l = 0;
    uint16_t comentario = 0;
    uint16_t indice = 0;

    while((c = fgetc(archivo_output)) != EOF)
    {
        if(c == '#' && indice == 0)
            comentario++;
        else if(c == '\n')
        {
            linea_l++;
            indice = 0;
        }
        else 
            indice++;
    }

    if(indice > 0 && linea_l == 0)
        linea_l++;

    comentario *= sizeof(var_elemento *);
    linea_l *= sizeof(hash_elem_t*);

    symbol_table_sizes * symbol = (symbol_table_sizes *)malloc(sizeof(symbol_table_sizes));
    if(symbol == NULL)
    {
        printf("Error: oucrrio un inconveniente en la heap\n");
        return NULL;
    }

    symbol->symbol_variable = linea_l;
    symbol->symbol_comentario_var = comentario;
    return symbol;
}

uint8_t informacion_basica(FILE * archivo_output, FILE * archivo)
{
    print_como_compilar(archivo_output);
    fprintf(archivo_output, "#include <stdio.h>\n");
    fprintf(archivo_output, "#include <stdlib.h>\n");
    fprintf(archivo_output, "#include <string.h>\n");
    if(fixed_width == 1)
        fprintf(archivo_output, "#include <inttypes.h>\n");
    if(!buscar_funciones(archivo, archivo_output))
    	return 0;
    print_strchar(archivo_output);
    fprintf(archivo_output, "\nint main()\n{\n");

    return 1;
}

void modificar_nombre(char * name, uint8_t len_archivo)
{
    name[len_archivo-3] = '\0';
    name[len_archivo-2] = '\0';
    name[len_archivo-1] = '\0';
}

FILE * crear_archivo_output(char * name)
{
    uint8_t len_archivo = strlen(name);
    modificar_nombre(name,len_archivo);

    uint8_t temp_name[len_archivo+1];
    memset(temp_name, 0, sizeof(temp_name));

    strcat(temp_name, name);
    strcat(temp_name, ".c");

    FILE * archivo_output = fopen(temp_name, "w");

    if(archivo_output == NULL)
    {
        debug("Ocurrio un error al crear el archivo .c\n");
        return NULL;
    }

    printf("[+] Output: %s\n", temp_name);

    return archivo_output;
}

uint8_t check_ext_name(char * name)
{
    uint8_t len = strlen(name);

    if(name[len-3] == '.' && name[len-2] == 'p' && name[len-1] == 'y')
        return 1;
    else
    {
        debug("[-] El nombre no tiene extension .py\n");
        return 0;
    }
}

uint16_t get_espacios(uint8_t * cadena)
{
    uint16_t len = strlen(cadena);
    uint16_t espacios = 0;
    uint16_t i = 0;

    for(i=0;i<len;i++)
    {
        if(isspace(cadena[i]))
            espacios += 4;
        else if(cadena[i] == '\t')
            espacios += 4;
        else
        {
            if(espacios != 0 && !check_indentacion(espacios))
                return 0;
            if(espacios == 0)
                espacios = 4;
            else
                espacios += 4;
            return espacios;
        }
    }
}

datos_comentarios * init_datos_comentarios(uint16_t len, uint8_t token) //NUEVO
{
    datos_comentarios * datos = malloc(sizeof(datos_comentarios));
    if(datos == NULL)
    {
    	debug("\nError: oucrrio un inconveniente en la heap\n");
    	return NULL;
    }

    datos->index = len;
    datos->token = token;
    return datos;
}

datos_comentarios * comentario_match(uint8_t * cadena) //NUEVO
{
    uint16_t len = strlen(cadena);
    uint8_t temp[len+1];
    memset(temp, 0, sizeof(temp));

    uint16_t i=0;
    uint16_t indice = 0; //NUEVO
    uint8_t chars = 0;
    for(i=0;i<len;i++)
    {
        if(isspace(cadena[i]) || cadena[i] == '\t' || cadena[i] == '(') //NUEVO
        {
            if((isspace(cadena[i]) || cadena[i] == '\t') && chars == 1)
                break;
            else if(cadena[i] == '(')
                break;
        }
        else
        {
            if(chars == 0)
                chars++;
            temp[indice++] = cadena[i];
        }
    }

    datos_comentarios * datos = NULL;

    if(strcmp(temp, "#char") == 0)
    {
        datos = init_datos_comentarios(i, CHAR);
        if(datos == NULL)
            return NULL;
    }
    else if(strcmp(temp, "#float") == 0)
    {
        datos = init_datos_comentarios(i, FLOAT);
        if(datos == NULL)
            return NULL;
    }
    else if(strcmp(temp, "#free") == 0) //NUEVO
    {
        datos = init_datos_comentarios(i, FREE);
        if(datos == NULL)
            return NULL;
    }

    return datos;
}

uint8_t busca_comentario(uint8_t * comentario) //NUEVO
{
    uint16_t len = strlen(comentario);
    uint16_t i = 0;
    uint8_t chars = 0;
    for(i=0;i<len;i++)
    {
        if(isspace(comentario[i]) || comentario[i] == '\t')
        {
            //if(chars)
        }
        else if(comentario[i] == '#')
        {
            if(chars == 0)
                return 1;
        }
        else
        	return 0;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    uint8_t ret = 0;

    if(argc < 2)
    {
        debug("[-] Debes pasar un archivo. Ejemplo:\n");
        debug("%s archivo.py\n", argv[0]);
        exit(1);
    }
    else if(argc == 2)
    {
        if(strcmp(argv[1], "-fw") == 0)
        {
            debug("[-] Falta el nombre del archivo\n");
            exit(1);
        }
        else
        { 
            if(!check_ext_name(argv[1]))
                exit(1);
            if(access(argv[1], F_OK) == -1)
            {
                debug("[-] El archivo no existe\n");
                exit(1);
            }
        }
    }
    else if(argc == 3)
    {
        if(strcmp(argv[1], "-fw") == 0)
        {
            if(!check_ext_name(argv[2]))
                exit(1);
            if(access(argv[2], F_OK) == -1)
            {
                debug("[-] El archivo no existe: %s\n", argv[2]);
                exit(1);
            }
            ret = 1;
        }
        else
        {
            if(!check_ext_name(argv[1]))
                exit(1);
            if(access(argv[1], F_OK) == -1)
            {
                debug("[-] El archivo no existe: %s\n", argv[1]);
                exit(1);
            }
			if(strcmp(argv[2], "-fw") != 0)
            {
                debug("\n[-] Se desconoce: %s\n", argv[2]);
                exit(1);
            }
        }
        fixed_width = 1;
    }
    else if(argc > 3)
    {
        debug("\n[-] Demasiados argumentos\n");
        exit(1);
    }

    FILE * archivo = NULL;

    if(ret == 1)
        archivo = fopen(argv[2], "r");
    else 
        archivo = fopen(argv[1], "r");
    if(archivo == NULL)
    {
        debug("Ocurrio un error al abrir el archivo\n");
        exit(1);
    }

    if(ret == 1)
        printf("[+] Entrada: %s\n", argv[2]);
    else
        printf("[+] Entrada: %s\n", argv[1]);

    FILE * archivo_output = NULL;
    if(ret == 1)
        archivo_output = crear_archivo_output(argv[2]);
    else
        archivo_output = crear_archivo_output(argv[1]);
    if(archivo_output == NULL)
    {
        fclose(archivo);
        exit(1);
    }

    symbol_table_sizes * symbol = get_size_file(archivo);
    if(symbol == NULL)
    {
        fclose(archivo);
        fclose(archivo_output);
        exit(1);
    }

    ht_create(symbol->symbol_variable);
    free_create(symbol->symbol_variable);
    if(symbol->symbol_comentario_var > 0)
        init_definir(symbol->symbol_comentario_var);

    fseek(archivo, 0L, SEEK_SET);
    if(!informacion_basica(archivo_output, archivo))
    {
        fclose(archivo);
        fclose(archivo_output);
        free(symbol);
        ht_destroy();
        if(ret == 1)
            error_delete_file(argv[2]);
        else
           	error_delete_file(argv[1]);
        exit(1);
    }

    short len = 0;
    uint8_t cadena[200];
    output_code * codigo = NULL;
    memset(cadena, 0, sizeof(cadena));

    fseek(archivo, 0L, SEEK_SET);

    while(fgets(cadena, sizeof(cadena), archivo) != NULL)
    {
        len = strlen(cadena);
        if(len > 1 && cadena[len-1] == '\n')
            cadena[len-1] = '\0';

        if(cadena[0] == '#' || busca_comentario(cadena)) //NUEVO
        {
        	datos_comentarios * datos = comentario_match(cadena);
            if(datos != NULL && (datos->token == CHAR || datos->token == FLOAT))
            {
                if(!verificar_comentario(cadena, datos))
                {
                    fclose(archivo_output);
                    fclose(archivo);
                    liberacion_general();
                    free(symbol);

                    if(ret == 1)
                        error_delete_file(argv[2]);
                    else
                        error_delete_file(argv[1]);
                    exit(1);
                }
            }
            else if(datos != NULL && datos->token == FREE)
            {
                uint8_t * retorna = comentario_free(cadena, datos->index);
                if(retorna == NULL)
                {
                    fclose(archivo_output);
                    fclose(archivo);
                    liberacion_general();
                    free(symbol);

                    if(ret == 1)
                        error_delete_file(argv[2]);
                    else
                        error_delete_file(argv[1]);
                    exit(1);
                }
                uint16_t espacios = get_espacios(cadena);
                if(espacios == 0)
                {
                    fclose(archivo_output);
                    fclose(archivo);
                    liberacion_general();
                    free(symbol);

                    if(ret == 1)
                        error_delete_file(argv[2]);
                    else
                        error_delete_file(argv[1]);
                    exit(1);
                }
                espacios -= 4;
                output_code * codigo_temp = get_output(retorna, espacios);
                if(codigo_temp == NULL)
                {
                    fclose(archivo_output);
                    fclose(archivo);
                    free(symbol);

                    if(ret == 1)
                        error_delete_file(argv[2]);
                    else
                        error_delete_file(argv[1]);
                    exit(1);
                }
                print_output_in_archivo(archivo_output, codigo_temp);
                free(codigo_temp->cadena_retorno);
                free(codigo_temp);
                free(retorna);
            }
            if(datos != NULL)
                free(datos);
            memset(cadena, 0, sizeof(cadena));
            linea++;
            continue;
        }
        else if((strcmp(cadena, "\n") == 0) || (cadena[0] == '\t' && len == 2))
        {
            fprintf(archivo_output, "\n");
            memset(cadena, 0, sizeof(cadena));
            linea++;
            continue;
        }
        codigo = parser(cadena);
        if(codigo == NULL)
        {
            fclose(archivo_output);
            fclose(archivo);
            free(symbol);

            if(ret == 1)
                error_delete_file(argv[2]);
            else
                error_delete_file(argv[1]);
            exit(1);
        }
        //printf("%s\n", codigo->cadena_retorno);
        print_output_in_archivo(archivo_output, codigo);
        free(codigo->cadena_retorno);
        free(codigo);
        codigo = NULL;
        memset(cadena, 0, sizeof(cadena));
        linea++;
    }

    printf("[+] Agregando datos finales\n");
    agregar_info_final(archivo_output);
    if(hash != NULL)
        ht_destroy();
    if(definir != NULL)
        var_destroy();
    free(symbol);

    fclose(archivo_output);
    fclose(archivo);

    printf("[+] Traduccion hecha con exito\n");

    return 0;
}
