#include "include/XNOR.h"

void XNOR::Calculate() {
  Pin::level in1Level = this->in1_->GetLevel();
  Pin::level in2Level = this->in2_->GetLevel();

  if ((in1Level == HIGH && in2Level == LOW) ||
      (in1Level == LOW && in2Level == HIGH)) {
    this->out_->SetLevel(LOW);
    return;
  }

  this->out_->SetLevel(HIGH);
}