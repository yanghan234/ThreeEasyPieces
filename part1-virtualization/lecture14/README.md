# Interlude: Memory API

## Types of Memory
1. stack上的内存由程序自动申请和释放
2. heap上的内存需要由用户手动申请和释放

## The ```malloc()``` Call
C语言中使用```malloc()```函数向堆中申请内存，函数原型为
```c++
// stdlib.h
void *malloc(size_t size_in_bytes);
```

## The ```free()``` Call
C/C++中没有垃圾回收机制，因此需要手动释放动态内存。
```c++
// stdlib.h
void free(void *ptr);
```

## Common Errors
1. 使用指针前没有分配内存
```c++
int *iptr;
*iptr = 1; // used before being allocated
```
2. 没有分配足够内存，溢出
```c++
int *ptr = (int*) malloc(2*sizeof(int));
ptr[2] = 1; // might run, but it is an error
```
3. 没有初始化内存，garbage values
```c++
int *ptr = (int*) malloc(sizeof(int));
int a = *ptr; // garbage value
```
4. 没有释放内存，内存泄漏
```c++
// 由于ptr本身分配在栈上，每次循环结束，ptr会被销毁，但是ptr所指向的动态内存并没有被释放
while (1)
{
    int *ptr = (int*) malloc(sizeof(int));
    *ptr = 0;
}
```
5. 动态内存被提前释放，悬空指针
```c++
int *ptr = (int*) malloc(sizeof(int));
free(ptr);
*ptr = 0; // dangling pointer
```
6. 重复释放内存
```c++
int *ptr = (int*) malloc(sizeof(int));
free(ptr);
free(ptr); // double free
```
7. 释放非malloc申请的指针
malloc申请的内存在头尾有cookie会在free释放的时候用到，如果一个指针不是用malloc申请的，那么用free释放自然会出错。
```c++
int a = 0;
int *ptr = &a;
free(ptr); // error
```

## System calls
值得指出的是，```malloc```和```free```都是C语言的library functions, 而不是系统调用。真正的系统调用是```brk```和```sbrk```，分别用于调整heap和stack的指针。一般不推荐直接使用。

