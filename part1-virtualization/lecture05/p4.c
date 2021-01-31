#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if ( pid < 0 )
    {
        fprintf(stderr,"fork error\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        close(STDOUT_FILENO);
        open("./tmp",O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);

        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("p4.c");
        myargs[2] = NULL;
        if ( execvp(myargs[0],myargs) < 0 )
        {
            fprintf(stderr,"execvp error\n");
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
