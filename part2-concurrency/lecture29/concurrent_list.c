#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

void destroy(list_t *list)
{
    pthread_mutex_destroy(&list->mutex);
}

void *func(void *arg)
{
    list_t *list = (list_t *) arg;
    for ( int i = 0; i < 4; ++i )
        insert(list,i);
    return NULL;
}

int main()
{
    list_t list;
    init(&list);
    pthread_t t1, t2;
    pthread_create(&t1,NULL,func,(void *)&list);
    pthread_create(&t2,NULL,func,(void *)&list);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    destroy(&list);

    node_t *node = list.head;
    while ( node != NULL )
    {
        printf("%d, ",node->value);
        node = node->next;
    }
    return 0;
}