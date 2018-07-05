# Proteus

> 程序设计 Ⅱ 期末项目郑佳豪 16305204 数据科学与计算机学院 软件工程 17 级教务二班

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
  - 非法节点名称：节点不能设置为 `all` 或 `ALL` ，否则会与 `print all` 冲突
  - 输出非法节点状态：当尝试输出非法节点（即不存在的节点）的电平状态时，会抛出错误

## 设计思路

### 封装

`Proteus` 主要由 `Simulator` 、`Gate` 和 `Pin` 三部分构成，`Simulator` 负责对电路整体的仿真工作，`Gate` 为各类门电路的基类，`Pin` 为电路节点。

我们将 `Pin` 封装至 `Gate` 内部，以提供节点连接功能。然后将 `Gate` 封装到 `Simulator` 内部，以提供门电路逻辑功能。最后，我们通过 `Simulator` 实现**节点连接**、**门电路搭建**、**仿真**等功能。

### 继承与多态

各类门电路对 `Gate` 的 `Calculate` 方法进行实现，然后在 `Simulator` 里通过基类指针调用不同的子类方法实现多态。

```c++
class Pin {
  // ...
  int operator+(const Pin& other) const;  // 并运算
  int operator*(const Pin& other) const;  // 交运算
  int operator!() const;                  // 非运算
  int operator^(const Pin& other) const;  // 异或运算
  // ...
};

class Gate {
 public:
  // ...
  virtual void Calculate() = 0;
  // ...
 protected:
  Pin* in1_;
  Pin* in2_;
  Pin* out_;
};

class AND : public Gate {
 public:
  void Calculate();
};

void AND::Calculate() { 
  this->out_->SetLevel(*this->in1_ * *this->in2_);
}
```

`Simulator` 能对电路整体进行仿真，提供**电路搭建**、**交互式仿真**、**装载仿真**文件等功能。

```C++
class Simulator {
 public:
  Simulator() = default;
  ~Simulator();

  bool Empty() const;
  int GetGatesAmount() const;
  int GetPinsAmount() const;

  bool CheckLevel(Pin::level level) const;  // 检查非法电平
  bool CheckOut(std::string pin) const;     // 检查输出节点
  bool CheckSimpleLoop(std::string pin1,
                       std::string pin2) const;           // 检查简单闭环
  bool CheckLoop(std::string in, std::string out) const;  // 检查复杂闭环

  Pin* SearchPin(std::string name) const;  // 搜索节点
  Pin* AddPin(std::string name);           // 添加节点
  Pin* CheckPin(std::string name);         // 获取节点
  Gate* AddGate(std::string type);         // 添加门

  void Info();                  // 程序信息
  void Interact();              // 交互式
  void Load(std::string file);  // 装载文件
  void Print() const;           // 输出节点状态
  void Clear();
  void Simulate();  // 仿真

 private:
  std::vector<Gate*> gates_;
  std::vector<Pin*> pins_;
};
```

### 运算符重载

```c++
// 并运算
int Pin::operator+(const Pin& other) const {
  if (this->GetLevel() || other.GetLevel()) {
    return HIGH;
  }
  return LOW;
}

// 交运算
int Pin::operator*(const Pin& other) const {
  // ...
}

// 非运算
int Pin::operator!() const {
  // ...
}

// 异或运算
int Pin::operator^(const Pin& other) const {
  // ...
}
```

### 异常处理

+ 闭环检测

```C++
#define CHECK_SET_OUT_AND_LOOP(gate, out, name)                          \
  if (!this->CheckOut(out)) {                                            \
    gate->SetOut(this->CheckPin(out));                                   \
                                                                         \
    if (this->CheckLoop(out, out)) {                                     \
      throw(std::string("error: circuit cannot contain a loop"));        \
    }                                                                    \
  } else {                                                               \
    std::string error = "error: output cannot connect with output\n\t" + \
                        toUpperCase(name) + " pin: " + out;              \
    throw(error);                                                        \
  }
```

+ 设置非法电平和设置输出电平

```c++
#define COMMAND_SET(command, is)                                          \
  if (command == "SET") {                                                 \
    std::string name;                                                     \
    Pin::level level;                                                     \
                                                                          \
    is >> name >> level;                                                  \
                                                                          \
    if (!this->CheckOut(name)) {                                          \
      if (!this->CheckLevel(level)) {                                     \
        this->CheckPin(name)->SetLevel(level);                            \
      } else {                                                            \
        std::string error =                                               \
            "error: level must be 0 or 1\n\t" + name + " must be 0 or 1"; \
        throw(error);                                                     \
      }                                                                   \
    } else {                                                              \
      std::string error =                                                 \
          "error: output cannot be set\n\t" + name + " cannot be set";    \
      throw(error);                                                       \
    }                                                                     \
                                                                          \
    continue;                                                             \
  }
```

+ 非法节点名称

```c++
#define CHECK_PIN_NAME(name)                                        \
  if (name == "all" || name == "ALL") {                             \
    throw(std::string(                                              \
        "error: cannot use reserved word as the name of the pin")); \
  }
```

+ 非法命令判断

```c++
void Simulator::Interact() {
  // ...
  do {
	// ...
    try {
      if (command == "EXIT") {
        // ...
      }
	  // ...
      throw(temp + ": command not found");
    } catch (const std::string error) {
      std::cout << error << std::endl;
      // ...
    }
  } while (1);
}
```

+ 装载非法文件

```C++
void Simulator::Load(std::string file) {
  std::ifstream fs;

  fs.open(file);

  if (!fs) {
    throw(std::string("load: " + file + ": No such file or directory"));
  }

  while (!fs.eof()) {
    // ...
  }

  fs.close();
}
```

### 宏

由于 `Interact` 和 `Load` 代码重复度高，故使用了 `宏` 进行代码简化。

```c++
#define CHECK_PIN_NAME(name)
#define CHECK_SET_OUT_AND_LOOP(gate, out, game)
#define CHECK_SIMPLE_LOOP_NOT(in1, out)
#define CHECK_SIMPLE_LOOP(name, in1, in2, out)
#define COMMAND_ECHO(command, is)
#define COMMAND_PRINT(command, is)
#define COMMAND_ADD(command, is)
#define COMMAND_SET(command, is)
#define COMMAND_RESET(command)
#define COMMAND_SIM(command)
```

### 代码风格

使用了 `Google C++ Style` ，在开发过程中，严格遵守了类内变量以下划线结尾的规则，在审阅代码时，效率得到了提高。

## 扩展

- 增加了与非门、或非门、同或门
- 支持默认输入为低电平
- 类 `Terminal` 界面，提升用户使用体验
- 支持**交互式**和**装载文件**两种电路模拟方式
- 异常处理
  - 非法文件：当尝试载入非法文件（即不存在的文件）时，会抛出错误
  - 非法指令：当输入非法指令或添加非有效门时，会抛出错误
  - 闭环：同一器件的输入和输出不能直接或间接地相连，**该模拟器不支持时序电路设计**
  - 非法电平：电平只能设置为 `0` 和 `1`
  - 设置输出电平：输出电平不能人为设置
  - 非法节点名称：节点不能设置为 `all` 或 `ALL` ，否则会与 `print all` 冲突
  - 输出非法节点状态：当尝试输出非法节点（即不存在的节点）的电平状态时，会抛出错误

## 测试文件

在项目的 `test/` 文件夹下，有以下测试文件，我们可在程序运行后键入类似 `load test\and.txt`  的命令，执行相应的测试文件。

+ `and.txt` 与门
+ `nand.txt` 与非门
+ `nor.txt` 或非门
+ `or.txt` 或门
+ `xnor.txt` 同或门
+ `xor.txt` 异或门
+ `full_adder.txt` 全加器
+ `loop_error.txt` 闭环的错误检测
+ `print_error_pin.txt` 输出非法节点状态的错误检测
+ `set_error_level.txt` 异常电平的错误检测
+ `set_out_level.txt` 设置输出电平的错误检测
