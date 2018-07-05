#ifndef PROTEUS_INCLUDE_SIMULATOR_H_
#define PROTEUS_INCLUDE_SIMULATOR_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "AND.h"
#include "NAND.h"
#include "NOR.h"
#include "NOT.h"
#include "OR.h"
#include "XNOR.h"
#include "XOR.h"
#include "gate.h"
#include "pin.h"

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

#endif