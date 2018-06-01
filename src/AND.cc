#include "include/AND.h"

void AND::Calculate() {
  Pin::level in1Level = this->_in1->GetLevel();
  Pin::level in2Level = this->_in2->GetLevel();

  if (in1Level == HIGH && in2Level == HIGH) {
    this->_out->SetLevel(HIGH);
    return;
  }

  this->_out->SetLevel(LOW);
}