# Paging: Introduction
将进程的地址空间分成相同大小的许多页。

## Overview
每个进程的地址空间被分割成许多大小相等的页Page. 系统为每个进程维护一个页表page table. 每个虚拟地址可以分为两部分，高若干位是virtual page number (VPN)，低若干位是offset. 其中virtual page number会被翻译为physical frame number 物理页框号，offset为页框内的index. 

## 页表的存储
（单级）页表中包含许多页表项page table entry, 当前运行进程的页表放在硬件memory management unit (MMU)中，其余页表放在内存中。

## 页表项的内容
页表的作用是将虚拟内存中的virtual page number（VPN)翻译为physical frame number (PFN), 因此页表项最基本的内容是PFN. 除此之外，每个页表项还包含一些辅助的内容，例如，
* valid bit: 用来表示这个页表项是不是valid的，例如，如果程序中访问了一个非法地址，则页表项中会显式为invalid
* protection bit: 用来表示用户对于当前地址内容的权限
* present bit: 用来表示这个地址对应的页框是否在内存中，因为内存中的内容有可能被交换到了disk中，即缺页中断
* dirty bit: 用来表示这个地址对应的内容在读入内存后是否被修改过
* reference bit (aka access bit): 用来表示这个页框是否被访问过，这个可以用来帮助系统判定这个页表是否应该被交换到disk中；频繁访问的页框更应该放在内存，不频繁访问的页框更应该交换出去，这样可以减少缺页中断

## 分页的速度问题
在分页的情况下，每次内存访问都要经历一下步骤，
1. 取出virtual address中的virtual page number(VPN)
2. 根据VPN从page number中查找相应的physical frame number (PFN)
3. 结合PFN和offset得到实际的physical address
4. 访问physical address
其中步骤2和步骤4都需要进行内存访问，因此在没有其它软硬件优化的情况下，每次都次访问都需要两次内存访问，这会使得程序的运行变慢。


