#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


#define max 100

/*	
	Crear un archivo.
*/

int main(int argc, char *argv[]){

int error;

	//Si hay un solo argumento se crea un nuevo directorio
    if(argv[0]!=NULL && argv[1]==NULL){

        error = mkdir(argv[0],S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        if(error!=0)
            printf("No se pudo crear el directorio\n");

        else
            printf("El directorio fue creado correctamente\n");
    }
    else
        printf("Cantidad incorrecta de argumentos\n");

    return 0;

}
