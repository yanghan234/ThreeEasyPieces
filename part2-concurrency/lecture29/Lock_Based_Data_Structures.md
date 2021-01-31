# Lock-based Concurrent Data Strucutres
本节介绍如何设计线程安全的数据结构

## 计数器 Counters
### Non-concurrent counter
首先，在不考虑线程的情况下我们很容写出下面的counter,
```c++
typedef struct
{
    int value;
} counter_t;

void init(counter_t *c)
{
    c->value = 0;
}

void increment(counter_t *c)
{
    c->value++;
}

void decrement(counter_t *c)
{
    c->value--;
}

int get(counter_t *c)
{
    return c->value;
}
```

### Trivial concurrent counter: lock everywhere
为了线程安全，最直接的方法是为每一个需要访问共享数据的地方上锁。下面的代码中，在counter结构体中放入了一个互斥锁，用于在访问counter的时候上锁。
```c++
typedef struct
{
    int value;
    pthread_mutex_t mutex;
} counter_t;

void increment(counter_t *c)
{
    pthread_mutex_lock(&c->mutex);
    c->value++;
    pthread_mutex_unlock(&c->mutex);
}

void decrement(counter_t *c)
{
    pthread_mutex_lock(&c->mutex);
    c->value--;
    pthread_mutex_unlock(&c->mutex);
}
```
上面这种方法确实可以得到正确的结果，但是速度很慢。

### Approximate scalable concurrent counter
这种锁是在多处理器系统中，保证每个CPU上只有一个线程，每个线程都有一个local counter, 并且所有线程共享一个global counter. 于是每个线程在更新local counter的时候并不需要加锁，一旦local counter超过一个预设的threshold, 就将local counter的值加到global counter上，这个操作需要为global counter加锁。

从上面的叙述中可以看出，之所以这个counter更快，是因为减少了上锁的次数，但代价是在任何时刻global counter里面的值都不是准确的计数，而只是一个近似值。

## Concurrent LinkedList
Concurrent的链表比较trivil, 只需要在操作链表的时候上锁就可以了。也有人试图用一些理论上更加高效的方法，但实操中效果似乎不佳。
```c++
typedef struct __node_t
{
    int value; 
    struct __node_t *next;
} node_t;

typedef struct
{
    node_t *head;
    pthread_mutex_t mutex;
} list_t;

void init(list_t *list)
{
    list->head = NULL;
    pthread_mutex_init(&list->mutex,NULL);
}

void insert(list_t *list, int val)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->value = val;
    pthread_mutex_lock(&list->mutex);
    node->next = list->head;
    list->head = node;
    pthread_mutex_unlock(&list->mutex);
}
```

## Concurrent Queue
与linked list相比，Queue要对head和tail上锁

## Concurrent Hash Table
计算哈希值的过程与concurrent无关，每个哈希值对应一个concurrent链表。