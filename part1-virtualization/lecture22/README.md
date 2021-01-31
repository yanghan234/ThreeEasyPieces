# Beyond Physical Memory: Policies
本节介绍replacement policy

## Cache Management
物理内存可以视为对disk的一种缓存。

* Average memory access time (AMAT):
    $AMAT = T_M + P_{miss}*T_D$
平均的内存访问时间为真正的内存访问时间，加上可能的Disk访问时间。$P_{miss}$是miss rate

## Optimal Replacement Policy
如果我们可以知道未来的访问每个page的顺序，那么理论上的最优解是将在最远的未来会用到的page换出。但现实中我们无法预知未来，所以这个Policy没有实用性。

* cold-start miss/compulsory miss: 当程序开始启动的时候，每一级缓存里面都没有内容，所以程序启动时必然有一系列miss. 注意：这里的缓存是广义上的缓存，狭义上我们一般用缓存指代SRAM（各级缓存）对DRAM(memory)的缓存，广义上DRAM（memory)是对disk的缓存，而disk可以视作对云端的缓存。
  
## A Simple Policy: FIFO
First-in first-out (FIFO) 的原理和实现都比较简单。所有页在进入系统的时候被放入一个队列中。

## Another Simple Policy: Random
每次需要换页的时候随机地选择一个换出。Random policy和FIFO一样都比较容易实现，但他们的问题也很相似，就是没有考虑合理性，比如一个刚刚被换入的页面，显然不太应该立即被换出。

## Using History: LRU
Least recently used (LRU) 利用了局域性原理。上一次被使用的时间距离现在最远的页面应该被换出。类似的，还有least freqently used LFU 选择将在过去一段时间内使用频率最低的页面换出。

## 对比
1. 如果进程对于页面的访问毫无局域性，完全随机，那么FIFO, random, LRU的表现相似。
2. 如果进程对于页面的访问展现局域性，那么LRU的表现比较好
3. 如果进程对于页面的访问很不巧地在两个循环之间频繁地换入换出相同的页面，那么random方法会比较好

## Implementation of LRU
LRU的表现虽然比较好，但是LRU的实现比较复杂，很有可能为了确保LRU policy会使得效率变得很低。因此，我们需要更快的Policy

## Approximating LRU: clock algorithm
现代操作系统中在页表项中增加了reference bit，也叫use bit, 用来标志这个页面在最近是否被使用过。

Clock algorithm:
1. 每当一个页面被使用，reference bit被设置为1
2. 把所有页面想象为一个环状连链表，每当需要换出页面时，系统总是从上一次被换出的页面开始寻找，如果需要了reference bit为1的页面，则将其reference bit设为0，继续向后寻找，重复这一过程直至遇到一个referene bit为0的页面，换出这个页面。

### Dirty pages
在换出页面的时候，我们也应该考虑页面中的内容是否被修改过，在页表用dirty bit显式。如果一个页面被修改过，那么换出的时候需要写到硬盘，这个过程代价较高，所以换出的时候应该优先选择没有被修改过的页面。

## Thrashing
如果系统中正在运行的进程所需内存超过物理内存，那么系统会反复换入换出页面，这种情况称为thrashing

系统通常会杀掉某些内存使用很大的进程从而使得其它进程可以正常运行。



   


