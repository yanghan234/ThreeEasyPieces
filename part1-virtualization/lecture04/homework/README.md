# My Solution
1. 100%, because neither of the processes issue IO requests
2. Total time 10, CPU Busy 50%. This can only be obtained by running the code.
3. Total time 6, CPU Busy 83.33%. Yes, context switch does make a difference. When p0 issues IO request, p1 starts to run.
4. Total time 9, CPU Busy 55.56%.
5. Total time 6, CPU Busy 83.33%.
6. No. The IO heavy process is still unfinished after other processes are already finished.
7. Now. It is more efficient.
8. Time used and CPU busy rate
```bash
./process-run.py -s 3 -l 3:50,3:50 -p -c
```

|               | IO_RUN_LATER | IO_RUN_IMMEDIATE |
| ---           | ---          | ---              |
| SWITCH_ON_END | 18, 33.33%   | 18, 33.33%       |
| SWITCH_ON_IO  | 13, 46.15%   | 13, 46.15%       |