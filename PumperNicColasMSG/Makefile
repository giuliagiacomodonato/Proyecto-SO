CC = gcc
CFLAGS = -Wall 

SRCS = main.c cliente.c despachador.c hamburguesa.c papas.c vegano.c
EXECS = main cliente despachador hamburguesa papas vegano

.PHONY: all clean run

all: $(EXECS)

main: main.c estructura.h
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS)

cliente: cliente.c estructura.h
	$(CC) $(CFLAGS) cliente.c -o cliente $(LDFLAGS)

despachador: despachador.c estructura.h
	$(CC) $(CFLAGS) despachador.c -o despachador $(LDFLAGS)

hamburguesa: hamburguesa.c estructura.h
	$(CC) $(CFLAGS) hamburguesa.c -o hamburguesa $(LDFLAGS)

papas: papas.c estructura.h
	$(CC) $(CFLAGS) papas.c -o papas $(LDFLAGS)

vegano: vegano.c estructura.h
	$(CC) $(CFLAGS) vegano.c -o vegano $(LDFLAGS)

run:
	./main

clean:
	rm -f $(EXECS)
