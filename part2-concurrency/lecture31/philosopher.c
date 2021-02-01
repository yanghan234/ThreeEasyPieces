#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N 5
#define MAX_VISITS 3

sem_t forks[N];

int left(int p) { return p; }
int right(int p ) { return (p+1) % N; }

void get_forks(int p)
{
#ifndef DEADLOCK
    if ( p == 0 )
    {
        sem_wait(&forks[left(p)]);
        sleep(1);
        sem_wait(&forks[right(p)]);
    }
    else
    {
#endif
        sem_wait(&forks[right(p)]);
        sleep(1);
        sem_wait(&forks[left(p)]);
#ifndef DEADLOCK
    }
#endif
}

void put_forks(int p)
{
    sem_post(&forks[left(p)]);
    sem_post(&forks[right(p)]);
}

void *task(void *arg)
{
    int me = *( (int *) arg );
    int count = 0;
    while ( count < MAX_VISITS )
    {
        get_forks(me);
        printf("(%d) eating for %d-th time\n",me,++count);
        put_forks(me);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    for ( int i = 0; i < N; ++i )
        sem_init(&forks[i],0,1);
    pthread_t philosophers[N];
    int tid[N];
    for ( int i = 0; i < N; ++i )
        tid[i] = i;
    for ( int i = 0; i < N; ++i )
        pthread_create(&philosophers[i],NULL,task,(void*)&tid[i]);
    for ( int i = 0; i < N; ++i )
        pthread_join(philosophers[i],NULL);
    return 0;
}

