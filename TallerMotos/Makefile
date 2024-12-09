# Nombre del archivo ejecutable
EXEC = motos

# Compilador y opciones de compilación
CC = gcc
CFLAGS = -pthread -o $(EXEC)

# Archivo fuente
SRC = tallerMotos.c

# Regla para compilar el programa
all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(SRC) $(CFLAGS)

# Regla para ejecutar el programa
run: $(EXEC)
	./$(EXEC)

# Regla para limpiar archivos generados
clean:
	rm -f $(EXEC)
