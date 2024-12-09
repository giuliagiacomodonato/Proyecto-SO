
#include "estructura.h"

int main(){
    char numCliente[MAX_CLIENTES];
    char numPapas[2];
    char *argsClientes[] = {"./cliente", numCliente, NULL};
    char *argsDespachador[] = {"./despachador", NULL};
    char *argsPapas[] = {"./papas", numPapas, NULL};
    char *argsVegano[] = {"./vegano", NULL};
    char *argsHamburguesa[] = {"./hamburguesa", NULL};
	key_t clave = generateKey();
	int id = msgget(clave, 0666| IPC_CREAT);

	if(id < 0){
		printf("error en la creación de la cola de mensajes\n");
		exit(1);
	}
	
	struct mensaje_pedido pedido;
	pedido.tipo = CANT_CLIENTE;

		// Llenar la fila de mensajes para que puedan llegar clientes
	    for(int i=0; i<MAX_ESPERA; i++){
		msgsnd(id , &pedido, MSG_SIZE,0);
	    }

		// Crear despachador
		int pidD = fork();
		if(pidD == 0){
			execv("./despachador", argsDespachador);
			exit(EXIT_FAILURE);
		}
		
		// Crear 2 empleados de papas
		for(int i =0; i<2;i++){
		int pidP = fork();
		sprintf(numPapas,"%d",i+1);
			if(pidP == 0){
				execv("./papas", argsPapas);
				exit(EXIT_FAILURE);
			}
	    }
		
		// Crear empleado de hamburguesas
		int pidH = fork();
		if(pidH == 0){
			execv("./hamburguesa", argsHamburguesa);
			exit(EXIT_FAILURE);
		}

		// Crear empleado de menu vegano
	    int pidV = fork();
		if(pidV == 0){
			execv("./vegano", argsVegano);
			exit(EXIT_FAILURE);
		}

		// Crear clientes
		for(int i =0; i<MAX_CLIENTES; i++){
			int pidC = fork();
			if(pidC == 0){
			sprintf(numCliente,"%d",i+1);
				execv("./cliente", argsClientes);
				printf("error");
				exit(EXIT_FAILURE);
				}
			sleep(1);
		}

		for(int i=0; i<MAX_CLIENTES;i++){ // esperar a que los clientes terminen
			wait(NULL);	
		}
		
		for(int i = 0; i < 5; i++){ // esperar a que los cimpleados terminen
			wait(NULL);
		}
		msgctl(id,IPC_RMID,NULL);
		return 0;
	
	
}
