#include <stdio.h>
#include <time.h>

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

int main(int argc, char const *argv[])
{
    clock_t beg = clock();
    counter_t counter;
    for ( int i = 0; i < 1e6; ++i )
        increment(&counter);
    clock_t end = clock();
    double delta_time = (double)( end - beg ) / CLOCKS_PER_SEC; 
    printf("Time used to increment counter a million times: %lf\n",delta_time);
    return 0;
}
