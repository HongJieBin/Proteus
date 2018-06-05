#include "include/simulator.h"

Simulator::~Simulator() {
  for (unsigned int i = 0; i < this->gates_.size(); i++) {
    delete this->gates_[i];
  }

  for (unsigned int i = 0; i < this->pins_.size(); i++) {
    delete this->pins_[i];
  }
}

int Simulator::GetGatesAmount() const { return this->gates_.size(); }

int Simulator::GetPinsAmount() const { return this->pins_.size(); }

Pin* Simulator::SearchPin(std::string name) const {
  for (unsigned int i = 0; i < this->pins_.size(); i++) {
    if (this->pins_[i]->GetName() == name) {
      return this->pins_[i];
    }
  }

  return nullptr;
}

Pin* Simulator::AddPin(std::string name) {
  Pin* pin = new Pin(name);

  this->pins_.push_back(pin);
  return pin;
}

Pin* Simulator::CheckPin(std::string name) {
  Pin* pin = this->SearchPin(name);

  if (pin) {
    return pin;
  }

  return this->AddPin(name);
}

Gate* Simulator::AddGate(std::string type) {
  Gate* gate = nullptr;

  if (type == "NOT")
    gate = new NOT();
  else if (type == "AND")
    gate = new AND();
  else if (type == "OR")
    gate = new OR();
  else if (type == "XOR")
    gate = new XOR();
  else if (type == "NAND")
    gate = new NAND();
  else if (type == "NOR")
    gate = new NOR();
  else if (type == "XNOR")
    gate = new XNOR();
  else {
    std::cout << "Error Loading Design" << std::endl;
    std::cout << "Please refer to User Guide" << std::endl;
    return gate;
  }

  this->gates_.push_back(gate);

  return gate;
}

void Simulator::Print() const {
  for (unsigned int i = 0; i < this->pins_.size(); i++) {
    this->pins_[i]->Print();
  }
}

void Simulator::Simulate() {
  for (unsigned int i = 0; i < this->gates_.size(); i++) {
    this->gates_[i]->Calculate();
  }
}

void Simulator::Load(std::string file) {
  std::ifstream fs;

  fs.open(file);

  while (!fs.eof()) {
    std::string chunk;

    fs >> chunk;

    if (chunk == "SET") {
      std::string name;
      Pin::level level;

      fs >> name >> level;
      this->CheckPin(name)->SetLevel(level);
      continue;
    }

    if (chunk == "PRINT") {
      std::string name;

      fs >> name;
      name == "ALL" ? this->Print() : this->CheckPin(name)->Print();
      continue;
    }

    if (chunk == "SIM") {
      this->Simulate();
      continue;
    }

    Gate* gate = this->AddGate(chunk);
    std::string in1, in2, out;

    if (!gate) {
      break;
    }

    if (chunk != "NOT") {
      fs >> in1 >> in2 >> out;
      gate->SetIn1(this->CheckPin(in1));
      gate->SetIn2(this->CheckPin(in2));
      gate->SetOut(this->CheckPin(out));
      continue;
    }

    fs >> in1 >> out;
    gate->SetIn1(this->CheckPin(in1));
    gate->SetIn2(this->CheckPin(in1));
    gate->SetOut(this->CheckPin(out));
  }

  fs.close();
}