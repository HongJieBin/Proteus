#include "include/simulator.h"

std::string toUpperCase(std::string str) {
  std::string temp = str;

  std::transform(str.begin(), str.end(), temp.begin(), ::toupper);
  return temp;
}

Simulator::~Simulator() { this->Clear(); }

bool Simulator::Empty() const {
  return !this->gates_.size() || !this->pins_.size();
}

int Simulator::GetGatesAmount() const { return this->gates_.size(); }

int Simulator::GetPinsAmount() const { return this->pins_.size(); }

bool Simulator::CheckSelfLoop(std::string pin1, std::string pin2) const {
  return pin1 == pin2;
}

Pin* Simulator::SearchPin(std::string name) const {
  for (auto& iter : this->pins_) {
    if (iter->GetName() == name) {
      return iter;
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
    return gate;
  }

  this->gates_.push_back(gate);

  return gate;
}

void Simulator::Info() {
  std::cout << "Proteus - An application for simulating circuits" << std::endl;
  std::cout << "Created by Jiahonzheng" << std::endl;
  std::cout << "Enter 'help' to know more usage information" << std::endl;
}

void Simulator::Interact() {
  std::string temp;
  std::string command;

  do {
    std::cout << "proteus> ";
    std::cin >> temp;
    command = toUpperCase(temp);

    if (command == "EXIT") break;

    if (command == "HELP") {
      std::cout << "Usage: [options]" << std::endl << std::endl;
      std::cout << "Proteus: An application for simulating logic circuits"
                << std::endl
                << std::endl;
      std::cout << "Options:" << std::endl << std::endl;
      std::cout << "\thelp\t\t\t\t\t\t\tOutput usage information" << std::endl;
      std::cout << "\tclear\t\t\t\t\t\t\tClear the terminal screen"
                << std::endl;
      std::cout << "\texit\t\t\t\t\t\t\tExit the program" << std::endl;
      std::cout << "\tload <file>\t\t\t\t\t\tLoad the existed circuit"
                << std::endl;
      std::cout << "\tadd <gate> <in_pin> [<in_pin>] <out_pin>\t\tAdd "
                   "specified type of gate"
                << std::endl;
      std::cout << "\tprint <pin> | all\t\t\t\t\tPrint the specific pin or all "
                   "the pins"
                << std::endl;
      std::cout << "\treset\t\t\t\t\t\t\tReset the circuit to empty state"
                << std::endl;
      std::cout << "\tsim\t\t\t\t\t\t\tSimulate the circuit" << std::endl;
      std::cout << std::endl;
      continue;
    }

    if (command == "SET") {
      std::string name;
      Pin::level level;

      std::cin >> name >> level;
      this->CheckPin(name)->SetLevel(level);
      continue;
    }

    if (command == "PRINT") {
      std::string name;

      std::cin >> name;
      toUpperCase(name) == "ALL" ? this->Print()
                                 : this->CheckPin(name)->Print();
      continue;
    }

    if (command == "SIM") {
      this->Simulate();
      continue;
    }

    if (command == "ADD") {
      std::string name;
      Gate* gate = nullptr;
      std::string in1, in2, out;

      std::cin >> name;
      name = toUpperCase(name);
      gate = this->AddGate(name);

      if (!gate) {
        std::cout << "Unsupported gate" << std::endl;
        std::cin.clear();
        std::cin.sync();
        continue;
      }

      if (name != "NOT") {
        std::cin >> in1 >> in2 >> out;

        if (this->CheckSelfLoop(in1, out) || this->CheckSelfLoop(in2, out)) {
          std::string error =
              "error: output cannot connect with input in the same gate";
          throw(error);
        }

        gate->SetIn1(this->CheckPin(in1));
        gate->SetIn2(this->CheckPin(in2));
        gate->SetOut(this->CheckPin(out));
        continue;
      }

      std::cin >> in1 >> out;

      if (this->CheckSelfLoop(in1, out)) {
        std::string error =
            "error: output cannot connect with input in the same gate";
        throw(error);
      }

      gate->SetIn1(this->CheckPin(in1));
      gate->SetIn2(this->CheckPin(in1));
      gate->SetOut(this->CheckPin(out));
      continue;
    }

    if (command == "RESET") {
      if (this->Empty()) {
        std::cout << "No pins or gates to clear" << std::endl;
      }

      this->Clear();
      continue;
    }

    if (command == "CLEAR") {
      system("cls");
      continue;
    }

    if (command == "LOAD") {
      std::string file;

      std::cin >> file;

      try {
        this->Load(file);
      } catch (const std::string error) {
        std::cout << error << std::endl;
      }

      this->Clear();
      continue;
    }

    std::cout << temp << ": command not found" << std::endl;
  } while (1);
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

      if (this->CheckSelfLoop(in1, out) || this->CheckSelfLoop(in2, out)) {
        std::string error =
            "error: output cannot connect with input in the same gate";
        throw(error);
      }

      gate->SetIn1(this->CheckPin(in1));
      gate->SetIn2(this->CheckPin(in2));
      gate->SetOut(this->CheckPin(out));
      continue;
    }

    fs >> in1 >> out;

    if (this->CheckSelfLoop(in1, out)) {
      std::string error =
          "error: output cannot connect with input in the same gate";
      throw(error);
    }

    gate->SetIn1(this->CheckPin(in1));
    gate->SetIn2(this->CheckPin(in1));
    gate->SetOut(this->CheckPin(out));
  }

  fs.close();
}

void Simulator::Print() const {
  if (!this->pins_.size()) {
    std::cout << "No pins or gates to clear" << std::endl;
  }

  for (auto& iter : this->pins_) {
    iter->Print();
  }
}

void Simulator::Clear() {
  for (auto& iter : this->gates_) {
    delete iter;
  }

  for (auto& iter : this->pins_) {
    delete iter;
  }

  this->gates_.clear();
  this->pins_.clear();
}

void Simulator::Simulate() {
  for (auto& iter : this->gates_) {
    iter->Calculate();
  }

  this->Print();
}