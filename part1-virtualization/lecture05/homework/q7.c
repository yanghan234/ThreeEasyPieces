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
    else if ( pid == 0 )
    {
        close(STDOUT_FILENO);
        // nothing prints
        printf("Child print\n");
    }
    else
    {
        int ret = 0;
        if ( (ret = waitpid(pid,NULL,0)) < 0 )
        {
            fprintf(stderr,"Parent wait error\n");
            exit(1);
        }
        printf("Parent(%d), ret of wait = %d\n",getpid(),ret);
    }
    return 0;
}
