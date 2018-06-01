#include "include/NOT.h"

void NOT::Calculate() {
  Pin::level inLevel = this->_in1->GetLevel();

  if (inLevel == HIGH) {
    this->_out->SetLevel(LOW);
    return;
  }

  this->_out->SetLevel(HIGH);
}