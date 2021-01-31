#include <stdio.h>
#include <time.h>
#include <pthread.h>


typedef struct
{
    int value;
    pthread_mutex_t mutex;
} counter_t;

void init(counter_t *c)
{
    c->value = 0;
    pthread_mutex_init(&c->mutex,NULL);
}

void increment(counter_t *c)
{
    pthread_mutex_lock(&c->mutex);
    c->value++;
    pthread_mutex_unlock(&c->mutex);
}

void decrement(counter_t *c)
{
    pthread_mutex_lock(&c->mutex);
    c->value--;
    pthread_mutex_unlock(&c->mutex);
}

int get(counter_t *c)
{
    pthread_mutex_lock(&c->mutex);
    int rc = c->value;
    pthread_mutex_unlock(&c->mutex);
    return rc;
}

void *func(void *args)
{
    counter_t *c = (counter_t *)args;
    for ( int i = 0; i < 1e6; ++i )
        increment(c);
    return NULL;
}

void destroy(counter_t *c)
{
    pthread_mutex_destroy(&c->mutex);
}

int main(int argc, char const *argv[])
{

    pthread_t t[4];
    counter_t counter;
    init(&counter);
    clock_t beg = clock();
    for ( int it = 0; it < 4; ++it )
        pthread_create(&t[it],NULL,func,(void*)&counter);
    for ( int it = 0; it < 4; ++it )
        pthread_join(t[it],NULL);
    clock_t end = clock();
    printf("Counter = %d\n",get(&counter));
    destroy(&counter);
    double delta_time = (double)( end - beg ) / CLOCKS_PER_SEC; 
    printf("Time used to increment counter a million times in 4 threads: %lf\n",delta_time);
    return 0;
}
