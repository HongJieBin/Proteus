#include "include/gate.h"

Gate::Gate() : _in1(nullptr), _in2(nullptr), _out(nullptr) {}

Pin* Gate::GetIn1() const { return this->_in1; }

Pin* Gate::GetIn2() const { return this->_in2; }

Pin* Gate::GetOut() const { return this->_out; }

void Gate::SetIn1(Pin* in1) { this->_in1 = in1; }

void Gate::SetIn2(Pin* in2) { this->_in2 = in2; }

void Gate::SetOut(Pin* out) { this->_out = out; }