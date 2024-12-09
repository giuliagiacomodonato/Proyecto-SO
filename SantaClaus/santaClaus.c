

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h> 
#define N_RENOS 9
#define N_ELFOS 15

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define ORANGE "\033[38;5;214m"

pthread_mutex_t emutex = PTHREAD_MUTEX_INITIALIZER;  
pthread_mutex_t rmutex = PTHREAD_MUTEX_INITIALIZER;

sem_t santaSem;
sem_t renosAtendidos;
sem_t elfosAtendidos;
sem_t listoReno;
sem_t listoElfo;
sem_t elfos_problemas;
sem_t renos_problemas;
sem_t nuevosElfos; 

void *santaClaus(){
    while (1)    {
        sem_wait(&santaSem);
        printf(RED "Se despertó Santa Claus \n" RESET);
        fflush(stdout);

        pthread_mutex_lock(&rmutex);
        if(sem_trywait(&renos_problemas) == 0){ // atiendo a los renos primero si estan listos
                printf(RED "Santa Claus está preparando el trineo \n" RESET);
                fflush(stdout);
                sleep(1);

                pthread_mutex_lock(&emutex);
                if(sem_trywait(&elfos_problemas) == 0){ // ver si renos y elfos llegaron al mismo tiempo
                    sem_post(&elfos_problemas);
                    sem_wait(&santaSem);
			    }
                pthread_mutex_unlock(&emutex);

                for(int i = 0; i<9; i++){
                    printf( "Reno enganchado al trineo\n" );
                    fflush(stdout);
                    sem_post(&listoReno); // aviso a los renos que ya estan en el trineo
                } 

                for(int i = 0; i < 9; i++){
                    sem_post(&renosAtendidos); 
                }    

        }
        pthread_mutex_unlock(&rmutex); // permitir a los renos
     
        
        pthread_mutex_lock(&emutex);
        if(sem_trywait(&elfos_problemas) == 0){ // atiendo a los elfos si estan en problemas
            printf(RED "Santa Claus está ayudando a los elfos \n" RESET);
            fflush(stdout);
            sleep(1);

            for(int i = 0; i<3; i++){
                printf( "Elfo resolviendo problema\n" );
                fflush(stdout);
                sem_post(&listoElfo); // aviso a los elfos que ya solucione sus problemas
            }

            for(int i = 0; i < 3; i++){
                sem_post(&elfosAtendidos); 
            }
            
            for(int i = 0; i < 3; i++){
                sem_post(&nuevosElfos); 
            }
    
        }          
        pthread_mutex_unlock(&emutex); // permitir a los elfos

        printf(RED "Santa Claus vuelve a dormir \n" RESET);
        fflush(stdout);
        sleep(1);
    }
    return NULL;
}

void *reno(){    
    while (1){
            sleep((rand() + pthread_self()) % 5); 
            pthread_mutex_lock(&rmutex);
            sem_wait(&renosAtendidos);
            printf(ORANGE "Llegó un reno\n" RESET); 
            fflush(stdout);
            
            if(sem_trywait(&renosAtendidos) != 0){ // si soy el noveno reno aviso que los renos llegamos
                sem_post(&renos_problemas);
                sem_post(&santaSem);
            }
            else{
                sem_post(&renosAtendidos);    
            }
            
            pthread_mutex_unlock(&rmutex);
            sem_wait(&listoReno);
        
    }
    return NULL;
}

void *elfo(){
    while (1){
        sleep((rand() + pthread_self()) % 5); 
        sem_wait(&nuevosElfos); // de todos los elfos que tienen problemas solo 3 pueden esperar a santa en la tienda
         
        pthread_mutex_lock(&emutex);
        sem_wait(&elfosAtendidos);
        
        printf(GREEN "Llego un elfo \n" RESET);
        fflush(stdout);
        
        if(sem_trywait(&elfosAtendidos) != 0){ // si soy el tercer elfo aviso que los elfos tenemos probelmas
            sem_post(&elfos_problemas);
            sem_post(&santaSem);           
        }
        else{
            sem_post(&elfosAtendidos);      
        }
              
        pthread_mutex_unlock(&emutex); 
        sem_wait(&listoElfo);
      
    }
    return NULL;
}

int main(){
    
    srand(time(NULL));
    
    sem_init(&santaSem,0,0);
    sem_init(&renosAtendidos,0,9);
    sem_init(&elfosAtendidos,0,3);

    sem_init(&elfos_problemas,0,0);
    sem_init(&renos_problemas,0,0);
    
    sem_init(&listoReno,0,0);
    sem_init(&listoElfo,0,0);
    
    sem_init(&nuevosElfos,0,3);

    pthread_t santa_claus;
    pthread_create(&santa_claus, NULL, santaClaus, NULL);

    pthread_t renosT[N_RENOS];
    for (int r = 0; r < N_RENOS; r++){
        pthread_create(&renosT[r], NULL, reno, NULL);
    }
    pthread_t elfosT[N_ELFOS];
    for (int e = 0; e < N_ELFOS; e++){
        pthread_create(&elfosT[e], NULL, elfo, NULL);
    }
    
    // Esperar que Santa Claus termine
    pthread_join(santa_claus, NULL);

    // Esperar que todos los renos terminen
    for (int r = 0; r < N_RENOS; r++)
        pthread_join(renosT[r], NULL);

    // Esperar que todos los elfos terminen
    for (int e = 0; e < N_ELFOS; e++)
        pthread_join(elfosT[e], NULL);

    // Destruir los semáforos
    sem_destroy(&santaSem);
    sem_destroy(&renosAtendidos);
    sem_destroy(&elfosAtendidos);
    sem_destroy(&elfos_problemas);
    sem_destroy(&renos_problemas);
    sem_destroy(&listoReno);
    sem_destroy(&listoElfo);
    sem_destroy(&nuevosElfos);

    return 0;
}
