#ifndef PROTEUS_INCLUDE_PIN_H_
#define PROTEUS_INCLUDE_PIN_H_

#include <iostream>
#include <string>

#define HIGH 1
#define LOW 0

class Pin {
 public:
  typedef short level;

  Pin();
  Pin(std::string name);
  Pin(std::string name, Pin::level value);
  ~Pin() = default;

  std::string GetName() const;
  Pin::level GetLevel() const;

  void SetName(std::string name);
  void SetLevel(Pin::level level);

  void Print() const;

  int operator+(const Pin& other) const;
  int operator*(const Pin& other) const;
  int operator!() const;
  int operator^(const Pin& other) const;

 private:
  std::string name_;
  Pin::level level_;
};

#endif