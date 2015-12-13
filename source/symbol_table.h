#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "compilador.h"

typedef struct hash_elem_t
{
	struct hash_elem_t * next;
	uint8_t * var_name;
	uint8_t token;
	uint8_t token_function;
	uint8_t * value; 
}hash_elem_t;

typedef struct hashtable_t
{
	unsigned int size;
	hash_elem_t ** table;
}hashtable_t;

typedef struct hash_elem_it
{
	hashtable_t * ht;
	unsigned int index;
	hash_elem_t * elem;
}hash_elem_it;

extern hashtable_t * hash;

#define HT_ITERATOR(ht) {ht, 0, ht->table[0]}

void print_token(uint8_t token);
hashtable_t * ht_create(unsigned int size);
unsigned int ht_calc_hash(uint8_t * key);
uint8_t ht_put(uint8_t * key, uint8_t token, uint8_t token_function,uint8_t * value);
void * ht_remove(uint8_t * key);
uint16_t ht_get_len(uint8_t * key);
uint8_t ht_get(uint8_t * key);
hash_elem_t * ht_iterate(hash_elem_it * iterator);
uint8_t * ht_iterate_keys(hash_elem_it * iterator);
void ht_destroy();
uint8_t * ht_get_value(uint8_t * key);
uint16_t get_len_value(lexical * lexer, uint8_t size);
uint8_t * agregar_valor(lexical * lexer, uint8_t size);
uint8_t update_valor(uint8_t * key, uint8_t * value);
uint8_t update_token_function(uint8_t * key, uint8_t token_function);


#endif
