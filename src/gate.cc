#include "include/gate.h"

Gate::Gate() : in1_(nullptr), in2_(nullptr), out_(nullptr) {}

Pin* Gate::GetIn1() const { return this->in1_; }

Pin* Gate::GetIn2() const { return this->in2_; }

Pin* Gate::GetOut() const { return this->out_; }

void Gate::SetIn1(Pin* in1) { this->in1_ = in1; }

void Gate::SetIn2(Pin* in2) { this->in2_ = in2; }

void Gate::SetOut(Pin* out) { this->out_ = out; }