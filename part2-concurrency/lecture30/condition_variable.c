#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int done = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void thr_exit()
{
    pthread_mutex_lock(&mutex);
    done = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void *run_child(void *arg)
{
    printf("Child: running\n");
    thr_exit();
    return NULL;
}

void thr_join()
{
    pthread_mutex_lock(&mutex);
    while ( done == 0 )
        pthread_cond_wait(&cond,&mutex);
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char const *argv[])
{
    pthread_t child;
    printf("Parent: begin\n");
    pthread_create(&child,NULL,run_child,NULL);
    thr_join();
    printf("Parent: end\n");
    return 0;
}
