#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

sem_t s;

void *child(void *arg)
{
    printf("child\n");
    sem_post(&s);
    return NULL;
}

int main()
{
    int rc;
    printf("parent: beg\n");
    rc = sem_init(&s,0,0);
    assert( rc == 0 );
    pthread_t c;
    pthread_create(&c,NULL,child,NULL);
    sem_wait(&s);
    printf("parent: end\n");
    return 0;
}