#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/* 必须是这种格式/somename */
#define SEM_NAME "/test"

int main()
{
    unsigned int initv = 0;
    sem_t *sem;
    int value = 0;

    /* 
     * 创建和打开信号量
     * O_EXCL表示如果信号量文件存在则创建失败
     */
    sem = sem_open(SEM_NAME, O_RDWR | O_CREAT | O_EXCL, 0777, initv);
    if (sem == SEM_FAILED) {
        perror("sem_open create failed");
        sem = sem_open(SEM_NAME, O_RDWR);
        if (sem == SEM_FAILED) {
            perror("sem_open open failed");
            exit(EXIT_FAILURE);
        }
    }
    printf("sem_open %s success\n", SEM_NAME);

    while (1) {
        if (sem_wait(sem) != -1) {
            value = 0;
            if (sem_getvalue(sem, &value) != -1) {
                printf("sem_wait success, the semaphore value = %d\n", value);
            }
        }
    }

    return 0;
}
