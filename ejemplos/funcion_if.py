#En python no hay problema en declarar variables dentro de un if
#y usar dichas variables fuera de el. Pero en C si es un problema.
#En C, es valido definir variables dentro de un if o un else, pero
#siempre y cuando estas variables solo sea usadas dentro de dichas funciones
#Un intento de referenciar una variable declarada dentro de un if fuera de este
#generara un error. Por lo tanto, deberas declarar la variable fuera del if.
#Como en el ejemplo

#raw_input

numero = int(raw_input("numero: "))
password = ""

if numero == 100:
	print 'Los numeros coinciden'
	password = raw_input("password: ")
else:
	print 'Los numeros no coinciden'

if password == "secret":
	print 'password correcto'
else:
	print 'password incorrecto'

if 12 != 3:
	print 'si'		
