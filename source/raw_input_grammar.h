#ifndef RAW_INPUT_GRAMMAR_H
#define RAW_INPUT_GRAMMAR_H

#include "compilador.h"

uint8_t * print_original_input(lexical * lexer, uint8_t size, uint8_t token);
uint8_t check_sintax_input(lexical * lexer, uint8_t size, uint8_t token_type);
uint8_t check_raw_input_grammar(lexical * lexer, uint8_t size, uint8_t token_type);

#endif
