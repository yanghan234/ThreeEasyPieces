# Beyond Physical Memory: Mechanisms
本节介绍交换区

## Swap spaces
交换区是disk上预留的一部分区域，用于从物理内存中换入换出物理内存中存储的内容。

## The Present Bit
进程每次访问一个虚拟内存，CPU都会先在TLB中查找，如果没有找到，则按级在页表中查找。如果访问的内存是valid的并且已经存储在物理内存中，那么直接按照物理内存地址和offset去访问相应内存即可。如果valid，但不在物理内存中，则抛出缺页中断，系统需要到交换区找到相应的页框，换入物理内存。为了标志一个页框是否在物理内存中存储，page table entry里面会存储present bit.

## Page Fault
无论TLB是由software-managed还是hardware-managed，当发生缺页中断时都由OS来处理。

如果一个页框不在物理内存中，那么页表项中的physical frame number (PFN) 字段将会被用来存储页框在交换区中的位置。当OS将页框从交换区中换入内存，那么对应的表项中PFN将被更新。

## 内存已满
当物理内存已经用尽时，如果OS还想换入页框，那么就必须换出别的页框。至于换出哪一个，就涉及到了replacement policy. 这部分内容会在下一节中介绍

## Page Fault Control Flow
Page fault在return-from-trap之后会回到原来执行的命令继续执行，而很多其它的exception在return-from-trap之后会执行原来命令的下一条。

## When replacements occur
当页框置换发生时，OS并不会真得只置换一个页框，这只是想象中的场景。

真实的情况是，许多OS都有一个high watermark和一个low watermark, 用来标记可用内存的数量。当可用内存低于low watermark, 系统就已经开始换出物理内存中的内容，直到可用物理内存达到high watermark. 执行这一工作的进程通常叫做swap daemon或者page daemon. 