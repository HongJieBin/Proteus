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

  Pin* SearchPin(std::string name) const;
  Pin* AddPin(std::string name);
  Pin* CheckPin(std::string name);
  Gate* AddGate(std::string type);
  void Info();
  void Interact();
  void Load(std::string file);
  void Print() const;
  void Clear();
  void Simulate();

 private:
  std::vector<Gate*> gates_;
  std::vector<Pin*> pins_;
};

#endif