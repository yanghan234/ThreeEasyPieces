# Interlude: Files and Directories

## Files And Directories
Virtualization of storage: files and directories.

* File: 文件是一系列字节。每个文件都有一个底层名称low-level number，叫做inode number, 用户通常看不到这个名称
* Directory: 目录与文件相似，也有底层名称。但与文件不同的是，目录中包含一系列成对的数据，每一对数据包含用户可见文件名称和文件的底层名称。当然，目录中还可以包含下一层目录。

## File System Interface
下面介绍UNIX中有关文件的API.

## Creating Files
文件的创建可以通过```open```系统调用来实现，只需要在调用```open```的时候传递```O_CREAT``` flag即可，
```c++
int fd = open("foo",O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
```
通常```open```系统调用可以接受多个flag, 上面这个例子中我们要求，若文件不存在则创建(O_CREAT), 打开的文件只能用于写入(O_WRONLY), 若文件存在则truncate文件也就是删除(O_TRUNC). 

```open```的返回值是一个文件描述符file descriptor, 其它的系统调用可以使用文件描述符来操作文件。文件描述符等价于一个指向文件的指针。

## Reading And Writing Files
文件的读写可以使用```read```和```write```系统调用。

## Reading And Writing, But Not Sequentially
我们可以使用```lseek```系统调用来设置当前读写位置，
```c++
off_t lseek(int filedes, off_t offset, int whence);
```
其中第一个参数是一个文件描述符，第二个参数是位移，第三个参数是位移参考位置，如果第三个参数为，
1. ```SEEK_SET```: 则文件读写位置直接被设置为offset
2. ```SEEK_CUR```: 则新的文件读写位置为当先读写位置加上offset
3. ```SEEK_END```: 则新的读写位置为文件末尾加上offset. 这里offset通常为负。

## Shared File Table Entries: ```fork``` and ```dup```
每个进程会维护一个打开文件表，其中包含这个进程中打开的文件的描述符。同一个文件可以被多个进程打开。

有fork产生的进程会共享打开文件表。只有当父子进程都关闭文件，文件描述符才会从打开文件表中删除。

```dup()```系统调用可以给已经打开的文件创建另一个文件描述符，新的文件描述符和原来的文件描述符等价。

## Writing Immediately With ```fsync()```
```write```系统调用并不会立即将内容写入磁盘，而是会在内存中缓存一段时间再批量写入文件。```fsync()```要求立即刷新缓冲区，将其中内容写入文件。

## Renameing Files
可以使用```rename(char *old, char *new)```来给文件重命名。

## Getting Information About Files
文件有一系列metadata, 使用```stat()```或```fstat()```系统调用得到metadata.

## Removing Files
命令行中使用的```rm```命令底层调用的是```unlink```系统调用，原因将在后面章节介绍。

## Making Directories
```mkdir```既是命令行中的命令，也是一个系统调用，用于创建文件夹。

## Reading Directories
```opendir()```, ```readdir()```和```closedir()```可以用于操作文件。

## Deleting Directories
```rmdir()```系统调用可以用于删除空白文件夹，目的是防止意外删除某些文件。

## Hard Links
```link()```系统调用可以为文件创建一个新的链接。当我们创建一个文件的时候，我们首先创建这个文件的底层结构，即inode, 然后立即创建一个link指向这个inode。所以在删除文件的时候，我们只需要unlink那个文件就可以了。在这里```link```系统调用只是创建另外一个指向于底层inode的链接。

底层的inode会维护一个reference number，即指向自己的链接的数量。只有当所有链接都被unlink的时候，系统才会释放inode. 

## Symbolic Link
Symbolic Link也称为soft link. 我们不能为文件夹设置hard link; 我们也不能跨分区创建hard link, 因为inode在同一个分区内才是Unique的。这些情况下，soft link会更加有用。

Soft link 是一个不同的数据结构，而hard link与原文件是相同的数据结构。对于hard link来说，如果原文件的位置被删除了，我们依然可以使用hard link来访问这个文件。但是soft link指向的文件被删除了，soft link会失效，称为dangling reference. 

## Permission Bits and Access Control Lists
文件的permission bits有如下的形式，```-rw-r--r--```. 其中第一位表示文件的类型，-表示一般的文件，l表示soft link, d表示directory. 后面九个字符每三个为一组，分别表示owner, group 和 other 用户对于此文件的权限。用户可以使用```chmod```命令修改文件的权限。其中r(4),w(2),x(1), 因此```chmod 700 ```等价于```rwx------```，即owner拥有读写运行权限，其它所有用户没有任何权限。

Access Control List主要用在分布式系统中，是一种比permission bits更加通用的权限管理方式。


## Making and Mounting A File System
在UNIX下，给定一个设备和一个文件格式，```mkfs```命令可以创建一个空白的文件系统。

在得到文件系统之后，我们还需要使操作系统有访问文件系统的权限，这可以通过```mount```来做到。```mount```的功能是将文件系统复制到我们要求的mount point挂载点下。这样mount使得分散在不同设备上的文件系统统合到同一个文件树下。