/*
 * When the parent forks a child,
 * the child contains a copy of variables.
 * But they are independent of each other.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int x = 100;
    pid_t pid = fork();
    if ( pid < 0 )
    {
        fprintf(stderr,"fork failed\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        // child
        x++;
        printf("In child (pid:%d), x = %d\n",getpid(),x);
    }
    else
    {
        // parent
        x--;
        printf("In parent (pid:%d), x = %d\n",getpid(),x);
    }
    return 0;
}
