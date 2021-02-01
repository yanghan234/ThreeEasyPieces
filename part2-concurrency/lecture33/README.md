# Event-based Concurrency
上面几节介绍了多线程程序的基本写法，这些程序一般都是主动地创建线程并分配任务。现实中，另一类由事件驱动的多线程程序则更加实用。例如，web服务器中往往在收到http request之后在将任务分配给其它线程。

## The Basic Idea: An Even Loop
在考虑多线程之前，我们先考虑如何在单线程程序中对events进行相应。最简单的方法是运行一个死循环，等待事件发生，如果有事件发生则处理，否则继续循环等待。
```c++
while ( 1 )
{
    events = getEvents();
    for ( e in events )
        processEvent(e);
}
```
这个死循环称为event loop, 函数```processEvent```称为event handler.

这里有一个问题，我们的程序如何知道它等待的event是否发生？例如network/disk IO.

## API: select, poll
为了让一个程序接受事件，这里介绍```select```系统调用。这里不介绍```poll```和```epoll```，但他们的performance更好。

select的函数原型如下，
```c++
int select( int nfds,
            fd_set *restrict readfds,
            fd_set *restrict writefds,
            fd_set *restrict errorfds,
            struct timeval *restrict timeout);
```
其中```readfds```, ```writefds```和```errorfds```是我们希望select函数为我们检验是否可以读/写/error的file descriptor的集合。此外，select函数只会检验fd小于nfds的文件描述符。例如，如果```readfds```中包含一个descriptor=17并且nfds=17, 那么```readfds```中的这个17不会被检测到。

当```select```函数运行的时候，程序会阻塞，直到```select```函数返回。当```select```返回的时候，三个fd_set中为当前可用的文件描述符的集合。

若```timeout```设置为NULL, 则```select```永远等待，直到某个文件描述符可用；若为0，则立即返回。

## Using ```select```
下面是```select```的一个用例，
```c++
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
            // check if a file descriptor is set
            if ( FD_ISSET(fd,&readfds))
                processFD(fd);
    }
    return 0;
}
```
我们用select实现了一个极为简陋的server模型。每次循环我们都用select挑选可以使用的file descriptor并调用```processFD()```来处理。

## A Problem: Blocking System Calls
上面这样code的好处是不用考虑多线程程序中很容易出错的许多问题。

但劣势也很明显，如果这个段代码的```processFD```中发出了IO请求，那么它将被系统block直至IO请求完成。在被block的期间，显然没有办法再接受新的请求。

如果多线程，当一个线程被阻塞，其它线程还可以继续运行

## Asynchronous IO 异步IO
当一个线程发出IO请求时，异步IO会立即返回，并提供相应的API以便发出请求的线程可以检测IO是否完成。或者，异步IO会在完成后通过signal通知发出IO请求的线程。

