#include <stdio.h>
#include <stdlib.h>
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
    myarg_t *ret = (myarg_t *)malloc(sizeof(myarg_t));
    ret->a = 2 * args->a;
    ret->b = 2 * args->b;
    return (void*) ret;
}

int main(int argc, char const *argv[])
{
    pthread_t t;
    myarg_t args = {1,2};
    myarg_t *ret;
    pthread_create(&t,NULL,mythread,(void *)&args);
    pthread_join(t,(void**) &ret);
    printf("ret: a = %d, b = %d\n",ret->a,ret->b);
    return 0;
}
