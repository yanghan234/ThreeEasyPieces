#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX 4
#define NTASKS 10
#define N 2

sem_t unfull, nonempty, mutex;
int buffer[MAX];
int getIndex = 0, putIndex = 0;

void *put(int val)
{
    buffer[putIndex] = val;
    putIndex = ( putIndex + 1 ) % MAX;
}

int get()
{
    int rc = buffer[getIndex];
    getIndex = ( getIndex + 1 ) % MAX;
    return rc;
}

void *producer(void *arg)
{
    for ( int i = 0; i < NTASKS; ++i )
    {
        sem_wait(&unfull);
        sem_wait(&mutex);
        put(i);
        printf("(%ld) put: %d\n",(long)pthread_self(),i);
        sem_post(&mutex);
        sem_post(&nonempty);
    }
    return NULL;
}

void *consumer(void *arg)
{
    for ( int i = 0; i < NTASKS; ++i )
    {
        sem_wait(&nonempty);
        sem_wait(&mutex);
        int rc = get();
        printf("(%ld) get: %d\n",(long)pthread_self(),rc);
        sem_post(&mutex);
        sem_post(&unfull);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    sem_init(&unfull,0,MAX);
    sem_init(&nonempty,0,0);
    sem_init(&mutex,0,1);
    pthread_t producers[N];
    pthread_t consumers[N];
    for ( int i = 0; i < N; ++i )
    {
        pthread_create(&producers[i],NULL,producer,NULL);
        pthread_create(&consumers[i],NULL,consumer,NULL);
    }
    for ( int i = 0; i < N; ++i )
    {
        pthread_join(producers[i],NULL);
        pthread_join(consumers[i],NULL);
    }
    return 0;
}
