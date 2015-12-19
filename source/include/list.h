#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

enum Types{INT=1,FLOAT,CHAR};

struct list
{
	int data_type;
	union
	{
		int valor_int;
		double valor_float;
		char * valor_pointer;
	}union_data_types;
	struct list * next;
	struct list * prev;
};

struct type_value
{
	int type;
	union
	{
		int int_value;
		double float_value;
		char * pointer_value;
	}union_types;
};

struct list * GetNewNode(struct type_value * values);
void InsertAtTail(struct list * head, struct type_value * values);
void Print(struct list * current);
void print_indice_en_especifico(struct list * current, int indice);
void print_indice(struct list * current);
void destroy_list(struct list * current);
struct type_value * create_value(int type, void * data);
void agregar_elementos(struct list * head,int numeros, ...);
struct list * init_list();

#endif
