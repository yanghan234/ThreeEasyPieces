#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

static const int MIN = 1;
static const int MAX = 20;

void processFD(int fd)
{
    printf("fd = %d processed\n",fd);
}

int main(int argc, char const *argv[])
{
    while ( 1 )
    {
        fd_set readfds;
        FD_ZERO(&readfds); // clear up all fd

        for ( int fd = MIN; fd < MAX; ++fd )
            FD_SET(fd,&readfds);

        // do select
        int rc = select(MAX,&readfds,NULL,NULL,NULL);

        for ( int fd = MIN; fd < MAX; ++fd )
            if ( FD_ISSET(fd,&readfds))
                processFD(fd);
    }
    return 0;
}
