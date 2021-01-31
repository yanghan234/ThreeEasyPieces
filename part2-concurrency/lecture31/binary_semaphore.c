#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#define N 10000

sem_t s;
int count = 0;

void *task(void *arg)
{
    sem_wait(&s);
    for ( int i = 0; i < N; ++i )
        count++;
    sem_wait(&s);
    return NULL;
}

int main(int argc, char const *argv[])
{
    sem_init(&s,0,1);
    pthread_t t1,t2;
    printf("parent: beg\n");
    pthread_create(&t1,NULL,task,NULL);
    pthread_create(&t2,NULL,task,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("counter = %d\n",count);
    printf("parent: end\n");
    return 0;
}

