#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H

#include <sys/types.h> 
#include <sys/ipc.h>    
#include <unistd.h>   
#include <stdio.h>     
#include <stdlib.h> 
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

#define RESET "\033[0m"
#define WHITE "\033[37m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define ORANGE "\033[38;5;214m"

#define MAX_CLIENTES 25
#define TIEMPO_ESPERA 2
#define TIEMPO_PEDIDO 1
#define TIEMPO_PREPARACION 3

#define ESPERA_VIP 1
#define ESPERA_NORMAL 2
#define VIP_O_NORMAL -2
#define TERMINO_PEDIDO_H 10
#define TERMINO_PEDIDO_V 11
#define TERMINO_PEDIDO_P 12
#define PEDIDO_HAMBURGUESA 13
#define PEDIDO_VEGANO 14
#define PEDIDO_PAPAS 15
#define TIPO_HAMBURGUESA 22
#define TIPO_VEGANO 23
#define TIPO_PAPAS 24

#define MAX_ESPERA 5
#define CANT_CLIENTE 17
#define MSG_SIZE sizeof(struct mensaje_pedido) - sizeof(long)

int mq_mensajes;

// Estructura para almacenar los pedidos
struct mensaje_pedido {
    long tipo;  // Tipo de mensaje
    int id_cliente;
    int tipo_combo; // Tipo del pedido de los clientes
    int vip; // 1 si es vip 0 sinp
};
// Declarar la función generateKey
key_t generateKey();

// Funcion para generar la clave
key_t generateKey() {
    int projectId = 14;
    char path[200];
    getcwd(path, sizeof(path)); 
    key_t key = ftok(path, projectId);  // Generar clave única
    return key;
}

#endif // ESTRUCTURA_H
