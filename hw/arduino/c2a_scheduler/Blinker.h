#pragma once
#include <Arduino.h>

// Blink a LED
class Blinker {
public:                                           // anyone can call these
  Blinker(uint8_t pin, uint32_t half_period_ms)   // constructor: runs when the object is created
    : pin_(pin), half_period_ms_(half_period_ms)  // initializer list: sets the members
  {}

  void begin();  // declared here, defined below
  void update(uint32_t now_ms);
  bool isOn() const {
    return on_;
  }  // 'const' = promises not to change the object

private:         // only the class's own functions can touch these
  uint8_t pin_;  // trailing _ marks a member (just a convention)
  uint32_t half_period_ms_;
  uint32_t last_toggle_ms_ = 0;  // default value
  bool on_ = false;
};