# Segmentation 分段存储
上一节的dynamic relocation方法只使用一对base和bound寄存器，虽然简便，但是每个进程的stack和heap之间的内存没有被使用到，因此造成内部碎片internal fragmentation. 

## Segmentation: Generalized Base/Bounds
为了解决内部碎片，我们可以把代码区，heap区和stack区分别存储，使用三对base/bound寄存器分别存储他们的范围。这样三个部分在物理内存中分别存储，可以减少内部碎片。

## Which segment?
由于此时有了三个分块，因此我们需要告诉系统我们想要的是哪一个块中的地址。
1. 显式：将virtual address的最高两位用于指示块
2. 隐式：硬件通过检测地址的来源来判断处于哪个块中，比如一个地址来自%rsp（栈顶），那么硬件就知道物理地址应该位于stack块中。

## More about stack
由于栈在物理内存中从高地址向低地址生长，而代码段和heap段从低地址向高地址生长，因此segmentation register中还要存储生长方向的信息。

## Support for Sharing
为了节省内存，有时候允许不同的地址空间中贡献内存块（？？？？）只需要将内存块的权限设置为只读，就可以将其与其他地址空间共享。为了实现权限设置，因此Segmentation寄存器中还应该保存关于protection的信息。

## Fine-grained segmentation
上面描述的只分三块的方法称为coarse-grained segmentation, 现代的操作系统可能分成了更多的块，称为fine-grained segmentation. 为了存储关于分块的信息，于是需要segment table分块表，用于存储类似于上面分三时的信息。

## OS Support
由于分块的出现，操作系统需要做更多的工作，但与前面很类似
1. Save and restore segment table
2. Allocate memory
3. Manage free physical memomry
4. 由于被分配的内存是成块的，这导致许多未分配的内存被打散了，造成可能单一的一部分无法满足新到来的请求，这被称为外部碎片external fragmentation. 操作系统可以通过调整已经分配的内存在物理空间中的位置来处理外部碎片，但是这个操作耗时。
