#include "include/pin.h"

Pin::Pin() : _name("Undefined"), _level(0) {}

Pin::Pin(std::string name) : _name(name), _level(0) {}

Pin::Pin(std::string name, Pin::level value) : _name(name), _level(value) {}

std::string Pin::GetName() const { return this->_name; }

Pin::level Pin::GetLevel() const { return this->_level; }

void Pin::SetName(std::string name) { this->_name = name; }

void Pin::SetLevel(Pin::level level) { this->_level = level; }

void Pin::Print() const {
  std::cout << this->_name << " " << (this->_level == HIGH ? "HIGH" : "LOW")
            << std::endl;
}