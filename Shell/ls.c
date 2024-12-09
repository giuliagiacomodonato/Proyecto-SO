#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define RESET_COLOR "\e[m" 
#define COLOR_VERDE "\e[32m"  // Archivos ejecutables
#define COLOR_AMARILLO "\e[1;33m" // Directorios
#define COLOR_CYAN "\e[36m" // Enlaces simbólicos
#define COLOR_ROJO "\e[31m" // Archivos comprimidos
#define COLOR_NORMAL "\e[0m" // Normal

void imprimir_color(const char *nombre, const char *directorio) {
    struct stat sb;
    char ruta[1024];

    // Crear la ruta completa
    snprintf(ruta, sizeof(ruta), "%s/%s", directorio, nombre);

    // Obtener información sobre el archivo
    if (stat(ruta, &sb) == -1) {
        perror("stat");
        return;
    }

    // Verificar si es un directorio
    if (S_ISDIR(sb.st_mode)) {
        printf("%s%s%s  \t", COLOR_AMARILLO, nombre, RESET_COLOR);  // Directorios en amarillo
    }
    // Verificar si es un archivo ejecutable
    else if (sb.st_mode & S_IXUSR) {
        printf("%s%s%s  \t", COLOR_VERDE, nombre, RESET_COLOR);  // Archivos ejecutables en verde
    } else {
        printf("%s  \t", nombre);  // Archivos regulares sin color
    }
}

int main(int argc, char *argv[]) {
    DIR *dp = NULL; 
    struct dirent *sd = NULL; 
    const char *directorio;
    // si no hay argumentos el directorio es el actual
    if( argv[0] == NULL || * argv[0] == '\0') {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            directorio = cwd;
        }
    }
    else{
        directorio = argv[0]; // si hay argumentos el directorio es el argumento
    }
    dp = opendir(directorio);

    if (dp == NULL) {
        perror("opendir");
        return 1;
    }

    while ((sd = readdir(dp)) != NULL) {
        // Ignorar los puntos '.' y '..'
        if (strcmp(sd->d_name, ".") != 0 && strcmp(sd->d_name, "..") != 0) {
            imprimir_color(sd->d_name, directorio);
        }
    }
    printf("\n");
    closedir(dp);
    return 0;
}
