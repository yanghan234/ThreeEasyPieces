#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int fd = open("./tmp",O_CREAT|O_TRUNC|O_WRONLY,S_IRWXU);
    if ( fd < 0 )
    {
        fprintf(stderr,"failed to open file\n");
        exit(1);
    }
    pid_t pid = fork();
    if ( pid < 0 )
    {
        fprintf(stderr,"fork error\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        char buf[100];
        sprintf(buf,"I am child (pid:%d)\n",getpid());
        for ( int i = 0; i < 100; ++i )
            write(fd,buf,strlen(buf));
        close(fd);
    }
    else
    {
        char buf[100];
        sprintf(buf,"I am parent (pid:%d)\n",getpid());
        for ( int i = 0; i < 100; ++i )
            write(fd,buf,strlen(buf));
        close(fd);
    }
    return 0;
}
