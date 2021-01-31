# Scheduling: Proportional Share
Proportional-share scheduler, 也叫fair-share scheduler, 目的是保证每个任务得到相应比例的CPU时间。

这类调用机制的一个例子是lottery scheduling, 每隔一段时间随机挑选一个任务运行，但是要保证挑选到的任务要满足相应的概率。

## Basic Concept
Lottery scheduling使用*tickets*来表示每个进程应该分配到的资源的比例。

例如，进程A拥有tickets 0-74，进程B拥有75-99，则系统随机从中挑选一个ticket来决定A或B运行。

## Ticket Mechanisms
* ticket currency: 不同用户可能给自己的进程不一样的ticket范围，系统会将其统一转换到某个相同的范围，比如0-100
* ticket transfer: 一个进程可以将自己的ticket传递给另一个进程
* ticket inflation: 这个机制允许一个进程提高或降低自己拥有的ticket, 但是这个前提是进程之间必须互相信任，否则有的进程会排挤其它进程。

## Implementation
系统维护一个就绪队列，其中包含每个就绪进程的ticket. 在每一次需要决定下一个进程时，系统产生一个随机数，然后遍历就绪队列直至随机数落到相应进程的范围，取出进程。

## How to assign tickets?
This problem remains unaddressed.

## Why not deterministic?
Lottery scheduling并不是严格要求各个进程按照给定的比例占有CPU, 当然若进程运行时间够长，会逼近于预定的比例。

如果希望scheduling遵照预定的比例，可以使用stride scheduling. 但是这个方法的问题在于不好处理新加入的进程。

## The Linux Completely Fair Scheduler (CFS)
CFS是现在Linux中使用的调用方法

* CFS不使用fixed time slice, 而是依赖于一种技术叫做virtual runtime(vruntime).
* 在每个进程运行过程中，系统累计vruntime, 当进程切换发生时，拥有最小vruntime的进程先运行
* 由于没有固定的时间片，CFS使用sched_latency来作为下一次需要考虑进程切换的时间，进程越多，sched_latency越小
* 但是若schud_latency特别小，进程切换过于频繁，因此CFS使用另一个变量min_granularity作为最小的时间片，如果sched_latency小于min_granularity则会被忽略。
* **nicenes**: CFS允许用户对进程的优先级进行调整，用户可以调整进程的nice值[-20,19]，nice值越大，优先级越低。
* CFS按照进程的vruntime构建一个红黑树，因此查找速度很快。

