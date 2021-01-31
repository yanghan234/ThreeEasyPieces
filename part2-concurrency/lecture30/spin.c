#include <stdio.h>
#include <pthread.h>

volatile int done = 1;

void *child(void *arg)
{
    printf("I am child.\n");
    done = 0;
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t t;
    printf("Parent: begin\n");
    pthread_create(&t,NULL,child,NULL);
    while (done == 1);
    printf("Parent: end\n");
    return 0;
}
