#include "estructura.h"

int tipoCliente(int id){
    if (id % 5 == 0) { // 1 de cada 5 clientes es vip
        return 1; // Cliente VIP
    } 
    else {
        return 2; // Cliente no VIP
    }
}

int main(int argc, char*argv[]) {

    key_t clave = generateKey();
    int id = msgget(clave, 0666);
    if (id < 0) {
        printf("error al obtener la cola de mensajes\n");
        exit(1);
    }
    int id_cliente = atoi(argv[1]);
    srand(time(NULL));  // Inicializar la semilla para números aleatorios
    srand(getpid());

    int es_vip = tipoCliente(id_cliente);  
    int tipo = rand() % 3;
    int ped_tipo;
    
    char * tipo_cliente;
    if(es_vip == 1){
        tipo_cliente = "VIP";
    }
    else{ tipo_cliente = "REGULAR";}
    
    switch(tipo){
        case 0:
            ped_tipo = TIPO_HAMBURGUESA;
            break;
        case 1:
            ped_tipo = TIPO_VEGANO;
            break;
        case 2:
            ped_tipo = TIPO_PAPAS;
            break;
    }
    
    struct mensaje_pedido pedido;

    // Tomar un lugar en la fila
    if(msgrcv(id , &pedido, MSG_SIZE, CANT_CLIENTE, IPC_NOWAIT) != -1){
        printf("El cliente %d %s llego a la fila \n",id_cliente, tipo_cliente);
        fflush(stdout);
        pedido.tipo = es_vip;           
        pedido.id_cliente = id_cliente;
        pedido.tipo_combo =ped_tipo;
        pedido.vip = es_vip;
        sleep(TIEMPO_PEDIDO); // simular el tiempo que se tarda el cliente en pedir 
        msgsnd(id , &pedido, MSG_SIZE,0 ); // Enviar pedido
        switch(ped_tipo){
            case TIPO_HAMBURGUESA:
                printf("Cliente %d %s ha hecho un pedido de hamburguesa.\n", id_cliente, tipo_cliente);
                fflush(stdout);
                break;
            case TIPO_VEGANO:
                printf("Cliente %d %s ha hecho un pedido de vegano.\n", id_cliente, tipo_cliente);
                fflush(stdout);
                break;
            case TIPO_PAPAS:
                printf("Cliente %d %s ha hecho un pedido de papas.\n", id_cliente, tipo_cliente);
                fflush(stdout);
                break;
        }
       

    }
   else{
            printf("Cliente %d %s ve muchas personas en la fia.\n",  id_cliente, tipo_cliente);
            int des = rand () % 2;
            if(des == 0){
                printf(RED "Cliente %d %s se retira del local debido a la multitud \n" RESET, id_cliente, tipo_cliente);
                return 0;
           }
            else{
                printf(GREEN "Cliente %d %s decide quedarse a pesar de la multitud.\n" RESET , id_cliente, tipo_cliente);
                sleep(TIEMPO_PEDIDO); // simular el tiempo que se tarda el cliente en pedir 
                    pedido.tipo = es_vip;  
                    pedido.id_cliente = id_cliente;
                    pedido.tipo_combo=tipo;
                    msgsnd(id , &pedido, MSG_SIZE,0 ); // Enviar pedido
                }
            
    }

    // Esperar el pedido listo
    switch(ped_tipo){
        case TIPO_HAMBURGUESA:
            msgrcv(id , &pedido, MSG_SIZE, TERMINO_PEDIDO_H, 0);
            break;
        case TIPO_VEGANO:
            msgrcv(id , &pedido, MSG_SIZE, TERMINO_PEDIDO_V, 0);
            break;
        case TIPO_PAPAS:
           msgrcv(id , &pedido, MSG_SIZE, TERMINO_PEDIDO_P, 0);
           break;
    }
    
    printf(GREEN "El cliente %d se retira con su pedido listo.\n" RESET, pedido.id_cliente);
    fflush(stdout);

    return 0;
}

