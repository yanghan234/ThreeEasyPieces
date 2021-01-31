#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    int *ptr = (int*) malloc(sizeof(int));
    assert(ptr != NULL);
    printf("(%d) address pointer to by ptr: %p\n",getpid(),ptr);
    *ptr = 0;
    while ( 1 )
    {
        sleep(1);
        ++(*ptr);
        printf("(%d) address pointer to by ptr: %p\n",getpid(),ptr);
    }
    return 0;
}