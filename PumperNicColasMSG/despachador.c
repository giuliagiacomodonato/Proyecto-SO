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
        sleep(TIEMPO_ESPERA);
        // Intentar recibir un pedido VIP primero
        msgrcv(id , &pedido, MSG_SIZE, VIP_O_NORMAL, 0);
        char * tipo_cliente;
            if(pedido.vip == 1){
                tipo_cliente = "VIP";
            }
            else{ tipo_cliente = "REGULAR";}
    
            // Enviar el pedido a la cola correspondiente según el tipo de combo
            switch(pedido.tipo_combo){
                case TIPO_HAMBURGUESA:
                    pedido.tipo = PEDIDO_HAMBURGUESA;
                    msgsnd(id, &pedido, MSG_SIZE, 0);
                    printf(ORANGE "Despachador: Pedido %s del cliente %d (Hamburguesa) enviado.\n" RESET, tipo_cliente, pedido.id_cliente);
                    fflush(stdout);
                    break;
                case TIPO_VEGANO:
                    pedido.tipo = PEDIDO_VEGANO;
                    msgsnd(id , &pedido, MSG_SIZE, 0);
                    printf(ORANGE "Despachador: Pedido %s del cliente %d (Vegano) enviado.\n" RESET, tipo_cliente, pedido.id_cliente);
                    fflush(stdout);
                    break;
                case TIPO_PAPAS:
                    pedido.tipo = PEDIDO_PAPAS;
                    msgsnd(id , &pedido, MSG_SIZE, 0);
                    printf(ORANGE "Despachador: Pedido %s del cliente %d (Papas) enviado.\n" RESET, tipo_cliente, pedido.id_cliente);
                    fflush(stdout);
                    break;
            }
            pedido.tipo = CANT_CLIENTE; 
            msgsnd(id , &pedido, MSG_SIZE,0 ); // Devolver lugar en la fila
      
    }
}

