#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define PHI_NUM 5
#define THINK_TIME 2
#define EAT_TIME 1
#define left (phi_id + PHI_NUM - 1) % PHI_NUM
#define right (phi_id + 1) % PHI_NUM

pthread_mutex_t state[PHI_NUM] = {PTHREAD_MUTEX_INITIALIZER};

void *think_and_eat(void *arg)
{
    int phi_id = *(int *)arg;
    while (1)
    {
        sleep(THINK_TIME);
        pthread_mutex_lock(&state[left]);
        if (pthread_mutex_trylock(&state[right]) == EBUSY)
        {
            pthread_mutex_unlock(&state[left]);
            continue;
        }

        printf("philosopher[%d] is eating.\n", phi_id);
        sleep(EAT_TIME);

        pthread_mutex_unlock(&state[left]);
        pthread_mutex_unlock(&state[right]);
    }
}

int main()
{
    int num;
    pthread_t *phi = (pthread_t *)malloc(sizeof(pthread_t) * PHI_NUM);
    int *id = (int *)malloc(sizeof(int) * PHI_NUM);

    for (num = 0; num < PHI_NUM; num++)
    {
        id[num] = num;
        pthread_create(&phi[num], NULL, think_and_eat, (void *)(&id[num]));
    }

    for (num = 0; num < PHI_NUM; num++)
    {
        pthread_join(phi[num], NULL);
    }

    free(phi);
    free(id);

    return 0;
}