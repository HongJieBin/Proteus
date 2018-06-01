#include "include/NOR.h"

void NOR::Calculate() {
  Pin::level in1Level = this->_in1->GetLevel();
  Pin::level in2Level = this->_in2->GetLevel();

  if (in1Level == LOW || in2Level == LOW) {
    this->_out->SetLevel(LOW);
    return;
  }

  this->_out->SetLevel(HIGH);
}