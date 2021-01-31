# Interlude: Process API
本节介绍UNIX中具体的操作系统API,```fork```,```exec```和```wait```. 详细内容参看Advanced Programming in UNIX Environment.

## ```fork()```系统调用
系统调用```fork()```为当前进程创建一个子进程。```fork()```的特点是，调用一次，返回两次，即父子进程中各一次。其中父进程中返回值为子进程的PID，子进程的返回值为0。因此```fork()```的返回值，可以用于判断当前进程是哪一个。

PID, process identifier 是鉴别一个进程的标识符。

下面是```fork()```的一个例子，用```fork()```创建一个子进程并输出PID，
```c++
// p1.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n",getpid());
    pid_t pid = fork();
    if ( pid < 0 )
    {
        // fork failed
        fprintf(stderr,"Fork failed\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        // child process
        fprintf(stdout,"I am child (pid:%d)\n",getpid());
    }
    else
    {
        // parent process
        fprintf(stdout,"I am (pid:%d) and I am parent of (pid:%d)\n",getpid(),pid);
    }
    return 0;
}
```
这是其中一次运行结果
```bash
> ./p1.o
hello world (pid:46968)
I am (pid:46968) and I am parent of (pid:46969)
I am child (pid:46969)
```
需要指出的是，
1. ```fock()```创建的的进程并不是完全复制父进程，子进程拥有独立的地址空间，寄存器，计数器。
2. 由于系统各个进程并发地执行，因此上面代码的运行结果是nondeterministic的。

## ```wait()```系统调用
父进程可以调用```wait()```系列函数(包括```wait```, ```waitpid```, etc, 详见APUE)来等待子进程运行结束再执行接下来的代码。

下面是用```wait```来等待子进程的例子，由于父进程中使用了```wait```，父进程中的打印一定是在子进程的打印之后才会执行。
```c++
// p2.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if ( pid < 0 )
    {
        fprintf(stderr,"fork failed\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        printf("hello, I am child (pid:%d)\n",getpid());
    }
    else
    {
        if ( wait(NULL) < 0 )
        {
            fprintf(stderr,"wait error\n");
            exit(1);
        }
        printf("hello, I am (pid:%d) and I am parent of (pid:%d)\n",getpid(),pid);
    }
    return 0;
}
```
下面是一次运行的结果，
```bash
> ./p2.o
hello, I am child (pid:48659)
hello, I am (pid:48658) and I am parent of (pid:48659)
```
这段代码是deterministic的，两次执行的输出顺序会是相同的，只有pid会发生变化。

## ```exec()```系统调用
```exec()```包含了一系列的系统调用，例如```execl```, ```execlp```, ```execv```, etc, 具体的用法参见APUE. 这个系统调用的作用是运行一个与当前正在执行的程序不同的另一个程序，通常用法是先调用```fork```创建一个新进程，然后在新进程中调用```exec```运行目标程序。

下面是一个例子，
```c++
// p3.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    fprintf(stdout,"hello world (pid:%d)\n",getpid());
    pid_t pid = fork();
    if ( pid < 0 )
    {
        fprintf(stderr,"fork failed\n");
        exit(1);
    }
    else if ( pid == 0 )
    {
        fprintf(stdout,"hello, I am child (pid:%d)\n",getpid());
        char *myargs[3];
        myargs[0] = strdup("wc"); 
        myargs[1] = strdup("p3.c");
        myargs[2] = NULL;
        if ( execvp(myargs[0],myargs) < 0 )
        {
            fprintf(stderr,"execvp error\n");
            exit(1);
        }
        fprintf(stdout,"this shouldn't print out\n");
    }
    else
    {
        if ( wait(NULL) < 0 )
        {
            fprintf(stderr,"wait error\n");
        }
        fprintf(stdout,"hello, I am (pid:%d) and I am parent of (pid:%d)\n",getpid(),pid);
    }
    return 0;
}
```
父进程中调用```fork```创建一个子进程，并且在子进程中利用```execvp```调用命令```wc```统计文件```p3.c```中的字数。运行结果如下，
```bash
> ./p3.o
hello world (pid:49056)
hello, I am child (pid:49057)
      40     102     959 p3.c
hello, I am (pid:49056) and I am parent of (pid:49057)
```

值得注意的是，
1. ```exec```会载入目标的可执行文件，并且覆盖子进程中原有的代码信息，Stack, Heap.
2. 子进程中位于```exec```之后的代码不会被执行。

## 为什么要创造这些API?
```fork```和```exec```的分离，是的父进程在创建子进程之后，还可以在子进程中修改必要的信息，从而可以做更多事情，比如修改环境变量。

下面的例子中，创建子进程之后，将子进程的stdout关闭，并且打开一个文件```tmp```.由于此时```STDOUT_FILENO```是最小的可用file descriptor, 因此子进程中打印到```STDOUT_FILENO```的内容实际上会被写入文件```tmp```. 这个过程实现了bash中常见的命令结果重定向。
```c++
// p4.c
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
```
程序运行后查看文件```tmp```中的内容，
```bash
> cat tmp 
      41      85     819 p4.c
```

## Process Control
1. kill函数可以用于进程间传递信号
2. signal用于进程处理收到的信号
3. 键盘的特定组合可以向正在运行的程序传递信号

## Useful tools
1. ```ps```用于查看正在运行的进程
2. ```top```用于查看系统中正在运行的进程和系统资源的使用情况，例如CPU和内存
3. ```kill```命令(```man 1 kill```)用于向进程传递信号，区别标准库中的```kill```函数(```man 2 kill```)
