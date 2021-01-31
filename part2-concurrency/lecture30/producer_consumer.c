#include <stdio.h>
#include <pthread.h>

#define MAX 10
#define NTASKS 10

int buffer[MAX];
int getIndex = 0;
int putIndex = 0;
int count = 0;

pthread_mutex_t mutex;
pthread_cond_t unfull, nonempty;

void put(int val)
{
    buffer[putIndex] = val;
    putIndex = ( putIndex + 1 ) % MAX;
    count++;
}

int get()
{
    int rc = buffer[getIndex];
    getIndex = ( getIndex + 1 ) % MAX;
    count--;
    return rc;
}

void *producer(void *arg)
{
    for ( int i = 0; i < NTASKS; ++i )
    {
        pthread_mutex_lock(&mutex);
        while ( count == MAX )
            pthread_cond_wait(&unfull,&mutex);
        put(i);
        printf("(%ld) put : %d\n",(long)pthread_self(),i);
        pthread_cond_signal(&nonempty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg)
{
    for ( int i = 0; i < NTASKS; ++i )
    {
        pthread_mutex_lock(&mutex);
        while ( count == 0 )
            pthread_cond_wait(&nonempty,&mutex);
        int rc = get();
        printf("(%ld) get : %d\n",(long)pthread_self(),rc);
        pthread_cond_signal(&unfull);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&unfull,NULL);
    pthread_cond_init(&nonempty,NULL);
    pthread_t p1,p2;
    pthread_t c1,c2;
    pthread_create(&p1,NULL,producer,NULL);
    pthread_create(&p2,NULL,producer,NULL);
    pthread_create(&c1,NULL,consumer,NULL);
    pthread_create(&c2,NULL,consumer,NULL);
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    pthread_join(c1,NULL);
    pthread_join(c2,NULL);
    return 0;
}
