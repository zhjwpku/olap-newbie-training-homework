# olap-newbie-training-homework

### 问题描述

给定 TPC-H lineitem 数据表，计算满足 param1 < l_suppkey < param2 条件的的最大l_comment

使用SQL表达，则为：

```
select max(l_comment) from lineitem where l_suppkey > {param1} and l_suppkey <{param2}
```

程序在启动时，需要读入标准 100GB lineitem.tbl 文件，并构建索引或排序，将索引和数据存储在本地磁盘上。
然后读取n组参数（每组为两个整数，param1和param2），然后分别打印每组条件下的最大 comment。

测试样例{实际参数不定}：

```
输入：
5
100 200
200 2000
5000 6000
2345 6789
123  456

输出：
zzle throughout the
zzle? slyly final platelets sleep quickly.
zzle? slyly final platelets sleep quickly.
zzle? blithely ironi
zzle. special sentiments along
```

### 生成 100GB lineitem.tbl 文件

使用 [tpch-gen](https://github.com/eyalroz/tpch-dbgen/) 生成 lineitem.tbl 文件，执行如下命令:

```
nohup ./tpch-dbgen/build/dbgen -v -s 100 -b tpch-dbgen/dists.dss </dev/null &>/dev/null &
```

其中 -s 参数为 scale factor(SF) 值，根据该值可估算 TPC-H 所有表的大小，-s 100 即可生成大约 100GB 的数据库，其中 lineitem.tbl 大概 75GB


### 问题分析

首先看一下 TPC-H 的 schema:

![TPC-H schema](./img/tpch-schema.jpg)

由于只需处理 lineitem 的数据，查看 [TPC-H Standard Specification](http://tpc.org/tpc_documents_current_versions/pdf/tpc-h_v3.0.0.pdf) 的描述，lineitem 大约有 SF * 6,000,000 条数据，由于使用了 -s 100 生成数据，可知 lineitem 大概有 6 亿条数据。

再看手册中关于 Lineitem 的表结构说明:

```
Column Name        Datatype Requirements        Comment

L_ORDERKEY         identifier                   Foreign Key to O_ORDERKEY
L_PARTKEY          identifier                   Foreign key to P_PARTKEY, first part of the compound Foreign Key to (PS_PARTKEY,PS_SUPPKEY) with L_SUPPKEY
L_SUPPKEY          identifier                   Foreign key to S_SUPPKEY, second part of the compound Foreign Key to (PS_PARTKEY, PS_SUPPKEY) with L_PARTKEY
L_LINENUMBER       integer
L_QUANTITY         decimal
L_EXTENDEDPRICE    decimal
L_DISCOUNT         decimal
L_TAX              decimal
L_RETURNFLAG       fixed text, size 1
L_LINESTATUS       fixed text, size 1
L_SHIPDATE         date
L_COMMITDATE       date
L_RECEIPTDATE      date
L_SHIPINSTRUCT     fixed text, size 25
L_SHIPMODE         fixed text, size 10
L_COMMENT          variable text size 44

Primary Key: L_ORDERKEY, L_LINENUMBER
```

再看下我们要处理的问题：

```
select max(l_comment) from lineitem where l_suppkey > {param1} and l_suppkey <{param2}
```

上述语句只关心 l_comment 和 l_suppkey，而 l_suppkey 最终来自与 SUPPLIER 这张表：

```
Column Name        Datatype Requirements        Comment
S_SUPPKEY          identifier                   SF*10,000 are populated
S_NAME             fixed text, size 25
S_ADDRESS          variable text, size 40
S_NATIONKEY        identifier                   Foreign Key to N_NATIONKEY
S_PHONE            fixed text, size 15
S_ACCTBAL          decimal
S_COMMENT          variable text, size 101

Primary Key: S_SUPPKEY
```

其数据总量为 SF * 10,000，即 1,000,000 条数据。

返过来再来看 l_comment 字段，数据类型为 `variable text size 44`。由于最后要求的是最大值，因此对于每个 SUPPKEY，只需要根据扫描得到的数据只保存最大值即可。因此在 load 数据阶段，只需要 1,000,000 * (4 + 44) B 数据，不超过 64 MB 数据，而且评测程序不会重启，因此只需要内存就可以了。

### 方案设计

1. 多线程读取文件，每个线程分别使用 unordered_map 保存各个键对应的 comment 值，当一个键已经存在，则通过比较决定是否更新。
2. 读取完之后将各线程的 unordered_map 合并到一个 map 中，供 query 查询。

### Build & Run

```
apt install -y googletest
mkdir build && cd build
cmake ..
make -j4
```

执行程序

```
./bin/solutionTest
```

### 耗时分析

ECS 实例规格: ecs.c7.xlarge（4 vCPU 8 GiB），磁盘 320G PL1

- 读文件并解析文件消耗最主要的时间 270 秒左右
- 将 unordered_map 合并到 map 耗时 18 秒左右
- 查询时间跟查询范围相关，当前用例都在1毫秒以下，如果范围太大可以考虑增加辅助结构加速查询

### 一些可能的优化点

- 将 supp_key 的取值范围进行分区，每个分区分别记录其最大值，对于查询跨多个分区的，分三部分进行查询
- 由于 supp_key 的取值为 [0,1000000)，可以使用 vector 替换 unordered_map 和 merge 阶段的 map
