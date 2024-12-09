#include <stdio.h>
#include <unistd.h>



/*
	Eliminar un directorio.
*/

int main(int argc, char *argv[]){

	int error;

    if(argv[0]!=NULL && argv[1]==NULL)
        error = rmdir(argv[0]);
    else
        printf("Cantidad incorrecta de argumentos\n");


    if(error!=0)
        printf("No se pudo borrar el directorio\n");
    else
        printf("Se ha borrado correctamente el directorio\n");

	return 0;
}
