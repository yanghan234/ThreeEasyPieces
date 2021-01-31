# Paging: Faster Translation (TLB)
本节介绍Translation lookaside buffer (TLB). TLB是芯片上memory management unit (MMU)的一部分。

## TLB Basic Algorithm
每次进程需要访问一个访问一个内存地址时，CPU会先检查TLB中是否包含了这个地址对应的virtual page number，如果hit, 则可以直接取访问内存中的物理地址，从而省略了一次内存访问。如果TLB中不包含VPN对应的表项，则CPU继续到下一级缓存中寻找。找到后，将表项添加到TLB中，如果TLB已经满了，则需要按照一定算法剔除某个表项。

TLB的背后原理是局域性locality: 若进程访问了一个内存地址，我们有理由认为接下来会有很高的可能这个地址还会被访问(时间局域性 temperal locality)，也有很高的可能性这个地址的临近地址会在接下来被访问(空间局域性spatial locality)。

## TLB miss的处理
在complext instruction set computer (CISC)中，一般TLB miss由硬件来处理；而reduced instruction set computer (RISC)中，TLB miss由OS软件处理。在RISC机器中，硬件向OS抛出异常，OS则调用相应的handler来处理。

## TLB中的内容
TLB entry通常有如下的形式: VPN + PFN + other bits. Other bits通常也包含valid bit, protection bit, dirty bit等等

## TLB Issue: Context Switches
由于不同进程有不同的页表，因此我们需要考虑在进程切换时如何处理TLB中的内容。最简单的方式是清空TLB, 放入新的内容。但是由于系统的进程切换可能会很频繁，这会导致TLB频繁清空从而降低速度。

为了解决进程切换时的问题，一些硬件支持不同的进程共享TLB. 它们为TLB的每项中增加了address space identifier (ASID), ASID可以视为很类似于process identifier (PID)的标志服，用于区分不同的进程。这样进程切换时就不用每次都清空TLB了。

## Replacement Policy
常见的有LRU或random, 后面介绍交换区的时候回介绍内存的replacement policy, 方法是类似的。
