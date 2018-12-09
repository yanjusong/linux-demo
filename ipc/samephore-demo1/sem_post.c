#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define SEM_NAME "/test"

int main()
{
    unsigned int initv = 0;
    sem_t *sem;
    int i = 0;
    int num = 5; /* 挂出5次 */
    int value = 0;

    /* 
     * 创建和打开信号量
     * 指定O_EXCL表示如果信号量文件存在则创建失败
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


    for (i; i < num; ++i) {
        if (sem_post(sem) != -1) {
            printf("sem_post success, ");
        }

        if (sem_getvalue(sem, &value) != -1) {
            printf("the semaphore value = %d\n", value);
        }
    }

    sem_close(sem);

    return 0;
}
