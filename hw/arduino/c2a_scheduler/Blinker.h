#pragma once
#include <Arduino.h>

// Blink a LED
class Blinker {
public:                                           // anyone can call these
  Blinker(uint8_t pin)   // constructor: runs when the object is created
    : pin_(pin)  // initializer list: sets the members
  {}

  void begin();  // declared here, defined in .cpp
  bool isOn() const {
    return on_;
  }  // 'const' = promises not to change the object
  void toggle();

private:         // only the class's own functions can touch these
  uint8_t pin_;  // trailing _ marks a member (just a convention)
  bool on_ = false;
};