#include "include/XOR.h"

void XOR::Calculate() { this->out_->SetLevel(*this->in1_ ^ *this->in2_); }