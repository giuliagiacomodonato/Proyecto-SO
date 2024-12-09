#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


/*
	Modificar los permisos de un archivo. Los permisos son de lectura, escritura y ejecución.
*/


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo> <permisos en octal>\n", argv[0]);
        return 1;
    }

    char *archivo = argv[0];   // Primer argumento: nombre del archivo
    char *mode = argv[1];      // Segundo argumento: permisos en octal

    // Convertir la cadena de permisos a entero en base 8
    int permisos = strtol(mode, NULL, 8);

    // Verificar que los permisos sean válidos
    if (permisos == 0 && mode[0] != '0') {
        printf("Error: permisos no válidos\n");
        return 1;
    }

    // Cambiar los permisos del archivo usando chmod
    int estado = chmod(archivo, permisos);
    if (estado < 0) {
        perror("Error al cambiar los permisos del archivo");
        return 1;
    }

    printf("Permisos cambiados correctamente para el archivo %s\n", archivo);
    return 0;
}
