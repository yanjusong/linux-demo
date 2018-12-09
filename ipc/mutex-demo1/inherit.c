#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#ifndef _POSIX_THREAD_PROCESS_SHARED
#error "This platform does not support process shared mutex"
#endif

/* Define globally accessible variables and a mutex */
int* shared_sum = NULL;
pthread_mutex_t *sum_mutex;

int process_fn()
{
    int i;
    for (i = 0; i < 20; ++i) {
        pthread_mutex_lock(sum_mutex);
        *shared_sum += 1;
        pthread_mutex_unlock(sum_mutex);
    }

    return 0;
}

int main()
{
    int i;
    char *pmap;
    int *temp;
    int pid;
    pmap = (char *)mmap(NULL, sizeof(pthread_mutex_t) + sizeof(int), PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sum_mutex = (pthread_mutex_t *)pmap;
    shared_sum = (int*)(pmap + sizeof(pthread_mutex_t));
    *shared_sum = 0;

    pthread_mutexattr_t mutex_shared_attr;

    /* Set pthread_mutex_attr to process shared */
    pthread_mutexattr_init(&mutex_shared_attr);
    pthread_mutexattr_setpshared(&mutex_shared_attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(sum_mutex, &mutex_shared_attr);

    pid = fork();
    if (pid < 0) {
        printf("fork error.\n");
        return -1;
    } else if (pid == 0) {
        process_fn();
        return 0;
    } else {
        process_fn();
        waitpid(pid, NULL, 0);
    }

    temp = (int*)(pmap + sizeof(pthread_mutex_t));
    printf("in the end, the shared sum: %d\n", *temp);
    return 0;
}
