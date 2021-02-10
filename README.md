# ThreeEasyPieces
这个repository是我在阅读*Three Easy Pieces*时的笔记。我主要用汉语书写，以便日后自己可以快速复习，当然，到处可见中英文夹杂的坏习惯。

古人云：书读百遍其义自见。这不过是我的第二遍，很多先前不明白的地方就已经清晰很多了。古人诚不我欺也！我还记得第一次读的时候在进程切换、并发等关键章节基本都是懵的。与上一次不同的是，在这一遍之前我已经读过了CSAPP，尤其是有了一些x86汇编的概念。回头来再看Three Easy Pieces关于进程、进程管理的部分就豁然开朗了。这一遍在并发的部分还是花了不少时间，理解也更加深入了。只不过我的日常代码主要用MPI做并行计算，很少用到并发，因此还需要多找机会练习并发编程。本书最后三节有关于分布式系统的部分，我决定暂时放在一边，日后如需用到再来学习吧。

总得来说，这本书让我受益匪浅。以往在写项目代码的时候，常常会感觉知其然而不知其所以然，这大概是许多没有受过计算机科班训练的理工科人士的通病吧。

本书地址：https://pages.cs.wisc.edu/~remzi/OSTEP/

For myself: 登录[这里](https://gumroad.com/library)下载购买的PDF.

- [ThreeEasyPieces](#threeeasypieces)
- Part I. Introduction
  - [01. A Dialogue on the Book](part0-intro/lecture01/README.md)
  - [02. Introduction to Operating Systems](part0-intro/lecture02/README.md)
- Part II. Virtualization
  - [03. A Dialogue on Virtualization](part1-virtualization/lecture03/README.md)
  - [04. The Abstraction: The Process](part1-virtualization/lecture04/homework/README.md)
  - [05. Interlude: Process API](part1-virtualization/lecture05/REAME.md)
  - [06. Mechanism: Limited Direct Execution](part1-virtualization/lecture06/README.md)
  - [07. Scheduling: Introduction](part1-virtualization/lecture07/homework/README.md)
  - [08. Scheduling: The Multi-Level Feedback Priority Queue](part1-virtualization/lecture08/README.md)
  - [09. Scheduling: Proportional Share](part1-virtualization/lecture09/README.md)
  - [10. Multiprocessor Scheduling](part1-virtualization/lecture10/README.md)
  - [11. Summary Dialogue on CPU Virtualization](part1-virtualization/lecture11/README.md)
  - [12. A Dialogue on Memory Virtualization](part1-virtualization/lecture12/README.md)
  - [13. The Abstraction: Address Spaces](part1-virtualization/lecture13/README.md)
  - [14. Interlude: Memory API](part1-virtualization/lecture14/README.md)
  - [15. Mechanism: Address Translation](part1-virtualization/lecture15/README.md)
  - [16. Segmentation](part1-virtualization/lecture16/README.md)
  - [17. Free-Space Management](part1-virtualization/lecture17/README.md)
  - [18. Paging: Introduction](part1-virtualization/lecture18/README.md)
  - [19. Paging Faster Translations (TLBs)](part1-virtualization/lecture19/README.md)
  - [20. Paging: Smaller Tables](part1-virtualization/lecture20/README.md)
  - [21. Beyond Physical Memory: Mechanisms](part1-virtualization/lecture21/README.md)
  - [22. Beyond Physical Memory: Policies](part1-virtualization/lecture22/README.md)
  - [23. Complete Virtual Memory Systems](part1-virtualization/lecture23/README.md)
  - [24. Summary Dialogue on Memory Virtualization](part1-virtualization/lecture24/README.md)
- Concurrency
  - [25. A Dialogue on Concurrency](part2-concurrency/lecture25/README.md)
  - [26. Concurrency: An Introduction](part2-concurrency/lecture26/README.md)
  - [27. Interlude: Thread API](part2-concurrency/lecture27/README.md)
  - [28. Locks](part2-concurrency/lecture28/README.md)
  - [29. Lock-based Concurrent Data Structures](part2-concurrency/lecture29/README.md)
  - [30. Condition Variables](part2-concurrency/lecture30/README.md)
  - [31. Semaphores](part2-concurrency/lecture31/README.md)
  - [32. Common Concurrency Problems](part2-concurrency/lecture32/README.md)
  - [33. Event-based Concurrency](part2-concurrency/lecture33/README.md)
  - [34. Summary Dialogue on Concurrency](part2-concurrency/lecture34/README.md)
- Part III. Persistence
  - [35. A Dialogue on Persistence](part3-persistence/lecture35/README.md)
  - [36. IO Devices](part3-persistence/lecture36/README.md)
  - [37. Hard Disk Devices](part3-persistence/lecture37/README.md)
  - [38. Redundant Arrays of Inexpensive Disks (RAIDs)](part3-persistence/lecture38/README.md)
  - [39. Interlude: Files and Directories](part3-persistence/lecture39/README.md)
  - [40. File System Implementation](part3-persistence/lecture40/README.md)
  - [41. Locality and The Fast File System](part3-persistence/lecture41/README.md)
  - [42. Crash Consistency: FSCK and Journaling](part3-persistence/lecture42/README.md)
  - [43. Log-structured File Systems](part3-persistence/lecture43/README.md)
  - [44. Flash-based SSDs](part3-persistence/lecture44/README.md)
  - [45. Data Integrity and Protection](part3-persistence/lecture45/README.md)
  - [46. Summary Dialogue on Persistence](part3-persistence/lecture46/README.md)
  - [47. A Dialogue on Distribution](part3-persistence/lecture47/README.md)
  - [ ] 48. Distributed Systems
  - [ ] 49. Sun's Network File System (NFS)
  - [ ] 50. The Andrew FIle System (AFS)