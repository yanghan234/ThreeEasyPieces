# Mechanism: Address Translation
Hardware-based address translation简称Address translation, 即硬件将程序中的逻辑地址翻译为物理地址的机制。除了硬件之外，操作系统还需要管理内存。二者结合才能完成内存虚拟化。

## Dynamic Relocation
Dynamic relocation在早期的time-sharing机器中使用，它假定每个进程占据连续的物理内存，因此需要使用base和bound来限定每个进程的范围。对于每个进程来说，它的起始地址0x0实际上是物理地址的base. 为了实现这样的机制，硬件上需要base和bound两个寄存器，称为limit寄存器。这部分用于内存管理的硬件称为memory management unit (MMU).

## Hardware Support
1. OS需要有两种状态，即用户态和内核态，因此硬件应该有位置存储OS所处的状态
2. 硬件上需要MMU来存储每个进程的地址范围，即 base and bound registers
3. 硬件应该能够翻译虚拟地址，并在地址越界时抛出异常
4. 硬件需要为base和bound register的修改提供相应的操作
5. 当地址超过进程的范围时，应该抛出异常

## Operating System Issues
1. 操作系统需要管理尚未被分配的内存，常见的例如free list
2. 操作系统需要在进程切换时正确地设置base和bound寄存器
3. 操作系统需要提供exception handler

