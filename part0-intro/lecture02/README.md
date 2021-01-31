# Introduction to Operating Systems

## 概述
### 核心问题
How does the operating system virtualize resources?

### 虚拟化
操作系统将物理资源，如处理机，内存，硬盘，抽象为虚拟的资源的过程就是虚拟化virtualization. 

虚拟化之后，操作系统中可以运行许多程序。由此操作系统要为各个程序分配资源，这是操作系统的作用就是resource manager.

### 系统调用 System calls
为了使用户可以使用虚拟资源，系统会给用户提供一些列接口，这些接口通常称为系统调用system calls.

## CPU的虚拟化
以下面这段代码为例，
```c++
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    if ( argc != 2 )
    {
        fprintf(stderr,"usage: cpu <string>\n");
        exit(1);
    }
    char *str = argv[1];
    while (1)
    {
        sleep(1);
        printf("%s\n",str);
    }
    return 0;
}
```
这段代码的作用是从命令行中读入字符串并反复输出。运行效果如下
```bash
promp> ./cpu "A"
A
A
A
^C
```
这种情况下的输出是确定的。但是如果我们要求同时运行多个程序，则输出的结果是混乱的
```bash
prompt> ./cpu A & ./cpu B & ./cpu C & ./cpu D &
A
B
D
C
A
B
D
C
A
^C
```
每个程序都有一种illusion,似乎自己是独占CPU的。但事实上，只有在操作系统给程序分配CPU的时候，各个程序才能运行。这个过程就是就是CPU的虚拟化。每个程序都自以为独占CPU，事实上他们共享CPU。由于OS给各个程序分配CPU的顺序并没有确定性，因此他们的顺序是混乱的，且任意两次的运行顺序也未必相同。

## 内存的虚拟化
类似于CPU的情况也同样适用于内存。下面这段代码中，程序向Heap上申请一段动态内存，并打印内存地址。
```c++
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    int *ptr = (int*) malloc(sizeof(int));
    assert(ptr != NULL);
    printf("(%d) address pointer to by ptr: %p\n",getpid(),ptr);
    *ptr = 0;
    while ( 1 )
    {
        sleep(1);
        ++(*ptr);
        printf("(%d) address pointer to by ptr: %p\n",getpid(),ptr);
    }
    return 0;
}
```
如果我们同时运行多个程序，则会发现不同程序输出的内存地址是相同的，
```bash
> ./mem.o & ./mem.o
[1] 39293
(39293) address pointer to by ptr: 0x7ffe6cc05860
(39294) address pointer to by ptr: 0x7f8ff9c05860
(39293) address pointer to by ptr: 0x7ffe6cc05860
(39294) address pointer to by ptr: 0x7f8ff9c05860
```
内存虚拟化的结果是，每个程序都自以为独占内存空间，但是它们所能看到的内存地址并非物理内存的地址。操作系统会将程序中的逻辑内存转换为物理内存。

虚拟内存的一个好处是不同的进程之间不会互相干扰，一个程序无法访问另一个的地址空间。

## 并发 Concurrency
下面这个程序应用进程中的多线程来介绍了并发执行的概念，但是操作系统中进程之间也是并发执行的。
```c++
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0;
// int counter = 0;
int loops;

void *worker(void *tag)
{
    int i;
    for ( i = 0; i < loops; i++ )
    {
        ++counter;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if ( argc != 2 )
    {
        fprintf(stderr,"usage: threads <value>\n");
        exit(1);
    }
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value: %d\n",counter);
    pthread_create(&p1,NULL,worker,NULL);
    pthread_create(&p2,NULL,worker,NULL);
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    printf("Final value: %d\n",counter);
    return 0;
}
```
运行结果如下
```bash
promp> ./thread 1000
Initial value : 0
Final value : 2000

prompt> ./thread 100000
Initial value : 0
Final value : 143012

prompt> ./thread 100000
Initial value : 0
Final value   : 137298
```
可以看到，当次数足够多的时候，程序计算的结果是错误的，原因就在于两个线程并发执行。具体来说，在一个线程中将数据从内存取入寄存器后，可能发生线程切换，另一个线程又将内存中的相同值取入了所在线程的寄存器。结果导致，两个线程中各做了一次加法，但内存中的值只会被加一次。解决这个问题可以加锁，后面会详细介绍。

## 持久化 Persistence
内存中的数据会在断电后消失，因此用户需要将数据存到持久的存储设备，通常为磁盘或固态硬盘。

操作系统为了管理disk而引入了文件系统file system。但与CPU或者内存虚拟化不同的是，文件系统认为不同的程序会共享磁盘的某些空间，因此并不会为每个程序产生一个私有的虚拟化的磁盘。

## 操作系统的设计目标 Design Goals
* Abstraction: 将物理的资源抽象为虚拟的资源，以便更加易于使用。
* Performance: 为了实现虚拟化，操作系统必然要做一些额外的操作，因此在实现虚拟化的同时，操作系统也应该使得额外的开销overhead最小化。
* Protection: 操作系统应该使得不同程序之间互不干扰，且用户程序也不应该干扰操作系统本身。
* Reliability: 操作系统是一个需要上时间持续运行的程序，因此必须要有足够的稳定性。
* Other: energy-efficiency, security, mobility, etc 

## 操作系统的历史
略

