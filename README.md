# Proteus

> 程序设计 Ⅱ 期末项目 郑佳豪 16305204 数据科学与计算机学院 软件工程 17 级教务二班

## 项目说明

> 基于 `C++` 的简单逻辑门电路模拟器

- 支持**交互式**和**装载文件**两种方式，进行电路模拟
- 支持`AND` `OR` `NOT` `NAND` `NOR` `XOR` `XNOR`
- 输入悬空默认为**低电平**
- 异常处理
  - 非法文件：当尝试载入非法文件（即不存在的文件）时，会抛出错误
  - 非法指令：当输入非法指令或添加非有效门时，会抛出错误
  - 闭环：同一器件的输入和输出不能直接或间接地相连，**该模拟器不支持时序电路设计**
  - 非法电平：电平只能设置为 `0` 和 `1`
  - 设置输出电平：输出电平不能人为设置
  - 非法节点名称：节点不能设置为 `all` 或 `ALL`
  - 输出非法节点状态：当尝试输出非法节点的电平状态时，会抛出错误

## 目录结构

- `include/` 存放头文件
- `src/` 存放实现文件
- `test/` 存放测试文件

## 编译运行

运行环境： `Windows`

本项目使用了 `Makefile` 构建，故可在项目根目录执行 `make` 命令进行编译。若出现编译错误，可键入以下命令进行构建。

```bash
g++ ./src/*.cpp -std=c++11 -I. -o Proteus
```

当编译完成后，即可执行 `Proteus.exe` 运行项目。

## 指令介绍

- 显示帮助信息

  ```bash
  proteus> help
  ```

- 输出注释

  ```bash
  proteus> echo hello_world   // 注释不能出现空格
  hello_world
  ```

- 退出程序

  ```bash
  proteus> exit
  ```

- 添加门电路

  > add <gate> <in_pin> [<in_pin>] <out_pin>

  ```bash
  proteus> add and a b c   // a b 为输入 c 为输出
  proteus> add not a b     // a 为输入 b 为输出
  ```

- 设置电平

  > set <pin> <0 | 1>

  ```bash
  proteus> set a 1   // 设定 a 节点为高电平
  ```

- 装载文件

  > load <file>

  ```bash
  proteus> load test\and.txt   // 装载 test 文件夹下的 and.txt 文件
  ```

- 执行仿真

  ```bash
  proteus> sim
  ```

- 输出节点状态

  ```bash
  proteus> print A     // 输出节点 A 的状态
  proteus> print all   // 输出所有节点的状态
  ```

- 重置电路

  ```bash
  proteus> reset
  ```
