#ifndef PROTEUS_INCLUDE_GATE_H_
#define PROTEUS_INCLUDE_GATE_H_

#include "pin.h"

class Gate {
 public:
  Gate();
  virtual ~Gate() = default;

  Pin* GetIn1() const;
  Pin* GetIn2() const;
  Pin* GetOut() const;

  void SetIn1(Pin* in1);
  void SetIn2(Pin* in2);
  void SetOut(Pin* out);

  virtual void Calculate() = 0;

 protected:
  Pin* _in1;
  Pin* _in2;
  Pin* _out;
};

#endif