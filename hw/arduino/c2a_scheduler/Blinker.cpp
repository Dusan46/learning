#include "Blinker.h"

void Blinker::begin() {  
  pinMode(pin_, OUTPUT);
}

void Blinker::update(uint32_t now_ms) {
  // if half_period_ms_ has passed since the last toggle,
  // flip on_, write it to the pin, and remember when you did it.
  if (now_ms - last_toggle_ms_ >= half_period_ms_) {
    on_ = !on_;
    last_toggle_ms_ = now_ms;
    digitalWrite(pin_, on_);
  }
}