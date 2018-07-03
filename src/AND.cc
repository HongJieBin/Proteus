#include "include/AND.h"

void AND::Calculate() { this->out_->SetLevel(*this->in1_ * *this->in2_); }