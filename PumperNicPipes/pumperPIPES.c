#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#define RESET "\033[0m"
#define WHITE "\033[37m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define ORANGE "\033[38;5;214m"

#define MAX_CLIENTES 30

#define VIP 1
#define NO_VIP 0

#define TIEMPO_ESPERA 3
#define TIEMPO_PREPARACION 3
#define TIEMPO_LLEGADA 1

#define PEDIDO_PAPAS 3
#define PEDID_VEGANO 2
#define PEDIDO_HAMBURGUESA 1

// Estructura para representar un pedido
typedef struct {
    int cliente_id;
    int tipo_pedido; // 1: hamburguesa, 2: vegano, 3: papas fritas
    int es_vip; // 1 VIP, 0 NO VIP
} Pedido;


//-------------------- DESPACHADOR -------------------------------

void despachador(int pipe_normal, int pipe_vip, int pipe_escribir_hamburguesa, int pipe_escribir_vegano, int pipe_escribir_papas, int pipe_tomarPedido) {
    Pedido pedido;
    while(1){
        while (read(pipe_vip, &pedido, sizeof(Pedido)) > 0) {
            printf(ORANGE "Despachador recibió pedido del cliente %d, VIP\n" RESET, pedido.cliente_id);
            fflush(stdout);
            // Enviar el pedido al empleado correspondiente
            switch(pedido.tipo_pedido){
                case PEDIDO_HAMBURGUESA:
                    write(pipe_escribir_hamburguesa, &pedido, sizeof(Pedido));
                    break;
                case PEDID_VEGANO:
                    write(pipe_escribir_vegano, &pedido, sizeof(Pedido));
                    break;
                case PEDIDO_PAPAS:
                    write(pipe_escribir_papas, &pedido, sizeof(Pedido));
                    break;
            }
              write(pipe_tomarPedido,&pedido, sizeof(Pedido));
    
        }

            read(pipe_normal, &pedido, sizeof(Pedido));
            printf(ORANGE "Despachador recibió pedido del cliente %d, REGULAR \n" RESET, pedido.cliente_id);
            fflush(stdout);
 
            
            // Enviar el pedido al empleado correspondiente
          switch(pedido.tipo_pedido){
                case PEDIDO_HAMBURGUESA:
                    write(pipe_escribir_hamburguesa, &pedido, sizeof(Pedido));
                    break;
                case PEDID_VEGANO:
                    write(pipe_escribir_vegano, &pedido, sizeof(Pedido));
                    break;
                case PEDIDO_PAPAS:
                    write(pipe_escribir_papas, &pedido, sizeof(Pedido));
                    break;
            }
            write(pipe_tomarPedido,&pedido, sizeof(Pedido));
    
        sleep(TIEMPO_ESPERA);
    }
}

//-------------------- EMPLEADOS -------------------------------

void empleadoPapas(int pipe_leer,  int listos, char * nombre){
    while(1){
        Pedido pedido;
        while(read(pipe_leer, &pedido, sizeof(Pedido))>0){
            printf(YELLOW "El empleado de papas %s está preparando un pedido \n" RESET, nombre);
            fflush(stdout);
            sleep(TIEMPO_PREPARACION); 
            printf(YELLOW "El empleado de papas %s terminó el pedido \n" RESET, nombre);
            fflush(stdout);
            write(listos, &pedido, sizeof(Pedido));
        }
    }
}

void empleadoHamburguesa(int pipe_leer,  int listos){
    while(1){
        Pedido pedido;
        while(read(pipe_leer, &pedido, sizeof(Pedido))>0){
            printf(MAGENTA "El empleado de hamburguesas está preparando un pedido \n" RESET);
            fflush(stdout);
            sleep(TIEMPO_PREPARACION); 
            printf(MAGENTA "El empleado de hamburguesas terminó el pedido \n" RESET);
            fflush(stdout);
            write(listos, &pedido, sizeof(Pedido));
        }
    }
}

void empleadoVegano(int pipe_leer,  int listos){
    while(1){
    Pedido pedido;
        while(read(pipe_leer, &pedido, sizeof(Pedido))>0){
            printf(CYAN "El empleado de menu vegano está preparando un pedido \n" RESET);
            fflush(stdout);
            sleep(TIEMPO_PREPARACION); 
            printf(CYAN "El empleado de menu vegano terminó el pedido \n" RESET);
            fflush(stdout);
            write(listos, &pedido, sizeof(Pedido)); 
        }
    }
}

//-------------------- CLIENTES -------------------------------

void cliente(int pipe_escribirNormal, int pipe_escribirVIP, int listosH,int listosP,int listosV, int id, int tipo_pedido, int es_vip, int pipe_pedidoTomado) {
    Pedido pedido;
    pedido.cliente_id = id;
    pedido.tipo_pedido = tipo_pedido;
    pedido.es_vip = es_vip;

    // 1 de cada 7 clientes decide irse porque hay mucha gente
    srand48(time(NULL));
    long irse = lrand48() % 7;
    if( irse == 6){
        printf(RED "El cliente %d decidió irse sin pedir nada debido a la multitud \n" RESET, id);
        fflush(stdout);
        return;
    }

    // El cliente reliza el pedido en el pipe correspondiente dependiendo de si es VIP o no
    switch(es_vip){
        case VIP:
            printf(WHITE "El cliente %d llego a la fila VIP \n" RESET, id);
            fflush(stdout);
            write(pipe_escribirVIP, &pedido, sizeof(Pedido));
            break;
        case NO_VIP:
            printf(WHITE "El cliente %d llego a la fila regular \n" RESET, id);
            fflush(stdout);
            write(pipe_escribirNormal, &pedido, sizeof(Pedido));
            break;
    }

    char * tipo_cliente;
    if(es_vip){
        tipo_cliente = "VIP";
    }
    else{ tipo_cliente = "REGULAR";}
    
    switch(tipo_pedido){
        case PEDIDO_HAMBURGUESA: 
            printf(WHITE "Cliente %d %s realizó su pedido de hamburguesa \n" RESET, id, tipo_cliente);
            fflush(stdout);
            break;
        case PEDID_VEGANO:
            printf(WHITE "Cliente %d %s realizó su pedido de menu vegano \n" RESET, id, tipo_cliente);
            fflush(stdout);
            break;
        case PEDIDO_PAPAS:
            printf(WHITE "Cliente %d %s realizó su pedido de papas \n" RESET, id, tipo_cliente);
            fflush(stdout);
            break;
    }
    
    read(pipe_pedidoTomado, &pedido, sizeof(Pedido)); // esperar a ser atendido por el despachador
    
    // esperar a que el pedido este listo
    
    switch(tipo_pedido){
        case PEDIDO_HAMBURGUESA: 
            read(listosH, &pedido, sizeof(Pedido));
            printf(GREEN "El cliente %d se retira con su pedido \n" RESET, id);
            fflush(stdout);
            break;
        case PEDID_VEGANO:
            read(listosV, &pedido, sizeof(Pedido));
            printf(GREEN "El cliente %d se retira con su pedido \n" RESET, id);
            fflush(stdout);
            break;
        case PEDIDO_PAPAS:
            read(listosP, &pedido, sizeof(Pedido));
            printf(GREEN "El cliente %d se retira con su pedido \n" RESET, id);
            fflush(stdout);
            break;
    }
    
    return;
    
}
void pipeNoBloqueante(int pipe_fd) {
    int flags = fcntl(pipe_fd, F_GETFL, 0);
    fcntl(pipe_fd, F_SETFL, flags | O_NONBLOCK);
}


int tipoCliente(int id){
    if (id % 5 == 0) { // 1 de cada 5 clientes es vip
        return 1; // Cliente VIP
    } 
    else {
        return 0; // Cliente no VIP
    }

}
int main() {
    int pipe_normal[2], pipe_vip[2], pipe_hamburguesa[2], pipe_vegano[2], pipe_papas[2], pipe_listosH[2],pipe_listosV[2],pipe_listosP[2], pipe_pedidoTomado[2];

    pipe(pipe_normal);
    pipe(pipe_vip);
    pipe(pipe_hamburguesa);
    pipe(pipe_vegano);
    pipe(pipe_papas);
    pipe(pipe_listosH);
    pipe(pipe_listosV);
    pipe(pipe_listosP);
    pipe(pipe_pedidoTomado);

    pipeNoBloqueante(pipe_vip[0]);

    // Crear proceso despachador
    if (fork() == 0) {
        close(pipe_vip[1]);
        close(pipe_normal[1]);
        close(pipe_hamburguesa[0]);
        close(pipe_vegano[0]);
        close(pipe_papas[0]);
        close(pipe_listosH[0]);
        close(pipe_listosH[1]);
        close(pipe_listosP[0]);
        close(pipe_listosP[1]);
        close(pipe_listosV[0]);
        close(pipe_listosV[1]);
        close(pipe_pedidoTomado[0]);
        despachador(pipe_normal[0], pipe_vip[0], pipe_hamburguesa[1], pipe_vegano[1], pipe_papas[1], pipe_pedidoTomado[1]);
        exit(EXIT_SUCCESS);
    }

    // Crear procesos empleados
    if (fork() == 0) {
        close(pipe_vip[0]);
        close(pipe_normal[0]);
        close(pipe_vip[1]);
        close(pipe_normal[1]);
        close(pipe_listosP[0]);
        close(pipe_listosP[1]);
        close(pipe_listosV[0]);
        close(pipe_listosV[1]);
        close(pipe_listosH[0]);
        close(pipe_hamburguesa[1]);
        close(pipe_vegano[0]);
        close(pipe_papas[0]);
        close(pipe_vegano[1]);
        close(pipe_papas[1]);
        close(pipe_pedidoTomado[0]);
        close(pipe_pedidoTomado[1]);
        empleadoHamburguesa(pipe_hamburguesa[0], pipe_listosH[1]);
        exit(EXIT_SUCCESS);
    }

    if (fork() == 0) {
        close(pipe_vip[0]);
        close(pipe_normal[0]);
        close(pipe_vip[1]);
        close(pipe_normal[1]);
        close(pipe_listosP[0]);
        close(pipe_listosP[1]);
        close(pipe_listosH[0]);
        close(pipe_listosH[1]);
        close(pipe_listosV[0]);
        close(pipe_hamburguesa[0]);
        close(pipe_hamburguesa[1]);
        close(pipe_papas[0]);
        close(pipe_papas[1]);
        close(pipe_vegano[1]);
        close(pipe_pedidoTomado[0]);
        close(pipe_pedidoTomado[1]);
        empleadoVegano(pipe_vegano[0],pipe_listosV[1]);
        exit(EXIT_SUCCESS);
    }

    if (fork() == 0) {
        close(pipe_vip[0]);
        close(pipe_normal[0]);
        close(pipe_vip[1]);
        close(pipe_normal[1]);
        close(pipe_listosP[0]);
        close(pipe_listosH[0]);
        close(pipe_listosH[1]);
        close(pipe_listosV[0]);
        close(pipe_listosV[1]);
        close(pipe_listosP[0]);
        close(pipe_hamburguesa[0]);
        close(pipe_hamburguesa[1]);
        close(pipe_vegano[1]);
        close(pipe_vegano[0]);
        close(pipe_papas[1]);
        close(pipe_pedidoTomado[0]);
        close(pipe_pedidoTomado[1]);
        empleadoPapas(pipe_papas[0], pipe_listosP[1],"1");
        exit(EXIT_SUCCESS);
    }
   if (fork() == 0) {
        close(pipe_vip[0]);
        close(pipe_normal[0]);
        close(pipe_vip[1]);
        close(pipe_normal[1]);
        close(pipe_listosP[0]);
        close(pipe_listosH[0]);
        close(pipe_listosH[1]);
        close(pipe_listosV[0]);
        close(pipe_listosV[1]);
        close(pipe_listosP[0]);
        close(pipe_hamburguesa[0]);
        close(pipe_hamburguesa[1]);
        close(pipe_vegano[1]);
        close(pipe_vegano[0]);
        close(pipe_papas[1]);
        close(pipe_pedidoTomado[0]);
        close(pipe_pedidoTomado[1]);
        empleadoPapas(pipe_papas[0], pipe_listosP[1],"2");
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (fork() == 0) {
            int vip = tipoCliente(i+1);
            close(pipe_vip[0]);
            close(pipe_normal[0]);
            close(pipe_hamburguesa[0]);
            close(pipe_hamburguesa[1]);
            close(pipe_vegano[0]);
            close(pipe_vegano[1]);
            close(pipe_papas[0]);
            close(pipe_papas[1]);
            close(pipe_listosH[1]);
            close(pipe_listosP[1]);
            close(pipe_listosV[1]);
            close(pipe_pedidoTomado[1]);
            cliente(pipe_normal[1], pipe_vip[1], pipe_listosH[0],pipe_listosV[0],pipe_listosP[0],i+1, (i % 3)+1, vip, pipe_pedidoTomado[0]);
            exit(EXIT_SUCCESS);
        }
        sleep(TIEMPO_LLEGADA); // Simular llegada de clientes
    }

    close(pipe_vip[1]);
    close(pipe_normal[1]);
    close(pipe_hamburguesa[1]);
    close(pipe_vegano[1]);
    close(pipe_papas[1]);
    close(pipe_pedidoTomado[1]);
    close(pipe_listosH[1]);
    close(pipe_listosV[1]);
    close(pipe_listosP[1]);
    close(pipe_vip[0]);
    close(pipe_normal[0]);
    close(pipe_hamburguesa[0]);
    close(pipe_vegano[0]);
    close(pipe_papas[0]);
    close(pipe_pedidoTomado[0]);
    close(pipe_listosH[0]);
    close(pipe_listosV[0]);
    close(pipe_listosP[0]);
  

    // Esperar a que todos los clientes terminen
    for (int i = 0; i < MAX_CLIENTES; i++) {
        wait(NULL);
    }

    // esperar a los empleados
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);




    return 0;
}
