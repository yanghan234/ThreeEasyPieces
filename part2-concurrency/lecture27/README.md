# Interlude: Thread API
## Thread Creation
在POSIX中，我们可以使用```thread.h```头文件中定义的```pthread_create```函数来创建线程，
```c++
// protetype
int pthread_create( pthread_t *thread,
                    const pthread_attr_t *attr,
                    void *(*start_routine)(void *),
                    void *arg )
```
四个参数分别为
1. ```thread```为即将创建的线程
2. ```attr```是赋予线程的某些特性，比如优先级，一般为NULL
3. ```start_routine```为一个接受```void *```返回```void *```的函数指针，是线程中将要运行的内容
4. ```arg```是传递给```start_routine```的参数

## Thread Completion
我们使用```pthread.h```头文件中定义的```pthread_join```来等待线程完成
```c++
// protetype of pthread_join
int pthread_join(pthread_t thread, void **value_ptr)
```
值得注意的是，```pthread_join```的第二个参数可以用来传递```pthread_create```中的```start_routine```的返回值。

并不是所有的多线程程序都要用```pthread_join```来等待线程完成，例如web服务器往往会有一个线程等待接收request并将request分配给其它线程执行。这种长久运行的线程显然不用等待完成。

## Locks
POSIX中为critical section提供的互斥锁，
```c++
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unloc(pthread_mutex_t *mutex);
```
锁在使用之前需要初始化，通常可以用下面两种方法来初始化，
```c++
// method 1
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// method 2
int rc = pthread_mutex_init(&lock,NULL);
assert( rc == 0 );
```
如果不再使用锁，应当调用```pthread_mutex_destroy```将其销毁。

在调用这些库函数的时候，我们都应该检查这些函数的error code, 以防有一些奇怪的错误。

## Condition Variables
条件变量用于线程之间传递信号。为了使用condition variables, 我们需要有一个关联的锁。
```c++
int pthread_cond_wait(pthread_cond_t *cond,
                      pthread_mutex_t *lock);
int pthread_cond_signal(pthread_cond_t *cond);
```
上面的```pthread_cond_wait```函数将调用它的线程休眠，等待其它线程给它发信号再唤醒。
```c++
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// thread A
pthread_mutex_lock(&lock);
while (read==0)
    pthread_cond_wait(&cond,&lock);
pthread_mutex_unlock(&lock);

// thread B
pthread_mutex_lock(&lock);
ready = 1;
pthread_cond_signal(&cond);
pthread_mutex_unlock(&lock);
```
值得指出的是，```pthread_cond_wait```的第二个参数是一个锁，而```pthread_cond_signal```则只需要一个参数，不需要锁。原因在于当```pthread_cond_wait```将当前线程休眠的同时也会解除锁，否则其它线程无法进入critical section. 
