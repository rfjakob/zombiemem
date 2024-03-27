# zombiemem

In a subthread, allocate and touch 1 GiB of memory.
The main thread exits and will show up as a zombie.

Created for debugging https://github.com/rfjakob/earlyoom/issues/309 .

procps ticket: https://gitlab.com/procps-ng/procps/-/issues/324 ,
copied below for posterity.

# ps: zero rss on zombie main thread

Hi, developer of https://github.com/rfjakob/earlyoom here. I recently discovered that on Linux, the main thread can exit while the other threads keep running. The main thread then shows up as a zombie with zero rss in /proc, while the process obviously still uses memory.

PoC with https://github.com/rfjakob/zombiemem :

```
$ ./zombiemem &
[1] 105435
main thread (pid 105435) exiting
sub thread (pid 105439) now uses 1 GiB of memory. Sleeping.

$ ps axuH | head -1 ; ps axuH | grep zombiemem
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
jakob     105435 13.3  0.0      0     0 pts/0    Zl   19:15   0:00 [zombiemem] <defunct>
jakob     105435 13.3 13.0 1124928 1050240 pts/0 Sl   19:15   0:00 ./zombiemem

$ pstree -p $$
bash(103790)─┬─pstree(105465)
             └─zombiemem(105435)───{zombiemem}(105439)
```

What ps does is totally in line with what's in /proc. My question is more about UX when looking for the memory hog. A process with zombie main thread will be invisible when you sort by rss, unless you enable threads.

Can / should procps look at subthreads when it encounters a zombie that has subthreads?