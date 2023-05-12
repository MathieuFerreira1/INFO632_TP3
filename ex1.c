#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

void a1();
void a2();
void b1();
void b2();

/* Global variables */
int x = 0;
sem_t *sem_s1, *sem_s2;


/* Thread function */
void *p1(void *arg)
{
    a1();
    /* send signal to the created thread */
    sem_post(sem_s1);
    sem_wait(sem_s2);
    a2();

}
void *p2(void *arg)
{
    b1();
    /* wait for signal from main thread */
    sem_post(sem_s2);
    sem_wait(sem_s1);
    b2();
}
/*
void a(){
    sleep(10);
    printf("X = %d\n", x);
    x=55;
}

void b(){
  printf("X = %d\n", x);
}
 */

void a1() {
    printf("a1()\n");
}

void a2() {
    printf("a2()\n");
}

void b1() {
    printf("b1()\n");
}

void b2() {
    printf("b2()\n");
}

void main () {
    sem_s1 = sem_open("sem_s1", O_CREAT, 0644, 0);
    sem_s2 = sem_open("sem_s2", O_CREAT, 0644, 0);


    pthread_t thread1, thread2;
    /* semaphore sync should be initialized by 0 */
    if (sem_s1 == SEM_FAILED) {
        perror("Could not initialize mylock semaphore");
        exit(2);
    }
    if (sem_s2 == SEM_FAILED) {
        perror("Could not initialize mylock semaphore");
        exit(2);
    }
    if (pthread_create(&thread1, NULL, p1, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, p2, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    /* wait for created thread to terminate */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    /* destroy semaphore sync */
    sem_close(sem_s1);
    sem_close(sem_s2);

    exit(0);
}
