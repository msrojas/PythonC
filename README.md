# PythonC
Traducir codigo Python a codigo C/Translate Python code to C code
<br>
<h3>Actualizado: 19-diciembre-2015</h3>
PythonC es una herramienta que permite traducir código Python a código C. Ahora cuenta con funciones: función print, definición de variables, concatenaciones, operaciones numéricas, raw_input, int(), float(), len(), <strong>if</strong>, <strong>else</strong>, operadores logicos, elif, while, imprimir indices de cadenas, free, listas y bucle for. Pero no hay de que preocuparse, iremos actualizando el código lo más pronto posible.
<br>
Los if anidados no fueron probados al 100. Por lo tanto si encuentras un error, favor de avisar a cualquiera de las direcciones que se encuentran abajo.<br>
<strong>Ahora ya es posible usar raw_input para capturar cadenas dentro de un while sin problemas. Pero para ello tendrás que ver la documentación</strong><br>
<strong>Las listas aun estan en progreso. Por el momento, solo se permite declararlas e imprimirlas. El bucle for solo acepta la sintaxis basica: iterar cadenas, listas y la función range.</strong>
<br><br>
Para traducir código eficientemente tendrás que darle un vistazo a la documentación: <a href="https://pythoncblog.wordpress.com/documentacion/">link</a>
<br>
Solo traduce código Python 2.7
<br>

<h3>English: Translate Python code to C code</h3>
PythonC is a tool that generate C code using python. For the moment the tool count with the functions: print, definition of variables, concatenations, number operations, int(), float(), len(), if , else, logical operators, elif, while, print one char of a variable (print var[1]), free variables, for and lists. But not worry, we’ll go updating the most soon possible.<br><br>
To translate the code efficiently, you will have to see the documentation: <a href="https://pythoncblog.wordpress.com/documentation/">link</a><br><br>

<h3>Compilar/Compile</h3>
Para compilar solo ejecuta el makefile que viene dentro de la carpeta source: make compilador<br>
To compile just execute the makefile. You will find it in the directory source: make compilador<br><br>
Dentro de la carpeta source hay otra carpeta llamada "include". Dicho directorio contines los archvios necesarios para compilar los archivos ".c" generados. Dentro de la carpeta viene las instrucciones para compilarlos.<br>
Inside of the directory source there is another directory called "include". In that directory you will find the files necessary to compile the files ".c" generates.
<br>
Para ejecutar/To execute: ./compilador ejemplo.py

<h3>Bugs</h3>
Error corregido al momento de concatenar
<br>
Error corregido en definicion de variables
<br>
Buffer overflow corregido<br>
Error corregido en if_grammar.c<br>
Error corregido en definida_en_comentario.c<br>
Memory leak corregido en run_compiler<br>
Error corregido al momento de concatenar variables con cadenas<br>
Memory leak corregido en raw_input<br>
Memory leak corregido en run_compiler.c (otro mas)<br>
Error corregido al definir variables<br>
Error corregido al concatenar variables (otro mas)<br>
Error corregido en print_grammar.c
Error corregido en run_compiler.c <br>
<br>
<br>
Si llegas a encontrar un bug o quieres aportar, favor de comunicarte en cualquiera de las dos direcciones:
<br>
alez011@hotmail.com
<br>
python_c@hotmail.com
