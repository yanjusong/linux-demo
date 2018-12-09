#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

static void err_sys(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    printf(fmt, ap);
    va_end(ap);
    exit(1);
}

int main()
{
    int* shared_num = NULL;
    pthread_mutex_t *mutex;
    int fd;
    struct stat sbuf;
    int i;
    void *pmap;
    int *temp;
    int pid;
    int len;

    len = sizeof(pthread_mutex_t) + sizeof(int);

    fd = open(MAP_FILE_NAME, O_RDWR);
    if (fd == -1)
        err_sys("open error\n");

    if (fstat(fd, &sbuf) < 0)
        err_sys("fstat error\n");

    if (sbuf.st_size < len)
        err_sys("file size is not enough\n");

    pmap = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pmap == MAP_FAILED)
        printf("mmap error\n");

    mutex = (pthread_mutex_t *)pmap;
    shared_num = (int *)((char *)pmap + sizeof(pthread_mutex_t));

    pthread_mutexattr_t mutex_shared_attr;

    // mutex was initialized by calc2
    pthread_mutex_lock(mutex);
    // now shared_num increase one step
    *shared_num += 1;
    pthread_mutex_unlock(mutex);

    temp = (int* )((char *)pmap + sizeof(pthread_mutex_t));
    printf("in calc2, the shared num: %d\n", *shared_num);

    return 0;
}
