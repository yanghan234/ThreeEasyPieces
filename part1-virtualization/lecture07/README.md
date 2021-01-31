# Scheduling: Introduction
* Low-level mechanism: Context switch, 即底层的进程切换如何实现，上一节中已经介绍 
* High-level policies: Scheduling, 即系统决定由哪个进程上CPU运行，本节内容

## Workload assumption
由于实际的情况比较复杂，我们在讨论scheduling policies的时候可以先做出一些列假设，构建一个容易处理的理想状态，然后再逐步去除这些假设接近于真实情况。

我们假设，
1. 每个任务运行时间相同
2. 所有任务同时到达
3. 一旦启动，每个任务都运行到结束
4. 每个任务只使用CPU，没有用到其它设备，比如IO
5. 每个任务的运行时间事先已经知道

显然，上面的假设非常理想化。

## Scheduling Metrics
为了评价一个scheduler的好坏，我们需要制定一些衡量标准scheduling metrics. 这里先讨论周转时间Turnaround time, 即任务从到达系统到任务完成所需时间。

显然周转时间是衡量Performance的指标，后面会讨论衡量Fairness的指标。

## First In, First Out (FIFO)
先进先出FIFO, 也称为First Come, First Service(FCFS)先到先服务。即按照任务的到达顺序依次上CPU运行。

* Convoy effect: 很显然，在FIFO中，如果有长任务先于短任务到达系统，那么短任务要等待很久，这种现象称为convoy effect

## Shortest Job First (SJF)
最短任务优先。可以证明，当所有任务同时到达时，SJF是最有方法。

## Shortest Time-to-Completion First (STCF)
每当有新的任务到达系统，系统都要找出举例完成最短的任务上CPU运行，因此这种方法下需要进程切换。

## New Metric: Response Time
上面的讨论主要基于周转时间，而没有考虑响应时间response time, 即任务到达系统到第一次上CPU运行的等待时间。

以上的几种方法都没有考虑相应时间，因此可能导致有些任务要等待很久才会第一次运行。显然，在今天的操作系统中，这样的等待是难以接受的。

## Round Robin (RR)
时间片轮转。RR不再让任务运行到完成，而是每个任务只运行一个时间片后，系统介入并决定继续运行当前任务或切换到其它任务。

RR是一个注重fairness的算法，但是turnaround的表现可能未必很好。

## Incorporating IO
当任务发出IO请求时会被阻塞并等待系统完成相应请求，这个时候让任务继续占有CPU显然是对资源的浪费，因此系统可以在这个时候介入并考虑切换到另一个进程。

## 最后一个假设
到这里，我们只剩下一个假设还未移除，即系统预先知道每个任务所需要的运行时间，这显然是不现实的。下一节讨论multi-level feeback queue多级反馈队列时会移除这个假设，从而得到一个更加实用的scheduling policy.

