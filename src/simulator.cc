#include "include/simulator.h"
#define CHECK_PIN_NAME(name)                                        \
  if (name == "all" || name == "ALL") {                             \
    throw(std::string(                                              \
        "error: cannot use reserved word as the name of the pin")); \
  }

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

#define CHECK_SIMPLE_LOOP_NOT(in1, out)                        \
  if (this->CheckSimpleLoop(in1, out)) {                       \
    std::string error =                                        \
        "error: output cannot connect with input in the same " \
        "gate\n\tNOT in: " +                                   \
        in1 + " out: " + out;                                  \
    throw(error);                                              \
  }

#define CHECK_SIMPLE_LOOP(name, in1, in2, out)                              \
  if (this->CheckSimpleLoop(in1, out) || this->CheckSimpleLoop(in2, out)) { \
    std::string error =                                                     \
        "error: output cannot connect with input in the same "              \
        "gate\n\t" +                                                        \
        toUpperCase(name) + " in: " + in1 + " in: " + in2 + " out: " + out; \
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

#define COMMAND_PRINT(command, is)                                         \
  if (command == "PRINT") {                                                \
    std::string name;                                                      \
                                                                           \
    is >> name;                                                            \
                                                                           \
    if ((name != "ALL" && name != "all") && !this->SearchPin(name)) {      \
      std::string error =                                                  \
          "error: such pin does not exist\n\t" + name + " does not exist"; \
      throw(error);                                                        \
    }                                                                      \
                                                                           \
    toUpperCase(name) == "ALL" ? this->Print()                             \
                               : this->CheckPin(name)->Print();            \
    continue;                                                              \
  }

#define COMMAND_ADD(command, is)              \
  if (command == "ADD") {                     \
    Gate* gate = nullptr;                     \
    std::string name;                         \
    std::string in1, in2, out;                \
                                              \
    is >> name;                               \
    gate = this->AddGate(name);               \
                                              \
    if (!gate) {                              \
      throw("Unsupported gate " + name);      \
    }                                         \
                                              \
    if (toUpperCase(name) != "NOT") {         \
      is >> in1 >> in2 >> out;                \
      CHECK_PIN_NAME(in1)                     \
      CHECK_PIN_NAME(in2)                     \
      CHECK_PIN_NAME(out)                     \
      CHECK_SIMPLE_LOOP(name, in1, in2, out)  \
      gate->SetIn1(this->CheckPin(in1));      \
      gate->SetIn2(this->CheckPin(in2));      \
      CHECK_SET_OUT_AND_LOOP(gate, out, name) \
      continue;                               \
    }                                         \
                                              \
    is >> in1 >> out;                         \
    CHECK_PIN_NAME(in1)                       \
    CHECK_PIN_NAME(out)                       \
    CHECK_SIMPLE_LOOP_NOT(in1, out)           \
    gate->SetIn1(this->CheckPin(in1));        \
    gate->SetIn2(this->CheckPin(in1));        \
    CHECK_SET_OUT_AND_LOOP(gate, out, name)   \
    continue;                                 \
  }

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

bool Simulator::CheckLevel(Pin::level level) const {
  return level != 0 && level != 1;
}

bool Simulator::CheckOut(std::string pin) const {
  for (auto& iter : this->gates_) {
    if (!iter->GetOut()) {
      continue;
    }

    if (pin == iter->GetOut()->GetName()) {
      return true;
    }
  }

  return false;
}

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

bool Simulator::CheckLoop(std::string in, std::string out) const {
  bool connected = false;

  for (auto& iter : this->gates_) {
    if (!iter->GetIn1() || !iter->GetIn2() || !iter->GetOut()) {
      continue;
    }

    if (in == iter->GetIn1()->GetName() || in == iter->GetIn2()->GetName()) {
      connected = true;

      if (out == iter->GetOut()->GetName()) {
        return true;
      }

      this->CheckLoop(iter->GetOut()->GetName(), out);
    }
  }

  return connected;
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
  std::string temp;
  std::string command;

  do {
    std::cout << "proteus> ";
    std::cin >> temp;
    command = toUpperCase(temp);

    try {
      if (command == "EXIT") {
        this->Clear();
        break;
      }

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
      this->Clear();
      std::cout << error << std::endl;
      std::cin.clear();
      std::cin.sync();
    }
  } while (1);
}

void Simulator::Load(std::string file) {
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

#undef CHECK_PIN_NAME
#undef CHECK_SET_OUT_AND_LOOP
#undef CHECK_SIMPLE_LOOP_NOT
#undef CHECK_SIMPLE_LOOP
#undef COMMAND_ECHO
#undef COMMAND_PRINT
#undef COMMAND_ADD
#undef COMMAND_SET
#undef COMMAND_RESET
#undef COMMAND_SIM