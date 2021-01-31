#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if ( pid < 0 )
    {
        fprintf(stderr,"fork failed\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        printf("hello, I am child (pid:%d)\n",getpid());
    }
    else
    {
        if ( wait(NULL) < 0 )
        {
            fprintf(stderr,"wait error\n");
            exit(1);
        }
        printf("hello, I am (pid:%d) and I am parent of (pid:%d)\n",getpid(),pid);
    }
    return 0;
}