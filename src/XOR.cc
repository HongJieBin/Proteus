#include "include/XOR.h"

void XOR::Calculate() {
  Pin::level in1Level = this->_in1->GetLevel();
  Pin::level in2Level = this->_in2->GetLevel();

  if ((in1Level == HIGH && in2Level == LOW) ||
      (in1Level == LOW && in2Level == HIGH)) {
    this->_out->SetLevel(HIGH);
    return;
  }

  this->_out->SetLevel(LOW);
}