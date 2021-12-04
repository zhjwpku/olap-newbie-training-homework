### 文件切分结果

```
[ RUN      ] solution.test1
thread 140558163339008 handle slice [beg: 0; end: 9947840556; first to read: 0]
thread 140558154946304 handle slice [beg: 9947840512; end: 19895681135; first to read: 9947840556]
thread 140558146553600 handle slice [beg: 19895681024; end: 29843521592; first to read: 19895681135]
thread 140558138160896 handle slice [beg: 29843521536; end: 39791362135; first to read: 29843521592]
thread 140558129768192 handle slice [beg: 39791362048; end: 49739202615; first to read: 39791362135]
thread 140558121375488 handle slice [beg: 49739202560; end: 59687043101; first to read: 49739202615]
thread 140558039250688 handle slice [beg: 59687043072; end: 69634883640; first to read: 59687043101]
thread 140558030857984 handle slice [beg: 69634883584; end: 79579694556; first to read: 69634883640]
```

使用 hd 可查看切分之后最后一个字符是否为 '\n'。

### 纯读取文件

8线程 1MB read

```
root@byteboy:/home/olap-newbie-training-homework/build# ./bin/solutionTest 
Running main() from gtest_main.cc
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from solution
[ RUN      ] solution.test1
[       OK ] solution.test1 (269970 ms)
[----------] 1 test from solution (269970 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (269970 ms total)
[  PASSED  ] 1 test.
```

4线程 1MB read

```
root@byteboy:/home/olap-newbie-training-homework/build# ./bin/solutionTest 
Running main() from gtest_main.cc
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from solution
[ RUN      ] solution.test1
[       OK ] solution.test1 (270001 ms)
[----------] 1 test from solution (270001 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (270001 ms total)
[  PASSED  ] 1 test.
```

使用 iostat 可以看 IO

```
Device            r/s     w/s     rkB/s     wkB/s   rrqm/s   wrqm/s  %rrqm  %wrqm r_await w_await aqu-sz rareq-sz wareq-sz  svctm  %util
vda              0.00    0.00      0.00      0.00     0.00     0.00   0.00   0.00    0.00    0.00   0.00     0.00     0.00   0.00   0.00
vdb           1917.00    3.00 286180.00     36.00     0.00     6.00   0.00  66.67   17.71   16.00  32.87   149.29    12.00   0.52  99.60

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
           0.00    0.00    2.26   45.11    0.00   52.63

Device            r/s     w/s     rkB/s     wkB/s   rrqm/s   wrqm/s  %rrqm  %wrqm r_await w_await aqu-sz rareq-sz wareq-sz  svctm  %util
vda              0.00    0.00      0.00      0.00     0.00     0.00   0.00   0.00    0.00    0.00   0.00     0.00     0.00   0.00   0.00
vdb           2364.00    0.00 287012.00      0.00     0.00     0.00   0.00   0.00   17.57    0.00  40.65   121.41     0.00   0.42  99.60

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
           0.00    0.00    1.51   52.14    0.00   46.35

Device            r/s     w/s     rkB/s     wkB/s   rrqm/s   wrqm/s  %rrqm  %wrqm r_await w_await aqu-sz rareq-sz wareq-sz  svctm  %util
vda              0.00    0.00      0.00      0.00     0.00     0.00   0.00   0.00    0.00    0.00   0.00     0.00     0.00   0.00   0.00
vdb           1920.00    7.00 286748.00     28.00     0.00     0.00   0.00   0.00   17.91   14.29  33.89   149.35     4.00   0.52 100.00

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
           0.00    0.00    1.51   51.39    0.00   47.10
```

我在全息平台买的磁盘规格为 320 G PL1 ESSD, 根据 [云盘概述](https://help.aliyun.com/document_detail/25383.html) 里的单盘吞吐量性能计算公式，其吞吐性能的 SLA 为:

min{120+0.5*容量, 350} = min{120 + 0.5 * 320} = 280 MB/s，基本符合实测效果。

运行时间

load linetime split file use time 75701 ns.
read and parse the file use time 268468140731 ns.
merge unordered_map to map use time 17599990363 ns.
The max value between 100 and 200 is key:137, value: zzle slyly special ide; used time 26268 ns.
The max value between 200 and 2000 is key:892, value: zzle. special ; used time 336874 ns.
The max value between 5000 and 6000 is key:5126, value: zzle; slyly ironic account; used time 203284 ns.
The max value between 2345 and 6789 is key:5126, value: zzle; slyly ironic account; used time 837166 ns.
The max value between 123 and 456 is key:375, value: zzle. fluffily r; used time 57281 ns.