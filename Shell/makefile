CC = gcc
SRC1 = "cat.c"
SRC2 = "help.c"
SRC3 = "ls.c"
SRC4 = "mkdir.c"
SRC5 = "chmod.c"
SRC6 = "rmdir.c"
SRC7 = "touch.c"
SRC8 = "main.c"

Program:
	echo "Iniciando la compilacion de los archivos."
	$(CC) $(SRC1) -o cat
	$(CC) $(SRC2) -o help
	$(CC) $(SRC3) -o ls
	$(CC) $(SRC4) -o mkdir
	$(CC) $(SRC5) -o chmod
	$(CC) $(SRC6) -o rmdir
	$(CC) $(SRC7) -o touch
	$(CC) $(SRC8) -o main

	echo "Finalizo la compilacion de los archivos."

run:
	./main
