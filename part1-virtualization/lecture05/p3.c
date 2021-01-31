#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    fprintf(stdout,"hello world (pid:%d)\n",getpid());
    pid_t pid = fork();
    if ( pid < 0 )
    {
        fprintf(stderr,"fork failed\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        fprintf(stdout,"hello, I am child (pid:%d)\n",getpid());
        char *myargs[3];
        myargs[0] = strdup("wc"); 
        myargs[1] = strdup("p3.c");
        myargs[2] = NULL;
        if ( execvp(myargs[0],myargs) < 0 )
        {
            fprintf(stderr,"execvp error\n");
            exit(1);
        }
        fprintf(stdout,"this shouldn't print out\n");
    }
    else
    {
        if ( wait(NULL) < 0 )
        {
            fprintf(stderr,"wait error\n");
        }
        fprintf(stdout,"hello, I am (pid:%d) and I am parent of (pid:%d)\n",getpid(),pid);
    }
    return 0;
}
