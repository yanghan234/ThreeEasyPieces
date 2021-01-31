#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

void sigchild_handler(int signum)
{
    int olderrno = errno;
    char msg[100] = "goodbye\n";
    write(STDOUT_FILENO,msg,strlen(msg));
    errno = olderrno;
}

int main(int argc, char const *argv[])
{
    pid_t pid;
    if ( signal(SIGCHLD,sigchild_handler) == SIG_ERR )
    {
        fprintf(stderr,"Failed to signal\n");
        exit(1);
    }
    if ( ( pid = fork() ) < 0 )
    {
        fprintf(stderr,"Failed to fork\n");
        exit(1);
    }
    if ( pid == 0 )
    {
        printf("Child(%d): hello\n",getpid());
        exit(0);
    }
    pause();
    return 0;
}
