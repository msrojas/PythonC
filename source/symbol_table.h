#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "compilador.h"

typedef struct hash_elem_t
{
	struct hash_elem_t * next;
	uint8_t * var_name;
	uint8_t token;
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
uint8_t ht_put(uint8_t * key, uint8_t token);
void * ht_remove(uint8_t * key);
uint8_t ht_get(uint8_t * key);
hash_elem_t * ht_iterate(hash_elem_it * iterator);
uint8_t * ht_iterate_keys(hash_elem_it * iterator);
void ht_destroy();

#endif