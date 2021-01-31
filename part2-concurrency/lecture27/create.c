#include <stdio.h>
#include <pthread.h>

typedef struct
{
    int a;
    int b;
} myarg_t;

void *mythread(void *arg)
{
    myarg_t *args = (myarg_t *) arg;
    printf("a = %d, b = %d\n",args->a,args->b);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t t;
    myarg_t args = { 1, 2};
    int rc = pthread_create(&t,NULL,mythread,&args);
    if ( rc != 0 )
        printf("Failed to create\n");

    pthread_join(t,NULL);
    return 0;
}
