#include "include/pin.h"

Pin::Pin() : name_("Undefined"), level_(0) {}

Pin::Pin(std::string name) : name_(name), level_(0) {}

Pin::Pin(std::string name, Pin::level value) : name_(name), level_(value) {}

std::string Pin::GetName() const { return this->name_; }

Pin::level Pin::GetLevel() const { return this->level_; }

void Pin::SetName(std::string name) { this->name_ = name; }

void Pin::SetLevel(Pin::level level) { this->level_ = level; }

void Pin::Print() const {
  std::cout << this->name_ << " " << (this->level_ == HIGH ? "HIGH" : "LOW")
            << std::endl;
}