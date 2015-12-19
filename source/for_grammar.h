#ifndef FOR_GRAMMAR_H
#define FOR_GRAMMAR_H

#include "compilador.h"

uint8_t * get_rango_stop(lexical * lexer, uint8_t size);
uint8_t verificar_rango_stop(lexical * lexer, uint8_t size);
uint8_t * get_rango_origen(lexical * lexer, uint8_t size);
uint16_t get_len_list_origen(lexical * lexer, uint8_t size);
uint16_t get_var_origen(lexical * lexer, uint8_t size);
uint8_t * print_original_for(lexical * lexer, uint8_t size, uint8_t token_type);
uint8_t check_range_grammar(lexical * lexer, uint8_t size);
uint8_t check_for_grammar(lexical * lexer, uint8_t size);

#endif
