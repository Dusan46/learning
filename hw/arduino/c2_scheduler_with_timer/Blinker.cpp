#include "Blinker.h"

void Blinker::begin() {  
  pinMode(pin_, OUTPUT);
}

void Blinker::toggle(){
  on_ = !on_;
  digitalWrite(pin_, on_);
}