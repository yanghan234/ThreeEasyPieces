# Paging: Smaller Tables
前面介绍的页表本身也要占据不少内存，这节着重解决这一问题。

## Simple Solution: Bigger Pages
扩大每个页框的大小，这样页框的总数变小，因此页表也会变小。由于每个页框变大，经常会出现分配出来的页框并不会被全部使用，即造成内部碎片。因此这不是一个好方法。

## Hybrid Approach: Paging and Segments 段页表
前面的页表需要存储每个进程的全部地址空间，但是事实上，heap和stack之间往往有许多内存并没有被使用，因此存储这部分页表项没有必要。于是，我们可以在分段存储的基础上，将没段分页。这种情况下，页表中只存储已经没使用的内存。此时，virtual address分成三个部分，分别为segment number, virtual page number 和 offset.

当然，这个方法依然不能彻底解决所有问题。一个突出的问题是，heap区域可能很sparse，即中间出现很多已经被释放的内存，但是由于连续存储，页表中依然还有这部分内存地址对应的表项。这种情况下，段页表依然占据不小的空间。

## Multi-level page tables 多级页表
多级页表将virtual address分成多个virtual page number和一个offset. 通过每一层的virtual page number可以在页表中查找出下一级页表的其实地址，直到最终找到物理页框再用offset访问物理内存。

* 优点：多级页表并不需要在内存中中连续存储，而单级页表必须连续存储，且单级页表本身很大，因此为其分配一段连续的物理内存并非易事。多级页表中如果下一级的页表中没有任何一项被真正使用，那么没有使用的部分不会在总的页表大小中占据空间，因此多级页表的总体积比较小。
* 缺点：很显然，在多级页表下想访问一个地址需要访问多次页表才能得到最终的物理内存地址，因此速度会变慢。当然，在TLB的帮助下，速度可以得到优化。

## Inverted page tables
Inverted page tables, 反其道而行之，它为每个物理地址制定那个进程中的哪个虚拟地址。不太流行。

## Swapping the page tables to disk
页表通常放在kernel-owned physical memory中，但是页表有可能大到放不下。这种情况下，有些系统将kernel memory也虚拟化，然后将页表放入kernel virtual memory. 当页表大到放不下的时候，可以将页表交换到disk中。当然，页表一旦放入交换区，访问速度就会慢很多。


