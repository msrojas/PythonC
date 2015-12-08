# PythonC
Traducir codigo Python a codigo C/Translate Python code to C code
<br>
<h3>Actualizado: 8-diciembre-2015</h3>
PythonC es una herramienta que permite traducir código Python a código C. Ahora cuenta con funciones: función print, definición de variables, concatenaciones, operaciones numéricas, raw_input, int(), float(), len(), <strong>if</strong>, <strong>else</strong>, operadores logicos, elif, while e imprimir indices de cadenas. Pero no hay de que preocuparse, iremos actualizando el código lo más pronto posible.
<br>
Los if anidados no fueron probados al 100. Por lo tanto si encuentras un error, favor de avisar a cualquiera de las direcciones que se encuentran abajo.<br>
<strong>Debes tener precaucion de como usar raw_input en el while. Si usas raw_input dentro de un while para capturar numeros, no hay problema. Pero si lo utilizas para capturar cadenas, si los habra, ya que aun no he implementado un metodo para liberar la variable en cada repeticion</strong>
<br><br>
Para traducir código eficientemente tendrás que darle un vistazo a la documentación: <a href="https://pythoncblog.wordpress.com/documentacion/">link</a>
<br>
Solo traduce código Python 2.7
<br>

<h3>Compilar</h3>
Para compilar solo ejecuta el makefile que viene dentro de la carpeta source: make compilador
<br>
Para ejecutar: ./compilador ejemplo.py

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
Memory leak corregido en run_compiler.c (otro mas)
<br>
<br>
Si llegas a encontrar un bug o quieres aportar, favor de comunicarte en cualquiera de las dos direcciones:
<br>
alez011@hotmail.com
<br>
python_c@hotmail.com
