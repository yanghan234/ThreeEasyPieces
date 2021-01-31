#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    int fd0[2];
    int fd1[2];
    if ( pipe(fd0) < 0 || pipe(fd1) < 0 )
    {
        fprintf(stderr,"pipe error\n");
        exit(1);
    }
    pid_t pid = fork();
    pid_t kids[2];
    kids[0] = 0;
    kids[1] = 0;
    if ( pid < 0 )
    {
        fprintf(stderr,"failed to fork\n");
        exit(1);
    }
    if ( pid == 0 )
    {
        kids[0] = getpid();
    }
    else
    {
        kids[0] = pid;
        if ( ( pid = fork() ) < 0 )
        {
            fprintf(stderr,"failed to fork\n");
            exit(1);
        }
        if ( pid == 0 )
        {
            kids[1] = getpid();
        }
        else
        {
            kids[1] = pid;
        }
    }

    printf("variable pid = %d, my PID = %d, kids[0] = %d, kids[1] = %d\n",pid,getpid(),kids[0],kids[1]);

    pid_t mypid = getpid();
    char buf[512];

    // kids[0] --> parent --> kids[1]
    if ( mypid == kids[0] )
    {
        // child process 0
        close(fd0[0]);
        sprintf(buf,"Msg to be sent");
        printf("mypid = %d, msg to send: %s\n",mypid,buf);
        write(fd0[1],buf,strlen(buf));
    }
    else if ( mypid == kids[1] )
    {
        // child process 1
        close(fd1[1]);
        read(fd1[0],buf,14);
        printf("mypid = %d, msg received: %s\n",mypid,buf);
    }
    else
    {
        // parent
        close(fd0[1]);
        close(fd1[0]);
        int len = read(fd0[0],buf,14);
        write(fd1[1],buf,len);
    }
}
