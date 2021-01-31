#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define N 5

sem_t forks[N];

int left(int);
int right(int);
void get_forks(int);
void put_forks(int);
void *task(void *);

int main(int argc, char const *argv[])
{
    for ( int i = 0; i < N; ++i )
        sem_init(&forks[i],0,1);
    pthread_t philosophers[N];
    for ( int i = 0; i < N; ++i )
        pthread_create(&philosophers[i],NULL,task,(void *)&i);
    for ( int i = 0; i < N; ++i )
        pthread_join(philosophers[i],NULL);
    return 0;
}

int left(int p) { return p; }
int right(int p) { return (p+1) % N; }

void get_forks(int p)
{
    sem_wait(&forks[left(p)]);
    sem_wait(&forks[right(p)]);
}

void put_forks(int p)
{
    sem_post(&forks[right(p)]);
    sem_post(&forks[left(p)]);
}

void *task(void *arg)
{
    int *me = (int *)arg;
    printf("Me = %d\n",*me);
    for ( int i = 0; i < 3; ++i )
    {
        get_forks(*me);
        printf("(%d) Eating\n",*me);
        put_forks(*me);
    }
}
