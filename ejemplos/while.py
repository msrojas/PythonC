#raw_input

#raw_input almacena memoria en la heap. Por lo tanto, deberas decirle
#mediante un comentario cuando liberar la variable:
##free(variable) (solo con un '#')
#NOTA: ESO SOLO ES NECESARIO CUANDO LLAMAS RAW_INPUT PARA ALMACENAR CARACTERES DENTRO DE UN WHILE
#Si ese no es el caso, el compilador sera el encargado de liberar la variable

#raw_input storage memory in heap. Therefore, you will have to write the next instruction
##free(variable) (just with one '#')
#NOTE: THIS IS ONLY NECESSARY WHEN YOU CALL RAW_INPUT TO SAVE STRINGS INSIDE OF WHILE
#If that is not the case, the compiler would be the responsible to free the variable

i = 0
while i < 10:    
	cadena1 = raw_input()   #Hemos declarado la variable dentro del while/We've declared the variable inside of wile
	print cadena1           #Por lo tanto, solo se podra usar dentro de ella/Therefore, we can call the variable only inside of while   
	i = i + 1
	#free(cadena1)          

#Hemos declarado la variable fuera del while, por lo tanto, tal vez no sea necesario liberarala usando: ##free()
#Ya que el compilador tiene una manera mas sencilla de acceder a ella

#We've declared the variable outside of while, therefore, may be is not necessary using: ##free()
#Since, the compiler has a way easier to access the variable

cadena2 = ""  
i = 0
while i < 10:
	cadena2 = raw_input()
	print cadena2
	i = i + 1
