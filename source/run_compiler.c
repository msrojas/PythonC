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
free_struct * free_variables = NULL;
free_struct * free_root = NULL;
uint32_t size_free = 0;
uint32_t linea = 1;
uint8_t fixed_width = 0;
var_definidas * definir = NULL;

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
    if(free_variables != NULL)
    {   
        acomodar_free_vars();
        free_vars_compile(archivo);
        libera_free_vars();
    }

    fprintf(archivo, "\n%*s\n", strlen("return 0;")+4,"return 0;");
    fprintf(archivo, "}");
}

void print_output_in_archivo(FILE * archivo, uint8_t * output)
{
    uint16_t len = strlen(output);
    uint16_t i=0;
    uint8_t indice = 0; 
    uint8_t temp[len+1];
    memset(temp, 0, sizeof(temp));

    for(i=0;i<len;i++)
    {
        if(output[i] == '\n')
        {
            fprintf(archivo, "%*s\n", indice+4,temp);
            memset(temp, 0, sizeof(temp));
            indice = 0;
        }
        else
            temp[indice++] = output[i];
	}

    fprintf(archivo, "%*s\n", indice+4,temp);
}

symbol_table_sizes * get_size_file(FILE * archivo_output)
{
    uint32_t c = 0;
    uint16_t linea = 0;
    uint16_t comentario = 0;
    uint16_t indice = 0;

    while((c = fgetc(archivo_output)) != EOF)
    {
        if(c == '#' && indice == 0)
            comentario++;
        else if(c == '\n')
        {
            linea++;
            indice = 0;
        }
        else 
            indice++;
    }

    comentario *= sizeof(var_elemento *);
    linea *= sizeof(hash_elem_t*);

    symbol_table_sizes * symbol = (symbol_table_sizes *)malloc(sizeof(symbol_table_sizes));
    if(symbol == NULL)
    {
        printf("Error: oucrrio un inconveniente en la heap\n");
        return NULL;
    }

    symbol->symbol_variable = linea;
    symbol->symbol_comentario_var = comentario;
    return symbol;
}

void informacion_basica(FILE * archivo)
{
    fprintf(archivo, "#include <stdio.h>\n");
    fprintf(archivo, "#include <stdlib.h>\n");
    fprintf(archivo, "#include <string.h>\n");
    if(fixed_width == 1)
        fprintf(archivo, "#include <inttypes.h>\n");
    fprintf(archivo, "\nint main()\n{\n");
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

    informacion_basica(archivo_output);

    short len = 0;
    uint8_t cadena[200];
    uint8_t * cadena_output = NULL;
    uint8_t indentacion = 0;
    memset(cadena, 0, sizeof(cadena));

    fseek(archivo, 0L, SEEK_SET);

    while(fgets(cadena, sizeof(cadena), archivo) != NULL)
    {
        len = strlen(cadena);
        if(len > 1 && cadena[len-1] == '\n')
            cadena[len-1] = '\0';

        if(cadena[0] == '#')
        {
            if(!verificar_comentario(cadena, symbol->symbol_comentario_var))
           {
                fclose(archivo_output);
                fclose(archivo);

                if(ret == 1)
                    error_delete_file(argv[2]);
                else
                    error_delete_file(argv[1]);
                exit(1);
            }
            memset(cadena, 0, sizeof(cadena));
            linea++;
            continue;
        }
        else if(strcmp(cadena, "\n") == 0)
        {
            fprintf(archivo_output, "\n");
            memset(cadena, 0, sizeof(cadena));
            linea++;
            continue;
        }
        cadena_output = parser(cadena);
        if(cadena_output == NULL)
        {
            fclose(archivo_output);
            fclose(archivo);

            if(ret == 1)
                error_delete_file(argv[2]);
            else
                error_delete_file(argv[1]);
            exit(1);
        }
        print_output_in_archivo(archivo_output, cadena_output);
        free(cadena_output);
        cadena_output = NULL;
        memset(cadena, 0, sizeof(cadena));
        linea++;
    }

    printf("[+] Agregando datos finales\n");
    agregar_info_final(archivo_output);
    ht_destroy();
    if(definir != NULL)
        var_destroy();
    free(symbol);

    fclose(archivo_output);
    fclose(archivo);

    printf("[+] Traduccion hecha con exito\n");

    return 0;
}