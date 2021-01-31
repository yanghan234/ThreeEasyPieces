#include <stdio.h>
#include <pthread.h>

void *mythread(void *arg)
{
    printf("%s\n",(char*)arg);
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
    return 0;
}
