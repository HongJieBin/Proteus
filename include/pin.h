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

 private:
  std::string _name;
  Pin::level _level;
};

#endif