#include "list.h"

struct list * GetNewNode(struct type_value * values)
{
	struct list * newNode = malloc(sizeof(struct list));
	switch(values->type)
	{
		case INT:
			newNode->data_type = INT;
			newNode->union_data_types.valor_int = values->union_types.int_value;
			break;
		case FLOAT:
			newNode->data_type =FLOAT;
			newNode->union_data_types.valor_float = values->union_types.float_value;
			break;
		case CHAR:
			newNode->data_type = CHAR;
			newNode->union_data_types.valor_pointer = malloc(strlen(values->union_types.pointer_value)+1);
			if(newNode->union_data_types.valor_pointer == NULL)
			{
				printf("Error: ocurrio un inconveniente en la heap\n");
				exit(1);
			}			
			strcpy(newNode->union_data_types.valor_pointer, values->union_types.pointer_value);
			break;
		default:
			printf("DESCONOCIDO\n");
			break;		
	}
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

void InsertAtTail(struct list * head, struct type_value * values)
{
    struct list * temp = head;
    struct list * newNode = GetNewNode(values);
    while(temp->next != NULL)
    	temp = temp->next;
    temp->next = newNode;
    newNode->prev = temp;
}

void Print(struct list * current)
{
	while(current != NULL)
	{
		switch(current->data_type)
	    {
	    	case INT:
	    		printf("%d\n", current->union_data_types.valor_int);
	    		break;
	    	case FLOAT:
	    		printf("%f\n", current->union_data_types.valor_float);
	    		break;
	    	case CHAR:
	    		printf("%s\n", current->union_data_types.valor_pointer);
	    		break;
	    	default:
	    		printf("VALOR DESCONOCIDO\n");
	    		break;			
	    }
		current = current->next;
	}
}

void print_indice_en_especifico(struct list * current, int indice)
{
	int i = 0;
	while(current != NULL)
	{
		if(indice == i)
			break;
	    current = current->next;
	    i++;
	}

	if(current == NULL)
	{
	    printf("Indice fuera de rango\n");
	    return;
	}
	else if(indice == i)
	{
		switch(current->data_type)
	    {
	    	case INT:
	    		printf("%d\n", current->union_data_types.valor_int);
	    		break;
	    	case FLOAT:
	    		printf("%f\n", current->union_data_types.valor_float);
	    		break;
	    	case CHAR:
	    		printf("%s\n", current->union_data_types.valor_pointer);
	    		break;
	    	default:
	    		printf("VALOR DESCONOCIDO\n");
	    		break;			
	    }
	}
}

void print_indice(struct list * current)
{
	if(current == NULL)
	{
		printf("Indice fuera de rango\n");
		exit(1);
	}
	switch(current->data_type)
	{
	    case INT:
	    	printf("%d\n", current->union_data_types.valor_int);
	    	break;
	    case FLOAT:
	    	printf("%f\n", current->union_data_types.valor_float);
	    	break;
	    case CHAR:
	    	printf("%s\n", current->union_data_types.valor_pointer);
	    	break;
	    default:
	    	printf("VALOR DESCONOCIDO\n");
	    	break;			
	}
}

void destroy_list(struct list * current)
{
	struct list * temp = NULL;
	while(current != NULL)
	{
		temp = current;
		current = current->next;
		if(temp->data_type == CHAR)
			free(temp->union_data_types.valor_pointer);
		free(temp);
	}
}

struct type_value * create_value(int type, void * data)
{
	int numero_int = 0;
	double numero_float = 0;
	char * pointer = NULL;
	struct type_value * value = NULL;

	switch(type)
	{
		case INT:
			numero_int = (intptr_t)data;
			value = malloc(sizeof(struct type_value));
			if(value == NULL)
			{
				printf("Error: ocurrio un inconveniente en la heap\n");
				exit(1);
			}
			value->type = INT;
			value->union_types.int_value = numero_int;
			break;
		case FLOAT:
			numero_float = *(double *)data;
			value = malloc(sizeof(struct type_value));
			if(value == NULL)
			{
				printf("Error: ocurrio un inconveniente en la heap\n");
				exit(1);
			}
			value->type = FLOAT;
			value->union_types.float_value = numero_float;
			break;
		case CHAR:
			pointer = (char *)malloc(strlen(data)+1);
			strcpy(pointer, (char*)data);
			value = malloc(sizeof(struct type_value));
			if(value == NULL)
			{
				printf("Error: ocurrio un inconveniente en la heap\n");
				exit(1);
			}
			value->type = CHAR;
			value->union_types.pointer_value = malloc(strlen(pointer)+1);
			if(value->union_types.pointer_value == NULL)
			{
				printf("Error: ocurrio un inconveniente en la heap\n");
				exit(1);
			}
			strcpy(value->union_types.pointer_value, pointer);
			free(pointer);
			break;
		default:
			printf("DESCONOCIDO\n");
			break;			
	}

	return value;
}

void agregar_elementos(struct list * head,int numeros, ...)
{
    va_list argumentos;
    va_start(argumentos, numeros);
    int numero_int = 0;
    double numero_float = 0;
    struct type_value * values = NULL;

    int type = 0;
    int i=0;
    for(i=0;i<numeros;i++)
    {
    	type = va_arg(argumentos, enum Types);
    	switch(type)
    	{
    		case INT:
    			numero_int = va_arg(argumentos, int);
    			values = create_value(INT,(void *)(intptr_t)numero_int);
    			if(values == NULL)
    			{
    				printf("Error al almacenar la lista\n");
    				exit(1);
    			}
    			InsertAtTail(head,values);
    			free(values);
    			break;
    		case FLOAT:
    			numero_float = va_arg(argumentos, double);
    			void * p_v;
    			double * numero_temp = malloc(sizeof(double));
    			*numero_temp = numero_float;
    			p_v = (void *)numero_temp;
    			values = create_value(FLOAT, p_v);
    			if(values == NULL)
    			{
    				printf("Error al almacenar la lista\n");
    				exit(1);
    			}
    			free(numero_temp);
    			InsertAtTail(head,values);
    			free(values);
    			break;
    		case CHAR:
    			values = create_value(CHAR,(void *)va_arg(argumentos, char *));
    			if(values == NULL)
    			{
    				printf("Error al almacenar la lista\n");
    				exit(1);
    			}
    			InsertAtTail(head,values);
    			free(values->union_types.pointer_value);
    			free(values);
    			break;	
    		default:
    			printf("DESCONOCIDO\n");
    			break;		
    	}
    }
    va_end(argumentos);
}

struct list * init_list()
{
	struct list * head = malloc(sizeof(struct list));
	if(head == NULL)
	{
		printf("Error: ocurrio un inconveniente en la heap\n");
		exit(1);
	}
	head->next = NULL;
	head->prev = NULL;
	return head;
}
