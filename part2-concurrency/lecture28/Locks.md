# Locks
在critical section附近加锁可以使被加锁部分按照原子性执行。

本节的重点在于如何实现一个锁，为此往往需要硬件或系统的支持。

## The Basic Idea
下面是一个为critical section上锁的例子，
```c++
lock_t mutex; // must be globally allocated
// ...
lock(&mutex);
counter++; // critical section
unlock(&mutex);
```
Lock variable有两种状态，要么是available，要么是acquired. 处于前者的时候，任何线程都可以进入critical section. 处于后者的时候表明有且仅有一个线程已经进入了critical section, 其它线程不得再进入。

## Pthread Locks
POSIX库提供的是互斥锁 mutual exclusive lock (aka mutex). 
```c++
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&lock);
counter++;
pthread_mutex_unlock(&lock);
```
由于```lock```和```unlock```都接受一个锁作为参数，因此我们可以对于不同的critical section使用不同的锁，这样可以提高并发度。

## Evaluating A Lock
1. Mutual exclusion: 一个锁是否足以完成它的基本功能，即互斥
2. Fairness: 如果一个锁处于available状态，是否每个线程都可以机会均等地获得这个锁。
3. Performance: 为了实现这个锁，有多大的额外代价
   
## Controlling Interrupts
最早在单处理器的系统中，可以通过控制终端来实现锁。
```c++
void lock()
{
    DisableInterrupts();
}

void unlock()
{
    EnableInterrupts();
}
```
这种方法里面的上锁即禁止中断，那么进程调度完全不会发生，也就不存在其他线程访问critical section的问题。Interrupts的开启与关闭可以通过硬件来支持。

优点：实现简单
缺点：
1. 这种方法给了用户态privileged权限。用户可以通过这种方法独占CPU.
2. 这种方法在今天的多处理器系统中失效
3. 彻底关闭中断会造成副作用，例如进程发出的IO请求也无法完成，因为IO要切换到内核态运行
4. 这种方法非常慢

因此，这种方法实现的互斥锁通常只见于操作系统自身。

## A Failed Attempt: Just Using Loads/Stores
代码如下，
```c++
typedef struct { int flag; } lock_t;

void int(lock_t *lock)
{
    lock->flag = 0;
}

void lock(lock_t *lock)
{
    while ( lock->flag == 1 ); // spin-wait
    lock->flag = 1;
}

void unlock(lock_t *lock)
{
    lock->flag = 0;
}
```
这段代码试图使用一个flag来标志一个锁所处的状态。在尝试上锁的时候，通过```while```循环反复检测锁的状态，发现没有上锁，则上锁，否则就继续等待。

这段代码的问题：当线程1进入lock函数时，由于此时没有上锁，所以```lock->flag = 0```，因此线程1无需等待，直接进入下一条指令即```lock->flag=1```。但是，系统在线程1尚未执行这一条指令时，恰好切换到了线程2，在线程2看来，此时的flag尚未被修改，仍然为0。因此，线程2也会跳出循环。结果导致，lock函数并没有达到最基本的mutual exclusive的要求。

## Building Working Spin Locks with Test-And-Set
test-and-set是硬件支持的指令，其原理如下
```c
int TestAndSet(int *oldptr, int new)
{
    int old = *oldptr; // save old values
    *oldptr = new;     // set new into oldptr
    return old;        // return old value
}
```
在采用了test-and-set之后，我们得到了如下的锁
```c++
typedef struct { int flag; } lock_t;

void int(lock_t *lock)
{
    lock->flag = 0;
}

void lock(lock_t *lock)
{
    while ( TestAndSet(&lock->flag,1) == 1 ); // spin-waiting
}

void unlock(lock_t *lock)
{
    lock->flag = 0;
}
```
需要指出的是，test-and-set是原语。即一次执行，不会中断。否则的话，上面的锁也无效。

另外，由于我们使用while循环等待，spin-lock显然只能在preemptive 的操作系统中才能用，否则就真得死循环了。

## Evaluating Spin Locks
* Correctness: spin locks满足mutual exclusion, 因此是可以正常使用的
* Fairness: spin locks无法保证fairness, 有些线程可能饥饿
* Performance: spin locks的效率并不高，尤其是在单处理器的时候。处理器会给每个线程分配时间片，但是那些处于等待中的线程会在等待中用完时间片，因此效率并不高。

## Compare-And-Swap Lock
Compare-and-swap也是硬件提供的primitive操作，其基本原理如下，
```c++
int CompareAndSwap(int *ptr, int expected, int new)
{
    int old = *ptr;
    if ( old == expected )
        *ptr = new;
    return old;
}

void lock(lock_t *lock)
{
    while ( CompareAndSwap(lock->flag,0,1) == 1 ); //spin
}
```
Compare-and-swap的更方面表现与test-and-set非常相似，但其还有其它的功能，会在后面介绍到。

## Load-Linked and Store-Conditional
在MIPS架构下提供了一组load-linked和store-condictional指令，可以用来实现锁。

这两个指令的原理如下
```c++
int LoadLinked(int *ptr)
{
    return *ptr;
}

int StoreConditional(int *ptr, int value)
{
    if ( no update to *ptr since LoadLinked to this address )
    {
        *ptr = value;
        return 1; // success
    }
    else
        return 0; // failed
}
```
load-linked 操作可以理解为一个将内存中的内容载入寄存器，但是似乎有某种方法可以知道，寄存器中的内容在载入之后是否被修改过。而sore-conditional只有在载入的内容没有被修改过的情况下，在可以设置新的内容。利用这两个指令，我们可以这样实现锁
```c++
void lock(lock_t *loc)
{
    while (1)
    {
        while (LoadLinked(&lock->flag) == 1); // spin
        // 如果当前线程在这里中断并切换到另一个线程，
        // 那么再切换回来的时候，store-condictional操作会失败，然后再继续等待
        if ( StoreConditional(&lock->flag,1) == 1)
            return;
    }
}
```

## Fetch-And-Add
fetch-and-add 也是一个硬件支持的基本操作，它每次会将old value加一，并返回原先的old value, 其原理如下
```c++
int FetchAndAdd(int *ptr)
{
    int old = *ptr;
    *ptr = old + 1;
    return old;
}
```
利用fetch-and-add操作可以创建ticket lock, 其原理如下
```c++
typedef struct
{
    int ticket;
    int turn;
} lock_t;

void lock_int(lock_t *lock)
{
    lock->ticket = 0;
    lock->turn = 0;
}

void lock(lock_t *lock)
{
    int myturn = FetchAndAdd(&lock->ticket);
    while (lock->turn != myturn ); // spin
}

void unlock(lock_t *lock)
{
    lock->turn = lock->turn + 1;
}
```
fetch-and-add操作将现有的ticket分发给线程作为线程的turn, 只有当lock_t中的turn和线程的turn相同时才进入critical section. 而解锁则是将lock_t中的turn加一，这样可以让下一个线程可以运行。

Ticket Lock的好处在于，每一个分发了ticket的线程都会在将来运行，这样可以保证fairness, 而前面几种锁则无法保证。

## Beyond spinning
前面实现的几种都是自旋锁。自旋锁的特点是，如果一个线程试图进入critical section但是失败，那么这个线程会进入死循环一直等待。于是，自旋锁会在等待中用尽CPU分配的时间片，这显然效率不高。因此自旋锁最好用于每个线程对于critical section的占用时间都很短的情况。

为了提高并发效率，我们需要不使用自旋的锁。

## A Simple Approach: Yield
前面的自旋锁让等待中的线程始终处于死循环并用完CPU. 为了提高效率，一个比较简单的办法是让处于等待的线程放弃CPU, 允许其他线程运行。其原理如下，
```c++
void int()
{
    flag = 0;
}

void lock()
{
    while ( TestAndSet(&flag, 1) == 1 )
        yield(); // give up CPU
}

void unlock()
{
    flag = 0;
}
```
使用yield放弃CPU的线程从运行态转成就绪态。

这种lock的好处在于可以让线程放弃时间片从而提高效率。但是问题在于，频繁的上下文切换代价大，且有些线程会长期处于饥饿。

## Using Queues: Sleeping Instead of Spinning
自旋锁的问题在于浪费时间片，自旋锁+yield的问题是无法防止饥饿。

下面的方法维护一个队列，队列中式处于等待中的线程。

## Different OS, Different Support
不同的操作系统实现锁的方式略有不同，例如Linux下mutex通过futex来实现。futex_wait和futex_wake分别将线程放入休眠状态和唤醒线程。

## Two-Phase Locks
有的锁先spin一段时间，希望可以在这段时间内成功进入scritical section, 如果做不到在进入sleep状态。Linux中的Mutex就是一个例子。



