# Data Integrity and Protection

## Disk Failure Modes
RAID虽然可以提供保护，但是这种保护也不是绝对的。早期的RAID假定一块硬盘要么可用，要么真个fail掉，即fail-stop model. 这个模型简单，便于构建RAID, 但是显然不靠谱。。。

现代的磁盘可能偶尔会遇到一些block暂时不可用，或者一些block坏掉，即使如此，磁盘的其它大部分仍然是可用的。

下面介绍两种single-block failure模型：latent-sector errors(LSE) 和 block corruption.

LSE通常发生在磁盘中的扇区受损，例如磁头在读取的时候碰到了盘面(head crash). 对于这种类型的问题，磁盘中的error correcting codes (ECC)会试图修复受损扇区中的内容，如果无法修复则会报错。

而corruption的发生是磁盘自身无法检测到的，例如磁盘中的软件有bug，它以为自己将数据存入了正确的位置，但实际上并没有，那么磁盘以为自己没问题，但是读取的数据却总是错的。Corruption通常称为silent faults, 应为硬盘并不会提示错误。

值得注意的是，虽然disk faults发生的概率不是很高，但是即使是质量最好的磁盘照样有一定的概率发生错误，因此我们有必要对此进行处理。

## Handling Latent Sector Errors
因为LSE是硬盘可以自行检测到的，因此我们只需在其发生时对其进行处置。LSE的修复仰仗于冗余信息，例如RAID中如果有Mirror, 则可以到Mirror中找回信息。如果RAID中有parity, 则根据Partiy计算发生LSE的位置应该为什么。值得注意的是，RAID-4/5中至多允许一个磁盘出问题，这就意味着，如果两个或以上的磁盘的同一个扇区出了LSE, 则数据无法修复，返回错误。

## Detecting Corruption: The Checksum
Corruption的关键点在于检测，而检测到corruption之后修复的方式与LSE类似，也依赖于冗余的数据，否则无法修复。

Corruption的检测主要依赖于checksum， 即在写入数据的时候计算数据的checksum并保存到相应区域，在读取的时候计算读取数据的checksum并与磁盘中保存的checksum进行比较。当然，checksum匹配并不能保证万无一失，因为checksum可能碰撞。

## Misdirected write
Misdirected write是指文件系统以为自己将数据写到了某个位置，但是数据不知什么原因实际上却写到了别的位置。预防这种错误可以选择在写入数据的时候同时保存文件系统希望保存的位置信息，这样磁盘可以验证文件系统想写入的位置究竟是不是自己。

## Lost write
磁盘通知系统它已经完成了某次写入，但是不知什么原因，它并没有写入磁盘，这种现象称为Lost write.

Read-after-write写后即读可以用于检测lost write, 但这显然是一种效率很低的方法。

另一种方法是在文件的inode中保存文件data的checksum, 这种方法在一定程度下有效，但是极端情况下，有可能inode和data都是lost write, 这样还是解决不了lost write.

## Scrubbing
磁盘中的很多数据并不会被经常使用，这样就意味着它们不会被经常检测，如果它们中出现了LSE或者corruption都无法知晓。为此，文件系统采用disk scrubbing方式，即周期性地把不常用的数据也读一遍验证一下。

## Overheads of Checksum
Checksum机制的空间额外开销很小，如果一个4KB的block中使用8bytes用于存储chekcsum，那么额外开销仅为0.19%. 

Checksum机制的时间额外开销较大，一方面CPU需要根据数据计算checksum, 另一方面，读数据的时候，有可能checksum与数据本身并不在一起，那么磁盘需要付出额外的时间来读取checksum. 另外，disk scrubbing要付出很多时间，不过这个可以在用户活动不繁忙的时候进行。
