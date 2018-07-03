#include "include/simulator.h"
#define CHECK_SIMPLE_LOOP_NOT(in1, out)                        \
  if (this->CheckSimpleLoop(in1, out)) {                       \
    std::string error =                                        \
        "error: output cannot connect with input in the same " \
        "gate\n\tNOT IN: " +                                   \
        in1 + " OUT: " + out;                                  \
    throw(error);                                              \
  }
#define CHECK_SIMPLE_LOOP(name, in1, in2, out)                              \
  if (this->CheckSimpleLoop(in1, out) || this->CheckSimpleLoop(in2, out)) { \
    std::string error =                                                     \
        "error: output cannot connect with input in the same "              \
        "gate\n\t" +                                                        \
        name + " IN: " + in1 + " IN: " + in2 + " OUT: " + out;              \
    throw(error);                                                           \
  }
#define COMMAND_ECHO(command, is)      \
  if (command == "ECHO") {             \
    std::string comment;               \
                                       \
    is >> comment;                     \
    std::cout << comment << std::endl; \
    continue;                          \
  }
#define COMMAND_PRINT(command, is)                              \
  if (command == "PRINT") {                                     \
    std::string name;                                           \
                                                                \
    is >> name;                                                 \
    toUpperCase(name) == "ALL" ? this->Print()                  \
                               : this->CheckPin(name)->Print(); \
    continue;                                                   \
  }
#define COMMAND_ADD(command, is)             \
  if (command == "ADD") {                    \
    Gate* gate = nullptr;                    \
    std::string name;                        \
    std::string in1, in2, out;               \
                                             \
    is >> name;                              \
    gate = this->AddGate(name);              \
                                             \
    if (!gate) {                             \
      FLUSH_CACHE                            \
      throw("Unsupported gate " + name);     \
    }                                        \
                                             \
    name = toUpperCase(name);                \
                                             \
    if (name != "NOT") {                     \
      is >> in1 >> in2 >> out;               \
                                             \
      CHECK_SIMPLE_LOOP(name, in1, in2, out) \
                                             \
      gate->SetIn1(this->CheckPin(in1));     \
      gate->SetIn2(this->CheckPin(in2));     \
      gate->SetOut(this->CheckPin(out));     \
      continue;                              \
    }                                        \
                                             \
    is >> in1 >> out;                        \
    CHECK_SIMPLE_LOOP_NOT(in1, out)          \
    gate->SetIn1(this->CheckPin(in1));       \
    gate->SetIn2(this->CheckPin(in1));       \
    gate->SetOut(this->CheckPin(out));       \
  }
#define COMMAND_SET(command, is)           \
  if (command == "SET") {                  \
    std::string name;                      \
    Pin::level level;                      \
                                           \
    is >> name >> level;                   \
    this->CheckPin(name)->SetLevel(level); \
    continue;                              \
  }
#define COMMAND_RESET(command)                         \
  if (command == "RESET") {                            \
    if (this->Empty()) {                               \
      throw(std::string("No pins or gates to clear")); \
    }                                                  \
                                                       \
    this->Clear();                                     \
    continue;                                          \
  }
#define COMMAND_SIM(command) \
  if (command == "SIM") {    \
    this->Simulate();        \
    continue;                \
  }

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

bool Simulator::CheckSimpleLoop(std::string pin1, std::string pin2) const {
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

  type = toUpperCase(type);

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
#define FLUSH_CACHE \
  std::cin.clear(); \
  std::cin.sync();

  std::string temp;
  std::string command;

  do {
    std::cout << "proteus> ";
    std::cin >> temp;
    command = toUpperCase(temp);

    try {
      if (command == "EXIT") break;

      if (command == "HELP") {
        std::cout << "Usage: [options]" << std::endl << std::endl;
        std::cout << "Proteus: An application for simulating logic circuits"
                  << std::endl
                  << std::endl;
        std::cout << "Options:" << std::endl << std::endl;
        std::cout << "\thelp\t\t\t\t\t\t\tOutput usage information"
                  << std::endl;
        std::cout << "\tclear\t\t\t\t\t\t\tClear the terminal screen"
                  << std::endl;
        std::cout << "\texit\t\t\t\t\t\t\tExit the program" << std::endl;
        std::cout << "\tload <file>\t\t\t\t\t\tLoad the existed circuit"
                  << std::endl;
        std::cout << "\tadd <gate> <in_pin> [<in_pin>] <out_pin>\t\tAdd "
                     "specified type of gate"
                  << std::endl;
        std::cout
            << "\tprint <pin> | all\t\t\t\t\tPrint the specific pin or all "
               "the pins"
            << std::endl;
        std::cout << "\treset\t\t\t\t\t\t\tReset the circuit to empty state"
                  << std::endl;
        std::cout << "\tsim\t\t\t\t\t\t\tSimulate the circuit" << std::endl;
        std::cout << std::endl;
        continue;
      }

      COMMAND_ECHO(command, std::cin)
      COMMAND_PRINT(command, std::cin)
      COMMAND_SET(command, std::cin)
      COMMAND_RESET(command)
      COMMAND_SIM(command)
      COMMAND_ADD(command, std::cin)

      if (command == "CLEAR") {
        system("cls");
        continue;
      }

      if (command == "LOAD") {
        std::string file;

        std::cin >> file;
        this->Load(file);
        this->Clear();
        continue;
      }

      throw(temp + ": command not found");
    } catch (const std::string error) {
      std::cout << error << std::endl;
    }
  } while (1);

#undef FLUSH_CACHE
}

void Simulator::Load(std::string file) {
#define FLUSH_CACHE

  std::ifstream fs;

  fs.open(file);

  while (!fs.eof()) {
    std::string chunk;

    fs >> chunk;
    chunk = toUpperCase(chunk);

    try {
      COMMAND_ECHO(chunk, fs)
      COMMAND_PRINT(chunk, fs)
      COMMAND_SET(chunk, fs)
      COMMAND_RESET(chunk)
      COMMAND_SIM(chunk)
      COMMAND_ADD(chunk, fs)
    } catch (std::string error) {
      std::cout << error << std::endl;
      break;
    }
  }

  fs.close();

#undef FLUSH_CACHE
}

void Simulator::Print() const {
  if (!this->pins_.size()) {
    throw(std::string("No pins or gates to clear"));
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
}

#undef CHECK_SIMPLE_LOOP_NOT
#undef CHECK_SIMPLE_LOOP
#undef COMMAND_ECHO
#undef COMMAND_PRINT
#undef COMMAND_ADD
#undef COMMAND_SET
#undef COMMAND_RESET
#undef COMMAND_SIM