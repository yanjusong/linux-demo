/*
 * @file: multi-helloworld.cpp
 * @author: yanjusong
 * @date: 20180609
 */

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

int g_max_thread_num = 10;

struct thread_data {
    int id;

    void setId(int id)
    {
        this->id = id;
    }
};

void *printf_msg(void *user_data);

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        g_max_thread_num = atoi(argv[1]);    
    }

    pthread_t *thread_handle = NULL;
    int *thread_ret = NULL;
    thread_data *data = NULL;

    thread_handle = (pthread_t *)malloc(g_max_thread_num * sizeof(pthread_t));
    thread_ret = (int *)malloc(g_max_thread_num * sizeof(int));
    data = (thread_data *)malloc(g_max_thread_num * sizeof(thread_data));

    for (int i = 0; i < g_max_thread_num; ++i) {
        data[i].id = i;
    }

    for (int i = 0; i < g_max_thread_num; ++i) {
        thread_ret[i] = pthread_create(&thread_handle[i], NULL, printf_msg, (void *)&data[i]);
    }

    for (int i = 0; i < g_max_thread_num; ++i) {
        pthread_join(thread_handle[i], NULL);
    }


    for (int i = 0; i < g_max_thread_num; ++i) {
        printf("thrad%d returns: %d\n", i, thread_ret[i]);
    }

    return 0;
}

void *printf_msg(void *user_data)
{
    thread_data *pd = (thread_data *)user_data;

    printf("in printf_msg(void *): thread id is %d\n", pd ? pd->id : -1);

    return NULL;
}
