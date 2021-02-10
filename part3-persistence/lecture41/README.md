# Locality and The Fast File System
UNIX中最早的文件系统结构非常简单，它只包含super block, inode blocks 和 data blocks三个部分。其中super block中包含文件系统的必要信息，例如有多少个inode, free list的头指针等等。

## The Problem: Poor Performance
上面这样的文件系统的效率很差，它假定对磁盘的访问像内存一样是随机的。但是，磁盘的读取的额外代价是很高的。这个文件系统中的许多文件都是断断续续地存储在磁盘中的（碎片化），而我们知道磁盘中读取连续存取的数据效率更高。

## FFS: Disk Awareness Is The Solution
Fast File System (FFS) 对外提供的读写API不变，但是内部实现与之前的UNIX文件系统不同。

下面介绍FFS的一些特性。

## Organizing Struture: The Cylinder Group
考虑到磁盘的柱状结构，每张盘片上的磁道构成了一个空心圆柱cylinder. FFS将相邻的几个cylinder视为一个整体，称为cylinder group, 这样一个磁盘包含了若干个cylinder group. 

但是，通常磁盘不会告诉外界某个数据具体在哪个圆柱上，因此FFS将位于同一个cylinder group里面的所有block称为一个block group.

FFS在每个block group中划分出superblock, inode bitmap, data bitmap, inode blocks, data blocks五个部分。其中，superblock在所有block group中共享，这样即使一部分block group损坏，其它的数据依然可以正常访问。由于每个block group都含有bitmap, 这样方便寻找连续的一段存储空间，而不用像free list那样必须从头遍历。

## Policies: How to Allocate Files and Directories
* 当创建目录时，FFS会将目录创建在存储较少目录、包含较多可用inode的block group. 这样可以使得目录下的文件尽可能在同一个block group中。
* 当创建文件时，FFS尽可能使文件的data和它的inode存储在同一个block group中；FFS尽可能使文件和它所属的目录存储在同一个block group中。
* 当遇到大文件时，FFS先尽可能满足上面存储文件的policy, 但是把大文件全部存储在同一个block group中，往往会导致很多小文件被存在其他block group中，这样效率低。因此当大文件超过某个阈值时，FFS会将大文件分段存储到不同的block group中。虽然这样做磁盘会有更大的额外开销，但是由于文件较大，额外开销在摊匀amortized之后影响并不大。而且，大文件每段的size越大，额外开销的影响越小。
  
## A Few Other Things About FFS
* 为了减小内部碎片，FFS会缓存多个小于一个block大小的文件，然后将它们写在同一个block中。如果日后文件的大小变大，FFS会将其移动到合适的位置。
* 磁道中扇区的编号顺序不应该相邻，例如，如果两个相邻扇区编号为1和2，那么当1中的内容读取完毕之后，磁头已经越过了2号扇区，那么必须等待2号扇区转回来才能读取，这样效率很低。因此不同的磁盘往往会根据自己的技术指标来进行扇区的编号，比如根据转速间隔地对扇区编号。
* FFS也有可能会缓存同一个磁道中的内容，当收到读请求时，FFS直接从缓存中提取内容，并不真得再到磁盘中读取。
* FFS支持可变长度的文件名称和symbolic link.
* 现代的文件系统中，例如Linux的ext2, ext3, 依然在使用FFS中的很多思想。

