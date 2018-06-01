#include "include/XNOR.h"

void XNOR::Calculate() {
  Pin::level in1Level = this->_in1->GetLevel();
  Pin::level in2Level = this->_in2->GetLevel();

  if ((in1Level == HIGH && in2Level == LOW) ||
      (in1Level == LOW && in2Level == HIGH)) {
    this->_out->SetLevel(LOW);
    return;
  }

  this->_out->SetLevel(HIGH);
}