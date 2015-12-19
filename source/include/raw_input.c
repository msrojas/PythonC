#include "raw_input.h"

char * raw_input(char * input)
{
    char c;
    printf("%s", input);
    short size = 100;
    short indice = 0;
    char * get_input = (char *)malloc(size);
    if(get_input == NULL)
    {
        printf("Error: ocurrio un inconveniente en la heap\n");
        exit(1);
    }

   while((c = getchar()) != EOF && c != '\n')
    {
        if(indice >= size )
        {
            size += 50;
            get_input = (char *)realloc(get_input, size);
            if(get_input == NULL)
            {
                printf("Error: ocurrio un inconveniente en la heap\n");
                exit(1);
            }
        }
        get_input[indice++] = c;
    }

    get_input[indice] = '\0';
    return get_input;
}

int raw_input_int(char * input)
{
    char * get_input = raw_input(input);
    int numero = atoi(get_input);
    free(get_input);
    return numero;
}

float raw_input_float(char * input)
{
    char * get_input = raw_input(input);
    float numero = atof(get_input);
    free(get_input);
    return numero;
}
