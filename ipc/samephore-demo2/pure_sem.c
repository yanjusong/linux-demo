#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t g_sem;

void* thread(void* arg)
{
    pthread_t tid;

    tid = pthread_self();

    //wait
    sem_wait(&g_sem);
    printf("thread id(0x%lx): Entered..\n", (unsigned long)tid);

    //critical section
    sleep(4);

    //signal
    printf("thread id(0x%lx): Exiting...\n", (unsigned long)tid);
    sem_post(&g_sem);
}


int main()
{
    sem_init(&g_sem, 0, 1);
    pthread_t t1,t2;
    pthread_create(&t1, NULL, thread, NULL);

    sleep(2);

    pthread_create(&t2, NULL, thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&g_sem);
    return 0;
}
