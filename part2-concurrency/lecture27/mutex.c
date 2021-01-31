#include <stdio.h>
#include <pthread.h>
#include <assert.h>

static volatile int counter = 0;
pthread_mutex_t lock;

void *mythread(void *arg)
{
    for ( int i = 0; i < 100000; ++i )
    {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t t1, t2;
    int rc;
    rc = pthread_mutex_init(&lock,NULL);
    assert( rc == 0 );
    rc = pthread_create(&t1,NULL,mythread,NULL);
    assert( rc == 0 );
    rc = pthread_create(&t2,NULL,mythread,NULL);
    assert( rc == 0 );
    rc = pthread_join(t1,NULL);
    assert(rc==0);
    rc = pthread_join(t2,NULL);
    assert(rc==0);
    rc = pthread_mutex_destroy(&lock);
    assert(rc==0);
    printf("final counter = %d\n",counter);
    return 0;
}
