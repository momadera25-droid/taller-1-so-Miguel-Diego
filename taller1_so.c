#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Recurso compartido
int variable_compartida = 0;

// Mutex para Exclusión Mutua en la variable compartida
pthread_mutex_t mutex_variable;

// Semáforos POSIX para Sincronización de secuencia
sem_t semA_C; // Notifica a C que A escribió Cifra 1
sem_t semC_B; // Notifica a B que C leyó y limpió la variable
sem_t semB_C; // Notifica a C que B escribió Cifra 2

// --- INTEGRANTE 1: Hilo A ---
void* HiloA(void* arg) {
    printf("[Hilo A Dev: Diego] Iniciando... \n");
    int cifra_a = 15;
        // TO-DO Proteger escritura con Mutex e inicializar variable compartida
    pthread_mutex_lock(&mutex_variable);
    variable_compartida = cifra_a;
    pthread_mutex_unlock(&mutex_variable);
    
    // TO-DO Notificar a Hilo C
    sem_post(&semA_C);
    return NULL;
}

// --- INTEGRANTE 2: Hilo B ---
void* HiloB(void* arg) {
    printf("[Hilo B Dev: Diego] Iniciando... \n");
    int cifra_b = 25;
    
    // TO-DO Esperar a que Hilo C haya leído Cifra 1 y limpiado la variable
    sem_wait(&semC_B);

    // TO-DO Proteger escritura con Mutex y actualizar variable compartida
   pthread_mutex_lock(&mutex_variable);
   variable_compartida = cifra_b;
   pthread_mutex_unlock(&mutex_variable);
    
    // TO-DO Notificar a Hilo C
	sem_post(&semB_C);
    return NULL;
}

// --- INTEGRANTE 3: Hilo C ---
void* HiloC(void* arg) {
    printf("[Hilo C Dev: Diego] Iniciando... \n");
    int cifra1 = 0, cifra2 = 0;

    // TO-DO --- PARTE 1: Procesar Cifra 1 ---
    sem_wait(&semA_C);
    pthread_mutex_lock(&mutex_variable);
    cifra1 = variable_compartida;
    variable_compartida = 0;
    pthread_mutex_unlock(&mutex_variable);

    // TO-DO Dar paso a Hilo B
    sem_post(&semC_B);

    // TO-DO --- PARTE 2: Procesar Cifra 2 y Consolidar ---
    sem_wait(&semB_C);
    pthread_mutex_lock(&mutex_variable);
    cifra2 = variable_compartida;
    pthread_mutex_unlock(&mutex_variable);

    // Renderizado final
    printf("\n================ RENDERIZADO DE RESULTADOS ================\n");
    printf("-> Cifra enviada por Hilo A: %d\n", cifra1);
    printf("-> Cifra enviada por Hilo B: %d\n", cifra2);
    printf("-> SUMA TOTAL CALCULADA POR HILO C: %d\n", cifra1 + cifra2);
    printf("===========================================================\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t hA, hB, hC;

    // TO-DO Inicialización de Mutex y Semáforos
    pthread_mutex_init(&mutex_variable, NULL);
    sem_init(&semA_C, 0, 0);
    sem_init(&semC_B, 0, 0);
    sem_init(&semB_C, 0, 0);

    printf("=== INICIO DE LA PRUEBA DE FUEGO ===\n\n");

    // TO-DO Creación de hilos
    pthread_create(&hC, NULL, HiloC, NULL);
    pthread_create(&hB, NULL, HiloB, NULL);
    pthread_create(&hA, NULL, HiloA, NULL);

    // TO-DO Esperar a que terminen
    pthread_join(hA, NULL);
    pthread_join(hB, NULL);
    pthread_join(hC, NULL);

    // TO-DO Destrucción de recursos
    pthread_mutex_destroy(&mutex_variable);
    sem_destroy(&semA_C);
    sem_destroy(&semC_B);
    sem_destroy(&semB_C);

    printf("\n=== FIN DE LA EJECUCIÓN ===\n");
    return 0;
}