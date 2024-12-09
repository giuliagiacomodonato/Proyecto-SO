#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/*
    Mostrar una ayuda con los comandos posibles
*/

int main(int argc, char *argv[]){

    

        printf("Ayuda de la Mini-Shell\n");
        printf("Lista de comandos posibles:\n");
        printf("1) mkdir (1 Nombre del directorio a crear)\n");
        printf("2) rmdir (1 Nombre del directorio a borrar)\n");
        printf("3) touch (1 Nombre del archivo a crear)\n");
        printf("4) ls (0 Usa directorio actual para mostrar contenido) o (1 Nombre de otro dicectorio dentro del actual para mostrar su contenido)\n");
        printf("5) cat (1 Nombre del archivo a mostrar)\n");
        printf("6) help (0) \n");
        printf("7) chmod  (2 Nombre del archivo a modificar permisos, permisos en octal)\n");
        printf("8) exit (0)\n");
  
}
