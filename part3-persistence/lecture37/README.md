# Hard Disk Drivers
本节介绍硬盘。

## The Interface
磁盘分成一些列扇区，每个扇区为512个字节，扇区从0开始编号，可以视为磁盘的地址空间。磁盘对于一个扇区的读写是原子性的，但超过一个扇区大小的数据读写不是原子性的，所以这样的数据读写期间如果断电，那么数据可能丢失。磁盘中读写一系列相邻数据的速度比随机读写磁盘中内容要快。

## Basic Geometry
磁盘、磁道、扇区。磁盘旋转，磁头寻道。

## A Simple Disk Drive
1. Seek time: 寻道时间，磁头找到相应的磁道
2. Rotational Delay: 磁盘旋转到相应的扇区
3. Transfer: 磁头从扇区读出数据的时间

## Disk Scheduling
Disk scheduler 检验每个request并决定下一步处理哪一个需求。与Job scheduler不同的是，disk scheluder知道每个IO请求的大小，因此可以估计处理每个需求所需的时间。

* Shortest-seek-time-first (SSTF): 将寻道时间最短的IO请求作为下一个任务运行。与SSTF类似的，nearest-block-first (NBF) 把距离当前任务最近的地址的任务拿来运行。**缺点**：有些IO请求会饥饿。
* Elevator (a.k.a. SCAN or C-SCAN): 磁头向一个方向上移动到头，再返回，如果再一次扫描中出现了在背后的IO请求，这个请求并不会立刻处理，而是等到磁头扫描到头再返回的时候才会处理。C-SCAN是一个变种，它永远从外向内扫描，当磁头到最内侧，立即返回最外侧。这类算法很像电梯的运行，因此称为Elevator算法。
* Shortest positioning time first (SPTF): 综合考虑寻道时间和旋转时间，将总时间最短的任务运行。

