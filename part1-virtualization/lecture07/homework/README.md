1. Average response and turnaround time of SJF and FIFO
```bash
./scheduler.py -j 200 -p FIFO -s 1 -c
```

|      | response (ms) | turnaround (ms) |
| ---  | ---           | ---             |
| SJF  | 373.45        | 378.82          |
| FIFO | 537.29        | 542.66          |
2. SJF is systematically better than FIFO
```bash
./scheduler.py -j 100 -p FIFO -s 1 -c
```

|      | response (ms) | turnaround (ms) |
| ---  | ---           | ---             |
| SJF  | 199.89        | 205.54          |
| FIFO | 265.46        | 271.11          |

```bash
./scheduler.py -j 300 -p FIFO -s 1 -c
```

|      | response (ms) | turnaround (ms) |
| ---  | ---           | ---             |
| SJF  | 563.06        | 568.51          |
| FIFO | 810.81        | 816.26          |

3. RR with time slicing = 1. RR has better response time, but worse turnaround time.
```bash
./scheduler.py -j 300 -p RR -s 1 -c -q 1
```

|      | response (ms) | turnaround (ms) |
| ---  | ---           | ---             |
| SJF  | 563.06        | 568.51          |
| FIFO | 810.81        | 816.26          |
| RR-1 | 149.50        | 1048.63         |

4. If every job has the same length, then SJF is the same as FIFO.
5. If every job has the length of time-slice of RR, SJF and RR are equivalent.
6. As job lengths increase, response time with SJF increases.
7. As RR quantum lengths increase, response time increase. When quantum length is long enough, RR will be the same as FIFO.