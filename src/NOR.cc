#include "include/NOR.h"

void NOR::Calculate() {
  Pin::level in1Level = this->in1_->GetLevel();
  Pin::level in2Level = this->in2_->GetLevel();

  if (in1Level == LOW || in2Level == LOW) {
    this->out_->SetLevel(LOW);
    return;
  }

  this->out_->SetLevel(HIGH);
}