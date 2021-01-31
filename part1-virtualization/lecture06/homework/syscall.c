/*
 * Measure the cost a system call
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define N 1000

int main(int argc, char const *argv[])
{
    struct timeval tp;
    // struct timezone tzp;
    gettimeofday(&tp,NULL);
    time_t start;
    time_t end;
    for ( int i = 0; i < N; ++i )
    {
        gettimeofday(&tp,NULL);
        if ( i == 0 )
            start = tp.tv_usec;
        end = tp.tv_usec;
        printf("start = %ld, end = %ld\n",start,end);
        if ( i >= 1 )
            printf("Average time usage of a system call = %10.4f\n",(end-start)*1.0f/i);
    }
    return 0;
}
