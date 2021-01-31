# Mechanism: Limited Direct Execution

## 两个问题
我们已经知道，系统中的诸多进程通过分时time shareing共享CPU. 由此，有两个需要考虑的问题，
1. 进程的切换必然有额外开销，如何优化performance
2. 如何在CPU运行进程的同时保留操作系统对于资源的掌控，否则的话，同一个进程会永远占据系统资源，而不会切换到其它进程

## Basic Technique: Limited Direct Execution
所谓Direct executaion, 是指系统在创建进程并让其运行后，不再做任何事情，直到这个进程结束。这样的思路显然很有问题，首先这样的进程有可能对系统资源做不安全的操作，其次无法进行进程间的切换。因此，引入Limited Direct Execution的技术，对进程的运行加以限制。

## Problem 1: Restricted Operations
* User mode: 在user mode中运行的代码收到限制，不能进行privileged operations, 比如IO操作。
* Kernel mode: 可以进行privileged operations
* System call: 由于user mode中的操作受限，因此操作系统提供了一些列接口，被称为system calls, 一旦程序调用system calls, 则系统会切换到kernel state来执行相应的操作。
* Trap陷入：当用户态调用了相应的接口，那么系统会通过陷入trap转换为核心态，待执行完毕后，会通过return-from-trap转回用户态。
* Trap table & Trap handler: 操作系统内核会维护一个trap table, 在user mode中调用了system call而发生trap时，系统通过trap table查找到相应的trap handler来处理用户提出的请求。

## Problem 2: Switching Between Processes
当一个进程运行的时候，如何切换到另一个进程？或者说，当一个进程运行的时候，系统如何重新获得CPU, 从而执行进程切换？

### Cooperative approach: Wait for system calls
这是一种消极的管理办法，操作系统**信任**每个进程，因而不主动打断进程，而是**消极**地等待进程放弃CPU. 操作系统等待进程调用system call或者遇到非法操作，例如除以零。这种时候，发生陷入trap，返回内核态。操作系统会在此时判断是否进行进程切换。显然，如果进程中出现死循环，而不发生系统调用，那么操作系统无法重新控制CPU.

### Non-Cooperative approach: The OS takes controls
操作系统每个一段时间进行一次中断操作interrupt, 即timer interrupt. 因此操作系统需要维护一个计时器timer.

### Saving and restoring context -- 上下文切换
当中断发生后，scheduler会判断继续执行当前进程或切换到另一个进程，如果是后者，则需要进行上下文切换context switch, 通常需要保存原来在运行中进程的通用寄存器，计数器，栈指针等等，再将新进程的相关信息读回来。

## Concurrency Issue
系统在处理中断的时候，可能又有别的中断发生，比较简单的做法是在处理终端的时候禁止别的中断发生。

后面讨论并发的时候还会讨论。

