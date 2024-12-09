
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define ORANGE "\033[38;5;214m"


#define TIEMPO_ARMADO 1.5

sem_t sem_nuevaMoto, sem_ruedas, sem_cuadro, sem_motor, sem_pintor, sem_extra;

void* operario_ruedas() {
    while(1) { 
        sem_wait(&sem_extra); // Espera para comenzar el ciclo de armado
        
        for(int i = 0; i<4; i++){ // Producir 2 motos
            sem_wait(&sem_nuevaMoto); // Espera a poder comenzar con una nueva moto

            printf(YELLOW "Operario Ruedas: Armando una rueda para la moto\n" RESET);
            fflush(stdout);
            sleep(TIEMPO_ARMADO); 

            sem_post(&sem_ruedas); 
        }
        
    }
    return NULL;
}

void* operario_cuadro() {
    while(1) {
        sem_wait(&sem_ruedas); // Espera a que haya una rueda
        sem_wait(&sem_ruedas); // Espera a que haya otra rueda

        printf(MAGENTA "Operario Cuadro: Armando el cuadro para la moto \n" RESET);
        fflush(stdout);
        sleep(TIEMPO_ARMADO); 
        sem_post(&sem_cuadro); 
    }
    return NULL;
}

void* operario_motor() {
    while(1) {
        sem_wait(&sem_cuadro); // Espera a que el cuadro esté listo

        printf(CYAN "Operario Motor: Armando el motor para la moto\n" RESET);
        fflush(stdout);
        sleep(TIEMPO_ARMADO);

        sem_post(&sem_motor); 
    }
    return NULL;
}

void* operario_pintor_verde() {
    while(1) {
        sem_wait(&sem_motor); // Espera a que el motor esté listo

        printf(GREEN "Pintor: Pintando la moto de color Verde \n" RESET);
        fflush(stdout);
        sleep(TIEMPO_ARMADO); 
        printf("Moto lista\n");
        fflush(stdout);
        
        sem_post(&sem_pintor); 
        sem_post(&sem_nuevaMoto);
        sem_post(&sem_nuevaMoto);
         
    }
    return NULL;
}

void* operario_pintor_rojo() {
    while(1) {
        sem_wait(&sem_motor); // Espera a que el motor esté listo

        printf(RED "Pintor: Pintando la moto de color Rojo\n" RESET);
        fflush(stdout);
        sleep(TIEMPO_ARMADO); 
        printf("Moto lista\n");
        fflush(stdout);
        
        sem_post(&sem_pintor); 
        sem_post(&sem_nuevaMoto);
        sem_post(&sem_nuevaMoto);
        
    }
    return NULL;
}

void* operario_equipamiento() {
    while(1){
        sem_wait(&sem_pintor); 
        sem_wait(&sem_pintor); // Espera a que la moto este pintada, una de cada dos motos tiene equipamiento extra
        
        printf(ORANGE "Operario Equipamiento: Moto lista con equipamiento extra\n" RESET);
        fflush(stdout);
        sleep(TIEMPO_ARMADO); 
      
        sem_post(&sem_extra);
    }
    return NULL;
}

int main() {
    pthread_t threads[6];

    // Inicialización de semáforos
    sem_init(&sem_ruedas, 0, 0); 
    sem_init(&sem_cuadro, 0, 0); 
    sem_init(&sem_motor, 0, 0);   
    sem_init(&sem_pintor, 0, 0);  
    sem_init(&sem_nuevaMoto, 0, 2);   
    sem_init(&sem_extra, 0, 1);

    // Creación de hilos
    pthread_create(&threads[0], NULL, operario_ruedas, NULL);
    pthread_create(&threads[1], NULL, operario_cuadro, NULL);
    pthread_create(&threads[2], NULL, operario_motor, NULL);
    pthread_create(&threads[3], NULL, operario_pintor_verde, NULL);
    pthread_create(&threads[4], NULL, operario_pintor_rojo, NULL);
    pthread_create(&threads[5], NULL, operario_equipamiento, NULL);

    // Espera a que terminen todos los hilos
    for (int i = 0; i < 6; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Destrucción de semáforos
    sem_destroy(&sem_ruedas);
    sem_destroy(&sem_cuadro);
    sem_destroy(&sem_motor);
    sem_destroy(&sem_pintor);
    sem_destroy(&sem_nuevaMoto);
    sem_destroy(&sem_extra);
    
    return 0;
}
