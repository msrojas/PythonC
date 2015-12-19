compilador:libcomp.a libvari.a libprint.a libsintax.a libsymbol.a libfree.a libdefinida.a libraw.a libif.a libfor.a
	gcc run_compiler.c -o compilador -I. libcomp.a -I. libvari.a -I. libprint.a -I. libsintax.a -I. -I. libsymbol.a -I. libfree.a -I. libdefinida.a -I. libraw.a -I. libif.a -I. libfor.a

libcomp.a:compilador.c
	gcc -c compilador.c -o compilador.o
	ar rcs libcomp.a compilador.o

libvari.a:variable_grammar.c
	gcc -c variable_grammar.c -o variable_grammar.o
	ar rcs libvari.a variable_grammar.o

libprint.a:print_grammar.c
	gcc -c print_grammar.c -o print_grammar.o
	ar rcs libprint.a print_grammar.o		

libsintax.a:sintaxis_basica.c
	gcc -c sintaxis_basica.c -o sintaxis_basica.o
	ar rcs libsintax.a sintaxis_basica.o

libsymbol.a:symbol_table.c
	gcc -c symbol_table.c -o symbol_table.o
	ar rcs libsymbol.a symbol_table.o

libfree.a:free_lib.c
	gcc -c free_lib.c -o free_lib.o
	ar rcs libfree.a free_lib.o

libdefinida.a:definida_en_comentario.c
	gcc -c definida_en_comentario.c -o definida_en_comentario.o
	ar rcs libdefinida.a definida_en_comentario.o

libraw.a:raw_input_grammar.c
	gcc -c raw_input_grammar.c -o raw_input_grammar.o
	ar rcs libraw.a raw_input_grammar.o
	
libif.a:if_grammar.c
	gcc -c if_grammar.c -o if_grammar.o
	ar rcs libif.a if_grammar.o

libfor.a:for_grammar.c
	gcc -c for_grammar.c -o for_grammar.o
	ar rcs libfor.a for_grammar.o

clean:
	rm 	compilador.o variable_grammar.o print_grammar.o sintaxis_basica.o symbol_table.o free_lib.o definida_en_comentario.o raw_input_grammar.o if_grammar.o for_grammar.o				
