#include "include/NOT.h"

void NOT::Calculate() {
  Pin::level inLevel = this->in1_->GetLevel();

  if (inLevel == HIGH) {
    this->out_->SetLevel(LOW);
    return;
  }

  this->out_->SetLevel(HIGH);
}