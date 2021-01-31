#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    if ( pid < 0 )
    {
        fprintf(stderr,"failed to fork\n");
        exit(1);
    }
    else if ( pid == 0)
    {
        char *myargs[3];
        myargs[0] = "/bin/ls";
        myargs[1] = NULL;
        myargs[2] = NULL;
        if ( execvp(myargs[0],myargs) < 0 )
        {
            fprintf(stderr,"execl error\n");
            exit(1);
        }
    }
    else
    {
        if ( wait(NULL) < 0 )
        {
            fprintf(stderr,"wait error\n");
            exit(1);
        }
    }
    return 0;
}
