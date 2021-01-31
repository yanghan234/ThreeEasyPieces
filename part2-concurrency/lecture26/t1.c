#include <stdio.h>
#include <pthread.h>

static volatile int counter = 0;

void *mythread(void *arg)
{
    printf("thread %s: begin\n",(char*)arg);
    for ( int i = 0; i < 100000; ++i )
        ++counter;
    printf("thread %s: end\n",(char*)arg);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t t1, t2;
    printf("main: begin\n");
    pthread_create(&t1,NULL,mythread,"A");
    pthread_create(&t2,NULL,mythread,"B");
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("main: end\n");
    printf("counter = %d\n",counter);
    return 0;
}
