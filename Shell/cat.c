#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/stat.h>


/*
	Mostrar el conenido de un archivo.
*/

int main(int argc, char *argv[]){

	//Si hay un solo argumento se imprime el archivo.
    if(argv[0]!=NULL && argv[1]==NULL){

        FILE *archivo;
        char caracter;
        archivo = fopen(argv[0],"r");
        if (archivo == NULL)
            printf("No se puede imprimir el archivo.");
        else{
            caracter = fgetc(archivo);
            while(caracter!= EOF){
                printf("%c",caracter);
                caracter = fgetc(archivo);
            }
        }
        fclose(archivo);
    }
    else
        printf("Cantidad incorrecta de argumentos\n");

    
	return 0;
}
