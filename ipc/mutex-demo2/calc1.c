#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

static void err_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    printf(fmt, ap);
    va_end(ap);
    exit(1);
}

int main(int argc, char *argv[])
{
    int* shared_num = NULL;
    pthread_mutex_t *mutex;
    int fd;
    int i;
    void *pmap;
    int *temp;
    int pid;
    int len;
    int init_num;

    if (argc != 2)
        err_sys("invalid args\n");

    if (sscanf(argv[1], "%d", &init_num) != 1)
        err_sys("invalid argv[1]\n");

    len = sizeof(pthread_mutex_t) + sizeof(int);
    fd = open(MAP_FILE_NAME, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE);
    if (fd == -1)
        err_sys("open error\n");

    if (ftruncate(fd, len) < 0)
        err_sys("ftruncate error\n");

    pmap = mmap(NULL, len, PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0);
    if (pmap == MAP_FAILED)
        err_sys("mmap error\n");

    mutex = (pthread_mutex_t *)pmap;
    shared_num = (int*)((char *)pmap + sizeof(pthread_mutex_t));
    *shared_num = 0;

    pthread_mutexattr_t mutex_shared_attr;

    /* Set pthread_mutex_attr to process shared */
    pthread_mutexattr_init(&mutex_shared_attr);
    pthread_mutexattr_setpshared(&mutex_shared_attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(mutex, &mutex_shared_attr);

    pthread_mutex_lock(mutex);
    *shared_num = init_num;
    sleep(5); // for calc2 running, calc2 will wait for mutex.
    pthread_mutex_unlock(mutex);

    printf("in calc1, the shared num: %d\n", *shared_num);

    return 0;
}
