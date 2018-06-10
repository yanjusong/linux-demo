#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <pthread.h>
#include <unistd.h>

int g_max_thread_num = 10;
pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  g_cond = PTHREAD_COND_INITIALIZER;
int g_is_produce_thread_running = 1;

struct ThreadData
{
    std::string name;

    void setName(std::string name)
    {
        this->name = name;
    }
};

void *__thread_callback(void *user_data)
{
    ThreadData *p = (ThreadData *)user_data;

    while (1 == g_is_produce_thread_running) {
        pthread_mutex_lock(&g_mtx);
        pthread_cond_wait(&g_cond, &g_mtx);

        printf("In __thread_callback(void *), thread name is %s\n", p ? p->name.c_str() : "");

        pthread_mutex_unlock(&g_mtx);
    }
}

void *__thread_callback_produce(void *user_data)
{
    static int s_total = 0;

    while (s_total++ < 100) {
        pthread_mutex_lock(&g_mtx);
        pthread_cond_signal(&g_cond);
        pthread_mutex_unlock(&g_mtx);

        sleep(1);
    }

    g_is_produce_thread_running = 0;
    pthread_cond_broadcast(&g_cond);
}


int main()
{
    pthread_t *thread_handle = NULL;
    int *thread_ret = NULL;
    ThreadData *data  =NULL;
    pthread_t produce_thread_handle;
    int produce_ret = 0;

    thread_handle = (pthread_t *)malloc(g_max_thread_num * sizeof(pthread_t));
    thread_ret = (int *)malloc(g_max_thread_num * sizeof(int));
    data = new ThreadData[g_max_thread_num];

    for (int i = 0; i < g_max_thread_num; ++i) {
        char name[128]  = { 0 };
        sprintf(name, "thread %d", i);
        data[i].setName(name);
    }

    for (int i = 0; i < g_max_thread_num; ++i) {
        thread_ret[i] = pthread_create(&thread_handle[i], NULL, __thread_callback, (void *)&data[i]);
    }

    produce_ret= pthread_create(&produce_thread_handle, NULL, __thread_callback_produce, NULL);

    pthread_join(produce_thread_handle, NULL);

    for (int i = 0; i < g_max_thread_num; ++i) {
        pthread_join(thread_handle[i], NULL);
    }

    // clear
    free(thread_handle);
    free(thread_ret);
    delete[] data;

    return 0;
}