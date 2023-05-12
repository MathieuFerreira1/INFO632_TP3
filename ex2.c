#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* Global variables */
int x = 0;
sem_t m;

/* Thread function */
void *thread(void *arg)
{
    int i = *((int*) arg);
    /* Wait until the semaphore is available */
    sem_wait(&m);
    /* critical section */
    x = x + 1;
    /* Release the semaphore */
    sem_post(&m);
    printf("x:%d, i:%d\n",x,i);
    pthread_exit(NULL);
}

void main ()
{
    pthread_t tid[10];
    int i, args[10];
    /* Semaphore m should be initialized by 1 */
    if (sem_init(&m, 0, 1) == -1) {
        perror("Could not initialize mylock semaphore");
        exit(2);
    }
    /* Create TEN threads */
    for (i=0; i<10; i++)
    {
        args[i] = i;
        if (pthread_create(&tid[i], NULL, thread, &args[i]) < 0) {
            perror("Error: thread cannot be created");
            exit(1);
        }
    }
    /* Wait for all created thread to terminate */
    for (i=0; i<10; i++) pthread_join(tid[i], NULL);
    printf("Final value of x is %d\n", x);
    /* Destroy the semaphore */
    sem_destroy(&m);
    exit(0);
}