#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio_ext.h>


#define CYAN "\033[36m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

//Cada funcion es una archivo que se lo paso al excecv para que cargue la imagen

/*Llamadas al sistema implementadas:
    1- mkdir
    2- rmdir
    3- touch
    4- ls
    5- cat
    6- help
    7- chmod
    8- exit
*/

#define max 100

char entrada[max];
char *comando;
char *argumentos[max];

//obtiene los argumentos de las funciones.
void getArgumentos(){

    int i;

    comando=NULL;

	//Se elimina cualquier contenido previo de argumentos[i]
    for(i=0; i<(max);i++)
        argumentos[i]= NULL;

    comando = strtok(entrada," "); // entrada

    char *aux = strtok(NULL," ");

    i=0;
    while(i<max-2 && aux!=NULL){
        argumentos[i]=aux;
        i++;					// argumentos
        aux = strtok(NULL," ");
    }

}



int main (int argc, char *argv[]){

    int noTermine =1;

    printf(CYAN "Bienvenido a la Mini-Shell\n" RESET);
    printf("Escriba 'help' para obtener ayuda o un comando\n");

    while (noTermine){

        __fpurge(stdin);
        printf(GREEN "\nIngrese un comando: " RESET);
        scanf("%[^\n]s",entrada);
        strcat(entrada,"\0");

        getArgumentos();

        if(strcmp(comando,"exit")==0)
        	return 0;

        //Creo un proceso hijo que se encarga del comando solicitado
        int pid;
        pid = fork();
        if(pid == 0){
   			int error;
        	error = execv(comando,argumentos);
        	if (error < 0){ //Si ocurre un error
        		printf("Ocurrio un error en el execv(2), puede que los comandos ingresados no existan\n");
        		return 0;
        	}
        }else
        	if(pid<0)
        		printf("Ocurrio un error en el fork(0)\n");

        wait(NULL);
	}	

       return 0;
}
