#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>


#define SIZE_TAMPON 5

sem_t *semC;
sem_t *semP;

volatile char buffer[SIZE_TAMPON];

char caractere = 'a';

void* producer(void* args) {
    int ip = 0;
    while (1) {

        sleep(1);
        // Ajout au tampon
        sem_wait(semP);
        buffer[ip] = caractere;
        sem_post(semC);

        printf("On a produit [%d] :\n", ip);
        printf("\tcaractere produit : %c\n",caractere);


        caractere++;
        ip++;

        // si jamais on est à la fin du tampon
        if (ip == SIZE_TAMPON) {
            ip = 0;
        }
    }
}

void* consumer(void* args) {
    int ic = 0;
    while (1) {
        int mess;

        // Retirer du tampon
        sem_wait(semC);
        mess = buffer[ic];
        sem_post(semP);

        printf("On a recupere[%d] :\n", ic);
        printf("\tcaractere recupere : %c\n", mess);

        ic++;

        if (ic == SIZE_TAMPON) {
            ic = 0;
        }

        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    pthread_t prod_th, cons_th;

    semP = sem_open("semP", O_CREAT, 0644, 0);
    semC = sem_open("semC", O_CREAT, 0644, 0);

    pthread_create(&prod_th, NULL, producer, NULL);
    pthread_create(&cons_th, NULL, consumer, NULL);

    pthread_join(prod_th, NULL);
    pthread_join(cons_th, NULL);

    sem_destroy(semP);
    sem_destroy(semC);

    return 0;
}