#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    FILE *fd = fopen("./tmp","w");
    assert(fd!=0);
    fprintf(fd,"hello world!\n");
    fclose(fd);
    return 0;
}
