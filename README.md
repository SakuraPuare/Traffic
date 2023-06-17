# Traffic Inquiry System

## Introduction

交通路线查询系统

C语言期末大作业，主要基于图的最短路径算法，实现了一个交通路线查询系统。

虽然是用C++写的，但这就是C语言期末大作业 ^_^

## Requirements

- CMake >= 3.25
- C++ >= 17
- C++ Standard Library

## Usage

### Compile

```bash
cmake --build ./build --target all 
```

### Run

```bash
./build/Traffic
```

### ScreenShot

```text
===========================================
          Traffic Inquiry System
                  Feature
-------------------------------------------
     0: Map Information
     1: Map Node Info
     2: Map Edge Info
     3: Map Matrix Info
     4: All Edge Between Two Nodes
     5: Shortest Edge Between Two Nodes
     6: Add Node
     7: Set Node
     8: Delete Node
     9: Add Edge
    10: Set Edge
    11: Delete Edge
    12: Load Map
    13: Save Map
    14: Reset Map
    15: Quit System
===========================================
```

## Features

- [x] 交互式命令行界面
- [x] 交通路线查询
- [x] 交通路线修改
- [x] 交通路线删除
- [x] 交通路线添加
- [x] 交通路线导出
- [x] 交通路线导入
- [ ] 交通路线图形化展示
- [ ] 交通路线导航
- [ ] 交通路线推荐

## License

[MIT](./LICENSE)