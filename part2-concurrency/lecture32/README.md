# Common Concurrency Problems

## Types of Bugs
Non-Deadlock / Deadlock

## Non-Deadlock Bugs
Two major types of non-deadlock bugs: Atomicity violation and order violation.

### Atomicity violation
多个线程同时进入critical section, 修改了共享资源。举例，
```c++
// thread 1
if ( ptr )
    int a = *ptr; // imagine ptr is a pointer to int

// thread 2
ptr = NULL;
```
这类bug的解决往往比较直接，给critical section上锁即可。

### Order-Violation Bugs
我们以为某一个线程中的某些部分会早于另一些线程的某些部分运行，但实际情况可能未必如此。这类bug可以使用条件锁或信号量。

## Deadlock 死锁
线程1在等待线程2完成事件A才能继续，线程2在等待线程1完成事件B才能继续。二者互相等待，永远无法继续。

### 发生死锁的条件
1. Mutual exclusion 互斥性: 死锁往往发生在多线程抢占共享的资源
2. Hold-and-wait 持有和等待：线程持有手上的资源并且在要求新的资源
3. No preemption 不可剥夺性：线程持有的资源不能被强制剥夺，只能由持有资源的线程主动放弃
4. Circular wait 循环等待：程序中应该存在循环等待资源的链条

### 预防死锁 Prevention
1. 预防circular wait: 给锁加上ordering, 每个线程如果想获得一个锁，则它必须已经获得顺序在这个锁之前的锁
2. 预防hold-and-wait: 要求所有线程要么可以获得所有锁，要么等待。但问题是这样做会使得效率变慢，有的线程会饥饿。
3. 预防no preemption: 利用trylock, 如果不成功则之后再尝试，而不是被Block等待
4. 预防mutual exlcusion: 考虑使用lock-free的代码，当然这可能需要使用hardware-supported primitives

### 避免死锁 Avoidance
按照某个安全的顺序给每个线程分配资源，使得每个每个线程都能分配到资源。例如银行家算法。但是适用范围不大。

### 检测和解除 Detect and Recover
允许死锁偶然发生，等待死锁发生再介入处理。



