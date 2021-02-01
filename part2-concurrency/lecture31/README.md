# Semaphores 信号量

## Definition
信号量包含一个整数值。在POSIX中，我们可以使用```sem_wait()```和```sem_post()```来修改信号量。考虑到信号量的初始值会影响程序的运行效果，因此我们必须对信号量进行初始化。
```c++
#include <semaphore.h>
sem_t s;
sem_init(&s,0,1);
```
其中```sem_init```第二个参数是为0便是信号量在本进程中的所有线程共享，如果第二个参数不为0，则可以在不同进程之间共享，我们通常设为0。第三个参数是信号量的初始值。

```sem_wait```和```sem_post```的作用大致如下
```c++
int sem_wait(sem_t *s)
{
    // decrement the value of semphore s by one
    // wait if value of semaphore s is negative
    // 注意调用sem_wait的时候，一定是先减再做判断：
    // 也就是说，如果s的值为0，
    // 减一后为-1，为负，休眠等待
}

int sem_post(sem_t *s)
{
    // increment the value of semaphore s by one
    // if there are one or more threads waiting, wake one
}
```
具体来说，
1. ```sem_wait```有两种可能情况，要么将semaphore的值减一并返回，要么休眠等待
2. ```sem_post```不会造成等待，它总是将信号量加一，如果有线程在等待，则唤醒其中一个，否则返回。
3. 当信号量的值等于负数的时候，信号量的值的绝对值其实是在等待中的线程数量。

总的来说，信号量的使用通常具有下面这样的格式，
```c++
sem_t m;
sem_init(&m,0,X); // X is initial value

sem_wait(&m);
// critical section
sem_post(&m);
```

## Binary Semaphores (Locks)
首先，我们可以将信号量用作一个锁，这样的信号量称为binary semaphore. 作为锁使用的信号量初始值应该设置为1.

下面是一个使用binary semaphore的例子，
```c++
// binary_semaphore.c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#define N 10000

sem_t s;
int count = 0;

void *task(void *arg)
{
    sem_wait(&s);
    for ( int i = 0; i < N; ++i )
        count++;
    sem_wait(&s);
    return NULL;
}

int main(int argc, char const *argv[])
{
    sem_init(&s,0,1);
    pthread_t t1,t2;
    printf("parent: beg\n");
    pthread_create(&t1,NULL,task,NULL);
    pthread_create(&t2,NULL,task,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("counter = %d\n",count);
    printf("parent: end\n");
    return 0;
}
```
为什么这样的信号量称为binary semaphore呢？原因是这种semaphore要么被hold, 要么不被hold, 只有两种可能的状态。

## Semaphores For Ordering
信号量也可以用于多线程程序中的事件排序。例如，一个线程可能等待一个buffer非空才运行。这里信号量的作用是ordering primitive，与条件变量类似。

下面是一个用信号量做event ordering的例子，注意：这段代码不能在macOS上运行，原因是macOS上的unnamed POSIX semaphore有问题。```sem_init```在macOS上的返回值为-1，function not implemented error.
```c++
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

sem_t s;

void *child(void *arg)
{
    printf("child\n");
    sem_post(&s);
    return NULL;
}

int main()
{
    int rc;
    printf("parent: beg\n");
    rc = sem_init(&s,0,0);
    assert( rc == 0 );
    pthread_t c;
    pthread_create(&c,NULL,child,NULL);
    sem_wait(&s);
    printf("parent: end\n");
    return 0;
}
```

## The Producer/Consumer Problem
利用信号量，我们同样可以实现生产者/消费者问题。下面是完整的代码，
```c++
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX 4
#define NTASKS 10
#define N 2

sem_t unfull, nonempty, mutex;
int buffer[MAX];
int getIndex = 0, putIndex = 0;

void *put(int val)
{
    buffer[putIndex] = val;
    putIndex = ( putIndex + 1 ) % MAX;
}

int get()
{
    int rc = buffer[getIndex];
    getIndex = ( getIndex + 1 ) % MAX;
    return rc;
}

void *producer(void *arg)
{
    for ( int i = 0; i < NTASKS; ++i )
    {
        sem_wait(&unfull);
        sem_wait(&mutex);
        put(i);
        printf("(%ld) put: %d\n",(long)pthread_self(),i);
        sem_post(&mutex);
        sem_post(&nonempty);
    }
    return NULL;
}

void *consumer(void *arg)
{
    for ( int i = 0; i < NTASKS; ++i )
    {
        sem_wait(&nonempty);
        sem_wait(&mutex);
        int rc = get();
        printf("(%ld) get: %d\n",(long)pthread_self(),rc);
        sem_post(&mutex);
        sem_post(&unfull);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    sem_init(&unfull,0,MAX);
    sem_init(&nonempty,0,0);
    sem_init(&mutex,0,1);
    pthread_t producers[N];
    pthread_t consumers[N];
    for ( int i = 0; i < N; ++i )
    {
        pthread_create(&producers[i],NULL,producer,NULL);
        pthread_create(&consumers[i],NULL,consumer,NULL);
    }
    for ( int i = 0; i < N; ++i )
    {
        pthread_join(producers[i],NULL);
        pthread_join(consumers[i],NULL);
    }
    return 0;
}
```
一旦理解了信号量的含义，那么大致写出上面的代码是不困难的。但有一个地方非常容易出错，即我们需要一个锁。并且mutex一定要放在```sem_wait```和```sem_post```之间，否则的话会导致死锁。

## Reader-Writer Locks 读写锁
一般的数据结构读的时候不会修改数据内容，写的时候才会改变，因此可以对于读写操作施加不同的锁。这类锁称为读写锁。

```c++
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct
{
    sem_t rlock;
    sem_t wlock;
    int readers;
} rwlock_t;

void rwlock_init(rwlock_t *rw)
{
    sem_init(&rw->rlock,0,1);
    sem_init(&rw->wlock,0,1);
    rw->readers = 0;
}

void rwlock_acquire_rlock(rwlock_t *rw)
{
    sem_wait(&rw->rlock);
    rw->readers++;
    // 虽然读进程不会修改数据结构，
    // 为了防止在读的同时有写进程修改critical section
    // 所以第一个读进程需要同时获得写锁
    if ( readers == 1 )
        sem_wait(&rw->wlock);
    sem_post(&rw->rlock);
}

void rwlock_release_rlock(rwlock_t *rw)
{
    sem_wait(&rw->rlock);
    rw->readers--;
    if ( readers == 1 )
        sem_post(&rw->wlock);
    sem_post(&rw->rlock);
}

void rwlock_acquire_wlock(rwlock_t *rw)
{
    sem_wait(&rw->wlock);
}

void rwlock_release_wlock(rwlock_t *rw)
{
    sem_post(&rw->wlock);
}
```
上面这段代码中，每一次读进程都可以成功进入critical section，但单一时刻只能有一个写进程进入critical section.

但问题也是很明显的，任何写进程都必须等待所有获得了读锁的读进程全部完成才能进入critical section. 因此读进程会饥饿，读写锁的fairness可能会有问题。

另外，由于每次进入读写锁的时候需要频繁地进行lock/unlock操作，所以当用户一顿操作猛如虎之后，读写锁的性能可能还不如互斥锁。

## The Dining Philosophers 哲学家问题
哲学家问题是一个著名的死锁案例。假设一个圆桌上有五个哲学家吃饭，每两个相邻哲学家共用一个叉子。每个哲学家必须拿起左右两个叉子才能开始吃饭，这也就意味着相邻的两个哲学家不可能同时吃饭。

我们需要为每个哲学家写两个函数，```get_forks```和```put_forks```分别为拿起叉子和放下叉子。我们假定每个哲学家和自己左手边的叉子的编号相同，于是一个编号为```p```的哲学家左右手的叉子的编号为，
```c++
int left(int p) { return p; }
int right(int p) { return (p+1) % 5; // 一共五个 }
```
下一步，我们又很自然地想到用信号量来表示某一个叉子是否被拿起。显然，信号量的初始值应该为1, 这样第一个尝试拿起的哲学家可以顺利拿起。
```c++
sem_t forks[5];
for ( int i = 0; i < 5; ++i )
    sem_init(&forks[i],0,1);
```
每个希望用餐的哲学家应该依次拿起左右两个叉子，吃完后放下两个叉子，即
```c++
void get_forks(int p)
{
    sem_wait(&forks[left(p)]);
    sem_wait(&forks[right(p)]);
}
void put_forks(int p)
{
    sem_post(&forks[right(p)]);
    sem_post(&forks[left(p)]);
}
```
写出这段代码是很容易，在特定情况下也能正常运行。考虑下面这种*不太巧*的情况，每个线程都在先运行```sem_wait(&forks[left(p)]);```, 由于左侧的叉子都可以被拿起，因此这句话是没问题的，每个线程都可以继续执行。但是，当任何一个线程开始执行```sem_wait(&forks[right(p)])```的时候，问题出现了，每个线程右边的叉子都已经被拿起过了，所以每个线程都休眠等待，陷入死锁。

如果希望解决死锁，一个简单的方法是使其中一个哲学家拿起叉子的顺序与其他不同。下面这段代码综合了两种情况，
```c++
// philosopher.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N 5
#define MAX_VISITS 3

sem_t forks[N];

int left(int p) { return p; }
int right(int p ) { return (p+1) % N; }

void get_forks(int p)
{
#ifndef DEADLOCK
    if ( p == 0 )
    {
        sem_wait(&forks[left(p)]);
        sleep(1);
        sem_wait(&forks[right(p)]);
    }
    else
    {
#endif
        sem_wait(&forks[right(p)]);
        // 这里的sleep可以用来帮忙观察死锁现象
        sleep(1);
        sem_wait(&forks[left(p)]);
#ifndef DEADLOCK
    }
#endif
}

void put_forks(int p)
{
    sem_post(&forks[left(p)]);
    sem_post(&forks[right(p)]);
}

void *task(void *arg)
{
    int me = *( (int *) arg );
    int count = 0;
    while ( count < MAX_VISITS )
    {
        get_forks(me);
        printf("(%d) eating for %d-th time\n",me,++count);
        put_forks(me);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    for ( int i = 0; i < N; ++i )
        sem_init(&forks[i],0,1);
    pthread_t philosophers[N];
    int tid[N];
    for ( int i = 0; i < N; ++i )
        tid[i] = i;
    for ( int i = 0; i < N; ++i )
        pthread_create(&philosophers[i],NULL,task,(void*)&tid[i]);
    for ( int i = 0; i < N; ++i )
        pthread_join(philosophers[i],NULL);
    return 0;
}
```
这段代码中用到了一个宏```DEADLOCK```。如果如下编译，则可以看到死锁，
```bash
gcc -Wall -pthread -D DEADLOCK -o philosopher.o philosopher.c
```
如果不加```-D DEADLOCK```这个flag，那么不会观察到死锁。

## Implementation of Semaphore
我们可以用条件变量与模拟信号量的实现，其代码如下，
```c++
#include <pthread.h>
typedef struct 
{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sem_t;

sem_init(sem_t *s, int val)
{
    s->value = val;
    pthread_mutex_init(&s->mutex,NULL);
    pthread_cond_init(&s->cond,NULL);
}

sem_wait(sem_t *s)
{
    pthread_mutex_lock(&s->mutex);
    while ( s->value <= 0 )
        pthread_cond_wait(&s->cond,&s->mutex);
    s->value--;
    pthread_mutex_unlock(&s->mutex);
}

sem_post(sem_t *s)
{
    pthread_mutex_lock(&s->mutex);
    s->value++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->mutex);
}
```
当然，semaphore真正的实现并非如此，这里只是模拟。可以想见，实现同样的功能，信号量会比用条件变量方便很多。



