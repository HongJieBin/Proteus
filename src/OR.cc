#include "include/OR.h"

void OR::Calculate() { this->out_->SetLevel(*this->in1_ + *this->in2_); }