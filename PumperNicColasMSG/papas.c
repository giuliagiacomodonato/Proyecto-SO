#include "estructura.h"

int main(int argc, char*argv[]) {
    key_t clave = generateKey();
    int id = msgget(clave, 0666);
    
    if (id < 0) {
        printf("error al obtener la cola de mensajes\n");
        exit(1);
    }
    
    int id_emp = atoi(argv[1]);
    struct mensaje_pedido pedido; 

    while (1) {
       msgrcv(id , &pedido, MSG_SIZE, PEDIDO_PAPAS, 0);
        printf(YELLOW "Empleado de papas %d está preparando el pedido del cliente %d.\n" RESET, id_emp, pedido.id_cliente);
        fflush(stdout);
        sleep(TIEMPO_PREPARACION); 
        printf(YELLOW "Empleado de papas %d ha terminado de preparar el pedido del cliente %d.\n" RESET, id_emp, pedido.id_cliente);
        fflush(stdout);
        pedido.tipo = TERMINO_PEDIDO_P;
        msgsnd(id , &pedido, MSG_SIZE, 0);
    }

    return 0;
}

