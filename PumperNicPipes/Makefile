# Nombre del ejecutable
TARGET = pumper_nic

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall 

# Archivos fuente
SRC = pumperPIPES.c
TARGET = pumper_nic

# Regla para compilar el programa
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)

# Regla para recompilar y ejecutar
rebuild: clean $(TARGET)
	./$(TARGET)
