#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n",getpid());
    pid_t pid = fork();
    if ( pid < 0 )
    {
        // fork failed
        fprintf(stderr,"Fork failed\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        // child process
        fprintf(stdout,"I am child (pid:%d)\n",getpid());
    }
    else
    {
        // parent process
        fprintf(stdout,"I am (pid:%d) and I am parent of (pid:%d)\n",getpid(),pid);
    }
    return 0;
}
