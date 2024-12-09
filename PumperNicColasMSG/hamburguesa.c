#include "estructura.h"

int main() {
    key_t clave = generateKey();
    int id = msgget(clave, 0666);
    
    if (id < 0) {
        printf("error al obtener la cola de mensajes\n");
        exit(1);
    }

    struct mensaje_pedido pedido;

    while (1) {
        msgrcv(id , &pedido, MSG_SIZE, PEDIDO_HAMBURGUESA, 0);
        printf(MAGENTA "Empleado de hamburguesas está preparando el pedido del cliente %d.\n" RESET, pedido.id_cliente);
        fflush(stdout);
        sleep(TIEMPO_PREPARACION); 
        printf(MAGENTA "Empleado de hamburguesas ha terminado de preparar el pedido del cliente %d.\n" RESET, pedido.id_cliente);
        fflush(stdout);
        pedido.tipo = TERMINO_PEDIDO_H;
        msgsnd(id , &pedido, MSG_SIZE, 0);

    }

    return 0;
}

