#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

int g_max_thread_num = 10000;

int g_thread_cnt = 0;

void *__thread_callback(void *user_data);

struct ThreadData {
    int id;
    void setId(int id)
    {
         this->id = id;
    }
};

int main()
{
    pthread_t *thread_handle = NULL;
    int *thread_ret = NULL;
    ThreadData *data = NULL;

    thread_handle = (pthread_t *)malloc(g_max_thread_num * sizeof(pthread_t));
    thread_ret = (int *)malloc(g_max_thread_num * sizeof(int));
    data = new ThreadData[g_max_thread_num];

    for (int i = 0; i < g_max_thread_num; ++i) {
        data[i].setId(i);
        thread_ret[i] = pthread_create(&thread_handle[i], NULL, __thread_callback, (void *)&data[i]);
    }

    for (int i = 0; i < g_max_thread_num; ++i) {
        pthread_join(thread_handle[i], NULL);
    }

    for (int i = 0; i < g_max_thread_num; ++i) {
        printf("thread returns %d\n", thread_ret[i]);
    }

    free(thread_handle);
    free(thread_ret);
    delete [] data;

    printf("g_thread_cnt = %d\n", g_thread_cnt);

    return 0;
}

void *__thread_callback(void *user_data)
{
    ThreadData *pd = (ThreadData *)user_data;

    printf("In __thread_callback(void *), thread id is %d\n", pd ? pd->id : -1);

    ++g_thread_cnt;

    return NULL;
}
